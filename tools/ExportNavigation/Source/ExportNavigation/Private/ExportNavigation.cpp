// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#include "ExportNavigationPrivatePCH.h"

#include "SlateBasics.h"
#include "SlateExtras.h"

#include "ExportNavigationStyle.h"
#include "ExportNavigationCommands.h"

#include "LevelEditor.h"


#include "ExportNavigationHead.h"

#include "Recast.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
//#include "AI/Navigation/RecastNavMeshGenerator.h"

DEFINE_LOG_CATEGORY(LogExportNavigation);
static const FName ExportNavigationTabName("ExportNavigation");

#define LOCTEXT_NAMESPACE "FExportNavigationModule"



void FExportNavigationModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module

	FExportNavigationStyle::Initialize();
	FExportNavigationStyle::ReloadTextures();

	FExportNavigationCommands::Register();

	PluginCommands = MakeShareable(new FUICommandList);

	PluginCommands->MapAction(
		FExportNavigationCommands::Get().PluginAction,
		FExecuteAction::CreateRaw(this, &FExportNavigationModule::PluginButtonClicked),
		FCanExecuteAction());

	FLevelEditorModule& LevelEditorModule = FModuleManager::LoadModuleChecked<FLevelEditorModule>("LevelEditor");

	{
		TSharedPtr<FExtender> MenuExtender = MakeShareable(new FExtender());
		MenuExtender->AddMenuExtension("WindowLayout", EExtensionHook::After, PluginCommands, FMenuExtensionDelegate::CreateRaw(this, &FExportNavigationModule::AddMenuExtension));

		LevelEditorModule.GetMenuExtensibilityManager()->AddExtender(MenuExtender);
	}

	{
		TSharedPtr<FExtender> ToolbarExtender = MakeShareable(new FExtender);
		ToolbarExtender->AddToolBarExtension("Settings", EExtensionHook::After, PluginCommands, FToolBarExtensionDelegate::CreateRaw(this, &FExportNavigationModule::AddToolbarExtension));

		LevelEditorModule.GetToolBarExtensibilityManager()->AddExtender(ToolbarExtender);
	}

	m_ctx = new original::rcContext();
}

void FExportNavigationModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
	FExportNavigationStyle::Shutdown();

	FExportNavigationCommands::Unregister();
	delete m_ctx;
}

UWorld* _GetWorld()
{
	UWorld* world = NULL;
	if (GIsEditor && GEditor->PlayWorld && !GIsPlayInEditorWorld)
	{
		world = GEditor->PlayWorld;
	}
	ULocalPlayer* Player = GEngine->GetDebugLocalPlayer();
	if (Player)
	{
		UWorld* PlayerWorld = Player->GetWorld();
		if (!world)
		{
			world = PlayerWorld;
		}
	}

	if (!world)
	{
		world = GEditor->GetEditorWorldContext().World();
	}
	return world;
}

inline unsigned int nextPow2(unsigned int v)
{
	v--;
	v |= v >> 1;
	v |= v >> 2;
	v |= v >> 4;
	v |= v >> 8;
	v |= v >> 16;
	v++;
	return v;
}

inline unsigned int ilog2(unsigned int v)
{
	unsigned int r;
	unsigned int shift;
	r = (v > 0xffff) << 4; v >>= r;
	shift = (v > 0xff) << 3; v >>= shift; r |= shift;
	shift = (v > 0xf) << 2; v >>= shift; r |= shift;
	shift = (v > 0x3) << 1; v >>= shift; r |= shift;
	r |= (v >> 1);
	return r;
}

void _TransformVertexSoupToRecast(const TArray<FVector>& VertexSoup, TNavStatArray<FVector>& Verts, TNavStatArray<int32>& Faces)
{
	if (VertexSoup.Num() == 0)
	{
		return;
	}

	check(VertexSoup.Num() % 3 == 0);

	const int32 StaticFacesCount = VertexSoup.Num() / 3;
	int32 VertsCount = Verts.Num();
	const FVector* Vertex = VertexSoup.GetData();

	for (int32 k = 0; k < StaticFacesCount; ++k, Vertex += 3)
	{
		Verts.Add(Unreal2RecastPoint(Vertex[0]));
		Verts.Add(Unreal2RecastPoint(Vertex[1]));
		Verts.Add(Unreal2RecastPoint(Vertex[2]));
		Faces.Add(VertsCount + 2);
		Faces.Add(VertsCount + 1);
		Faces.Add(VertsCount + 0);

		VertsCount += 3;
	}
}


void _GrowConvexHull(const float ExpandBy, const TArray<FVector>& Verts, TArray<FVector>& OutResult)
{
	if (Verts.Num() < 3)
	{
		return;
	}

	struct FSimpleLine
	{
		FVector P1, P2;

		FSimpleLine() {}

		FSimpleLine(FVector Point1, FVector Point2)
			: P1(Point1), P2(Point2)
		{

		}
		static FVector Intersection(const FSimpleLine& Line1, const FSimpleLine& Line2)
		{
			const float A1 = Line1.P2.X - Line1.P1.X;
			const float B1 = Line2.P1.X - Line2.P2.X;
			const float C1 = Line2.P1.X - Line1.P1.X;

			const float A2 = Line1.P2.Y - Line1.P1.Y;
			const float B2 = Line2.P1.Y - Line2.P2.Y;
			const float C2 = Line2.P1.Y - Line1.P1.Y;

			const float Denominator = A2*B1 - A1*B2;
			if (Denominator != 0)
			{
				const float t = (B1*C2 - B2*C1) / Denominator;
				return Line1.P1 + t * (Line1.P2 - Line1.P1);
			}

			return FVector::ZeroVector;
		}
	};

	TArray<FVector> AllVerts(Verts);
	AllVerts.Add(Verts[0]);
	AllVerts.Add(Verts[1]);

	const int32 VertsCount = AllVerts.Num();
	const FQuat Rotation90(FVector(0, 0, 1), FMath::DegreesToRadians(90));

	float RotationAngle = MAX_FLT;
	for (int32 Index = 0; Index < VertsCount - 2; ++Index)
	{
		const FVector& V1 = AllVerts[Index + 0];
		const FVector& V2 = AllVerts[Index + 1];
		const FVector& V3 = AllVerts[Index + 2];

		const FVector V01 = (V1 - V2).GetSafeNormal();
		const FVector V12 = (V2 - V3).GetSafeNormal();
		const FVector NV1 = Rotation90.RotateVector(V01);
		const float d = FVector::DotProduct(NV1, V12);

		if (d < 0)
		{
			// CW
			RotationAngle = -90;
			break;
		}
		else if (d > 0)
		{
			//CCW
			RotationAngle = 90;
			break;
		}
	}

	// check if we detected CW or CCW direction
	if (RotationAngle >= BIG_NUMBER)
	{
		return;
	}

	const float ExpansionThreshold = 2 * ExpandBy;
	const float ExpansionThresholdSQ = ExpansionThreshold * ExpansionThreshold;
	const FQuat Rotation(FVector(0, 0, 1), FMath::DegreesToRadians(RotationAngle));
	FSimpleLine PreviousLine;
	OutResult.Reserve(Verts.Num());
	for (int32 Index = 0; Index < VertsCount - 2; ++Index)
	{
		const FVector& V1 = AllVerts[Index + 0];
		const FVector& V2 = AllVerts[Index + 1];
		const FVector& V3 = AllVerts[Index + 2];

		FSimpleLine Line1;
		if (Index > 0)
		{
			Line1 = PreviousLine;
		}
		else
		{
			const FVector V01 = (V1 - V2).GetSafeNormal();
			const FVector N1 = Rotation.RotateVector(V01).GetSafeNormal();
			const FVector MoveDir1 = N1 * ExpandBy;
			Line1 = FSimpleLine(V1 + MoveDir1, V2 + MoveDir1);
		}

		const FVector V12 = (V2 - V3).GetSafeNormal();
		const FVector N2 = Rotation.RotateVector(V12).GetSafeNormal();
		const FVector MoveDir2 = N2 * ExpandBy;
		const FSimpleLine Line2(V2 + MoveDir2, V3 + MoveDir2);

		const FVector NewPoint = FSimpleLine::Intersection(Line1, Line2);
		if (NewPoint == FVector::ZeroVector)
		{
			// both lines are parallel so just move our point by expansion distance
			OutResult.Add(V2 + MoveDir2);
		}
		else
		{
			const FVector VectorToNewPoint = NewPoint - V2;
			const float DistToNewVector = VectorToNewPoint.SizeSquared2D();
			if (DistToNewVector > ExpansionThresholdSQ)
			{
				//clamp our point to not move to far from original location
				const FVector HelpPos = V2 + VectorToNewPoint.GetSafeNormal2D() * ExpandBy * 1.4142;
				OutResult.Add(HelpPos);
			}
			else
			{
				OutResult.Add(NewPoint);
			}
		}

		PreviousLine = Line2;
	}
}

unsigned char* FExportNavigationModule::_BuildTileMesh(const original::InputGeom* geom, const int tx, const int ty, const float* bmin, const float* bmax, int& dataSize, const FRecastBuildConfig& config)
{
	const float* verts = geom->getMesh()->getVerts();
	const int nverts = geom->getMesh()->getVertCount();
	const int ntris = geom->getMesh()->getTriCount();
	const original::rcChunkyTriMesh* chunkyMesh = geom->getChunkyMesh();

	original::rcFreeHeightField(m_solid);
	original::rcFreeCompactHeightfield(m_chf);
	original::rcFreeContourSet(m_cset);
	original::rcFreePolyMesh(m_pmesh);
	original::rcFreePolyMeshDetail(m_dmesh);

	m_solid = NULL;
	m_chf = NULL;
	m_cset = NULL;
	m_pmesh = NULL;
	m_dmesh = NULL;

	FRecastBuildConfig cfg = config;
	cfg.width = cfg.tileSize + cfg.borderSize * 2;
	cfg.height = cfg.tileSize + cfg.borderSize * 2;
	original::rcVcopy(cfg.bmin, bmin);
	original::rcVcopy(cfg.bmax, bmax);
	cfg.bmin[0] -= cfg.borderSize*cfg.cs;
	cfg.bmin[2] -= cfg.borderSize*cfg.cs;
	cfg.bmax[0] += cfg.borderSize*cfg.cs;
	cfg.bmax[2] += cfg.borderSize*cfg.cs;

	// Allocate voxel heightfield where we rasterize our input data to.
	m_solid = original::rcAllocHeightfield();
	if (!m_solid)
	{
		UE_LOG(LogExportNavigation, Error, TEXT("buildNavigation: Out of memory 'solid'."));
		return 0;
	}
	if (!rcCreateHeightfield(m_ctx, *m_solid, cfg.width, cfg.height, cfg.bmin, cfg.bmax, cfg.cs, cfg.ch))
	{
		UE_LOG(LogExportNavigation, Error, TEXT("buildNavigation: Could not create solid heightfield."));
	}

	unsigned char* triareas = new unsigned char[chunkyMesh->maxTrisPerChunk];
	if (!triareas)
	{
		UE_LOG(LogExportNavigation, Error, TEXT("buildNavigation: Out of memory 'm_triareas' (%d)."), chunkyMesh->maxTrisPerChunk);
		return 0;
	}

	float tbmin[2], tbmax[2];
	tbmin[0] = cfg.bmin[0];
	tbmin[1] = cfg.bmin[2];
	tbmax[0] = cfg.bmax[0];
	tbmax[1] = cfg.bmax[2];
	int cid[512];// TODO: Make grow when returning too many items.
	const int ncid = original::rcGetChunksOverlappingRect(chunkyMesh, tbmin, tbmax, cid, 512);
	if (!ncid)
		return 0;

	for (int i = 0; i < ncid; ++i)
	{
		const original::rcChunkyTriMeshNode& node = chunkyMesh->nodes[cid[i]];
		const int* ctris = &chunkyMesh->tris[node.i * 3];
		const int nctris = node.n;

		memset(triareas, 0, nctris * sizeof(unsigned char));
		original::rcMarkWalkableTriangles(m_ctx, cfg.walkableSlopeAngle,
			verts, nverts, ctris, nctris, triareas);

		original::rcRasterizeTriangles(m_ctx, verts, nverts, ctris, triareas, nctris, *m_solid, cfg.walkableClimb);
	}

	delete[] triareas;
	triareas = 0;

	// Once all geometry is rasterized, we do initial pass of filtering to
	// remove unwanted overhangs caused by the conservative rasterization
	// as well as filter spans where the character cannot possibly stand.
	original::rcFilterLowHangingWalkableObstacles(m_ctx, cfg.walkableClimb, *m_solid);
	original::rcFilterLedgeSpans(m_ctx, cfg.walkableHeight, cfg.walkableClimb, *m_solid);
	original::rcFilterWalkableLowHeightSpans(m_ctx, cfg.walkableHeight, *m_solid);

	// Compact the heightfield so that it is faster to handle from now on.
	// This will result more cache coherent data as well as the neighbours
	// between walkable cells will be calculated.
	m_chf = original::rcAllocCompactHeightfield();
	if (!m_chf)
	{
		UE_LOG(LogExportNavigation, Error, TEXT("buildNavigation: Out of memory 'chf'."));
		return 0;
	}
	if (!original::rcBuildCompactHeightfield(m_ctx, cfg.walkableHeight, cfg.walkableClimb, *m_solid, *m_chf))
	{
		UE_LOG(LogExportNavigation, Error, TEXT("buildNavigation: Could not build compact data."));
		return 0;
	}

	original::rcFreeHeightField(m_solid);
	m_solid = 0;

	// Erode the walkable area by agent radius.
	if (!original::rcErodeWalkableArea(m_ctx, cfg.walkableRadius, *m_chf))
	{
		UE_LOG(LogExportNavigation, Error, TEXT("buildNavigation: Could not erode."));
		return 0;
	}

	// (Optional) Mark areas.
	const original::ConvexVolume* vols = geom->getConvexVolumes();
	for (int i = 0; i < geom->getConvexVolumeCount(); ++i)
		original::rcMarkConvexPolyArea(m_ctx, vols[i].verts, vols[i].nverts, vols[i].hmin, vols[i].hmax, (unsigned char)vols[i].area, *m_chf);


	// Partition the heightfield so that we can use simple algorithm later to triangulate the walkable areas.
	// There are 3 martitioning methods, each with some pros and cons:
	// 1) Watershed partitioning
	//   - the classic Recast partitioning
	//   - creates the nicest tessellation
	//   - usually slowest
	//   - partitions the heightfield into nice regions without holes or overlaps
	//   - the are some corner cases where this method creates produces holes and overlaps
	//      - holes may appear when a small obstacles is close to large open area (triangulation can handle this)
	//      - overlaps may occur if you have narrow spiral corridors (i.e stairs), this make triangulation to fail
	//   * generally the best choice if you precompute the nacmesh, use this if you have large open areas
	// 2) Monotone partioning
	//   - fastest
	//   - partitions the heightfield into regions without holes and overlaps (guaranteed)
	//   - creates long thin polygons, which sometimes causes paths with detours
	//   * use this if you want fast navmesh generation
	// 3) Layer partitoining
	//   - quite fast
	//   - partitions the heighfield into non-overlapping regions
	//   - relies on the triangulation code to cope with holes (thus slower than monotone partitioning)
	//   - produces better triangles than monotone partitioning
	//   - does not have the corner cases of watershed partitioning
	//   - can be slow and create a bit ugly tessellation (still better than monotone)
	//     if you have large open areas with small obstacles (not a problem if you use tiles)
	//   * good choice to use for tiled navmesh with medium and small sized tiles


		// Prepare for region partitioning, by calculating distance field along the walkable surface.
	if (!original::rcBuildDistanceField(m_ctx, *m_chf))
	{
		UE_LOG(LogExportNavigation, Error, TEXT("buildNavigation: Could not build distance field."));
		return 0;
	}

	// Partition the walkable surface into simple regions without holes.
	if (!original::rcBuildRegions(m_ctx, *m_chf, cfg.borderSize, cfg.minRegionArea, cfg.mergeRegionArea))
	{
		UE_LOG(LogExportNavigation, Error, TEXT("buildNavigation: Could not build watershed regions."));
		return 0;
	}


	// Create contours.
	m_cset = original::rcAllocContourSet();
	if (!m_cset)
	{
		UE_LOG(LogExportNavigation, Error, TEXT("buildNavigation: Out of memory 'cset'."));
		return 0;
	}
	if (!original::rcBuildContours(m_ctx, *m_chf, cfg.maxSimplificationError, cfg.maxEdgeLen, *m_cset))
	{
		UE_LOG(LogExportNavigation, Error, TEXT("buildNavigation: Could not create contours."));
		return 0;
	}

	if (m_cset->nconts == 0)
	{
		return 0;
	}

	// Build polygon navmesh from the contours.
	m_pmesh = original::rcAllocPolyMesh();
	if (!m_pmesh)
	{
		UE_LOG(LogExportNavigation, Error, TEXT("buildNavigation: Out of memory 'pmesh'."));
		return 0;
	}
	if (!original::rcBuildPolyMesh(m_ctx, *m_cset, cfg.maxVertsPerPoly, *m_pmesh))
	{
		UE_LOG(LogExportNavigation, Error, TEXT("buildNavigation: Could not triangulate contours."));
		return 0;
	}

	// Build detail mesh.
	m_dmesh = original::rcAllocPolyMeshDetail();
	if (!m_dmesh)
	{
		UE_LOG(LogExportNavigation, Error, TEXT("buildNavigation: Out of memory 'dmesh'."));
		return 0;
	}

	if (!original::rcBuildPolyMeshDetail(m_ctx, *m_pmesh, *m_chf,
		cfg.detailSampleDist, cfg.detailSampleMaxError,
		*m_dmesh))
	{
		UE_LOG(LogExportNavigation, Error, TEXT("buildNavigation: Could build polymesh detail."));
		return 0;
	}

	original::rcFreeCompactHeightfield(m_chf);
	m_chf = 0;
	original::rcFreeContourSet(m_cset);
	m_cset = 0;

	unsigned char* navData = 0;
	int navDataSize = 0;
	if (cfg.maxVertsPerPoly <= original::DT_VERTS_PER_POLYGON)
	{
		if (m_pmesh->nverts >= 0xffff)
		{
			UE_LOG(LogExportNavigation, Error, TEXT("Too many vertices per tile %d(max: %d)."), m_pmesh->nverts, 0xffff);
			// The vertex indices are ushorts, and cannot point to more than 0xffff vertices.
			return 0;
		}

		// Update poly flags from areas.
		for (int i = 0; i < m_pmesh->npolys; ++i)
		{
			if (m_pmesh->areas[i] == RC_WALKABLE_AREA)
				m_pmesh->areas[i] = HADES_POLYAREA_GROUND;

			if (m_pmesh->areas[i] == HADES_POLYAREA_GROUND ||
				m_pmesh->areas[i] == HADES_POLYAREA_GRASS ||
				m_pmesh->areas[i] == HADES_POLYAREA_ROAD)
			{
				m_pmesh->flags[i] = HADES_POLYFLAGS_WALK;
			}
			else if (m_pmesh->areas[i] == HADES_POLYAREA_WATER)
			{
				m_pmesh->flags[i] = HADES_POLYFLAGS_SWIM;
			}
			else if (m_pmesh->areas[i] == HADES_POLYAREA_DOOR)
			{
				m_pmesh->flags[i] = HADES_POLYFLAGS_WALK | HADES_POLYFLAGS_DOOR;
			}
		}

		original::dtNavMeshCreateParams params;
		memset(&params, 0, sizeof(params));
		params.verts = m_pmesh->verts;
		params.vertCount = m_pmesh->nverts;
		params.polys = m_pmesh->polys;
		params.polyAreas = m_pmesh->areas;
		params.polyFlags = m_pmesh->flags;
		params.polyCount = m_pmesh->npolys;
		params.nvp = m_pmesh->nvp;
		params.detailMeshes = m_dmesh->meshes;
		params.detailVerts = m_dmesh->verts;
		params.detailVertsCount = m_dmesh->nverts;
		params.detailTris = m_dmesh->tris;
		params.detailTriCount = m_dmesh->ntris;
		params.offMeshConVerts = geom->getOffMeshConnectionVerts();
		params.offMeshConRad = geom->getOffMeshConnectionRads();
		params.offMeshConDir = geom->getOffMeshConnectionDirs();
		params.offMeshConAreas = geom->getOffMeshConnectionAreas();
		params.offMeshConFlags = geom->getOffMeshConnectionFlags();
		params.offMeshConUserID = geom->getOffMeshConnectionId();
		params.offMeshConCount = geom->getOffMeshConnectionCount();
		params.walkableHeight = cfg.AgentHeight;
		params.walkableRadius = cfg.AgentRadius;
		params.walkableClimb = cfg.AgentMaxClimb;
		params.tileX = tx;
		params.tileY = ty;
		params.tileLayer = 0;
		original::rcVcopy(params.bmin, m_pmesh->bmin);
		original::rcVcopy(params.bmax, m_pmesh->bmax);
		params.cs = cfg.cs;
		params.ch = cfg.ch;
		params.buildBvTree = true;

		if (!original::dtCreateNavMeshData(&params, &navData, &navDataSize))
		{
			UE_LOG(LogExportNavigation, Error, TEXT("Could not build Detour navmesh."));
			return 0;
		}
	}

	dataSize = navDataSize;
	return navData;
}

bool FExportNavigationModule::_BuildAllTile(original::dtNavMesh* navMesh, const original::InputGeom* geom, const FRecastBuildConfig& config)
{
	float lastBuiltTileBmin[3];
	float lastBuiltTileBmax[3];
	int gw = 0, gh = 0;
	original::rcCalcGridSize(config.bmin, config.bmax, config.cs, &gw, &gh);
	const int ts = (int)config.tileSize;
	const int tw = (gw + ts - 1) / ts;
	const int th = (gh + ts - 1) / ts;
	const float tcs = config.tileSize*config.cs;

	for (int y = 0; y < th; ++y)
	{
		for (int x = 0; x < tw; ++x)
		{
			lastBuiltTileBmin[0] = config.bmin[0] + x*tcs;
			lastBuiltTileBmin[1] = config.bmin[1];
			lastBuiltTileBmin[2] = config.bmin[2] + y*tcs;

			lastBuiltTileBmax[0] = config.bmin[0] + (x + 1)*tcs;
			lastBuiltTileBmax[1] = config.bmax[1];
			lastBuiltTileBmax[2] = config.bmin[2] + (y + 1)*tcs;

			int dataSize = 0;
			unsigned char* data = _BuildTileMesh(geom, x, y, lastBuiltTileBmin, lastBuiltTileBmax, dataSize, config);
			if (data)
			{
				// Remove any previous data (navmesh owns and deletes the data).
				navMesh->removeTile(navMesh->getTileRefAt(x, y, 0), 0, 0);
				// Let the navmesh own the data.
				original::dtStatus status = navMesh->addTile(data, dataSize, original::DT_TILE_FREE_DATA, 0, 0);
				if (original::dtStatusFailed(status))
				{
					UE_LOG(LogExportNavigation, Error, TEXT("Failed to add tile to navMesh at %d %d"), x, y);
					original::dtFree(data);
					return false;
				}
			}
		}
	}
	return true;
}

void FExportNavigationModule::_MeshToNavMesh(const original::InputGeom* geom, const FRecastBuildConfig& config, const FString& FileName)
{
	original::dtNavMesh* navMesh = original::dtAllocNavMesh();
	if (!navMesh)
	{
		UE_LOG(LogExportNavigation, Error, TEXT("Failed to alloc navMesh"));
		return;
	}
	original::dtNavMeshParams params;
	original::rcVcopy(params.orig, geom->getNavMeshBoundsMin());
	params.tileWidth = config.tileSize*config.cs;
	params.tileHeight = config.tileSize*config.cs;

	int gw = 0, gh = 0;
	original::rcCalcGridSize(config.bmin, config.bmax, config.cs, &gw, &gh);
	const int ts = (int)config.tileSize;
	const int tw = (gw + ts - 1) / ts;
	const int th = (gh + ts - 1) / ts;

	int tileBits = rcMin((int)ilog2(nextPow2(tw*th)), 14);
	if (tileBits > 14) tileBits = 14;
	int polyBits = 22 - tileBits;

	params.maxTiles = 1 << tileBits;
	params.maxPolys = 1 << polyBits;
	original::dtStatus status;

	status = navMesh->init(&params);
	if (original::dtStatusFailed(status))
	{
		UE_LOG(LogExportNavigation, Error, TEXT("Failed to init navMesh"));
		original::dtFreeNavMesh(navMesh);
		return;
	}

	if (_BuildAllTile(navMesh, geom, config))
	{
		FString fullpath = FString::Printf(TEXT("%s/%s.nav"), *FPaths::GameSavedDir(), *FileName);
		_Saveall(fullpath, navMesh);
		UE_LOG(LogExportNavigation, Verbose, TEXT("Export navigation done!"));
	}
	original::dtFreeNavMesh(navMesh);
}

void FExportNavigationModule::_Saveall(const FString& path, const original::dtNavMesh* mesh)
{
	FArchive* FileAr = IFileManager::Get().CreateFileWriter(*path);

	NavMeshSetHeader header;
	header.magic = NAVMESHSET_MAGIC;
	header.version = NAVMESHSET_VERSION;
	header.numTiles = 0;
	for (int i = 0; i < mesh->getMaxTiles(); ++i)
	{
		const original::dtMeshTile* tile = mesh->getTile(i);
		if (!tile || !tile->header || !tile->dataSize) continue;
		header.numTiles++;
	}
	memcpy(&header.params, mesh->getParams(), sizeof(original::dtNavMeshParams));
	FileAr->Serialize((ANSICHAR*)&header, sizeof(NavMeshSetHeader));

	// Store tiles.
	for (int i = 0; i < mesh->getMaxTiles(); ++i)
	{
		const original::dtMeshTile* tile = mesh->getTile(i);
		if (!tile || !tile->header || !tile->dataSize) continue;

		NavMeshTileHeader tileHeader;
		tileHeader.tileRef = mesh->getTileRef(tile);
		tileHeader.dataSize = tile->dataSize;
		FileAr->Serialize((ANSICHAR*)&tileHeader, sizeof(tileHeader));

		FileAr->Serialize((ANSICHAR*)tile->data, tile->dataSize);
	}
	FileAr->Close();
}

void FExportNavigationModule::_ExportNavMesh(const ARecastNavMesh* NavData, const FNavigationOctree* NavOctree, const FString& FileName)
{
	struct FAreaExportData
	{
		FConvexNavAreaData Convex;
		uint8 AreaId;
	};
	TArray<FAreaExportData> AreaExport;
	TNavStatArray<float> CoordBuffer;
	TNavStatArray<int32> IndexBuffer;

	const FRecastNavMeshGenerator * Generator = (FRecastNavMeshGenerator*)NavData->GetGenerator();
	FBox TotalNavBounds = Generator->GetTotalBounds();
	const ARecastNavMesh* DestNavMesh = Generator->GetOwner();
	for (FNavigationOctree::TConstElementBoxIterator<FNavigationOctree::DefaultStackAllocator> It(*NavOctree, TotalNavBounds);
		It.HasPendingElements();
		It.Advance())
	{
		const FNavigationOctreeElement& Element = It.GetCurrentElement();
		const bool bExportGeometry = Element.Data->HasGeometry() && Element.ShouldUseGeometry(DestNavMesh->GetConfig());

		if (bExportGeometry && Element.Data->CollisionData.Num())
		{
			BRecastGeometryCache CachedGeometry(Element.Data->CollisionData.GetData());
			IndexBuffer.Reserve(IndexBuffer.Num() + (CachedGeometry.Header.NumFaces * 3));
			CoordBuffer.Reserve(CoordBuffer.Num() + (CachedGeometry.Header.NumVerts * 3));
			for (int32 i = 0; i < CachedGeometry.Header.NumFaces * 3; i++)
			{
				IndexBuffer.Add(CachedGeometry.Indices[i] + CoordBuffer.Num() / 3);
			}
			for (int32 i = 0; i < CachedGeometry.Header.NumVerts * 3; i++)
			{
				CoordBuffer.Add(CachedGeometry.Verts[i]);
			}
		}
		else
		{
			const TArray<FAreaNavModifier>& AreaMods = Element.Data->Modifiers.GetAreas();
			for (int32 i = 0; i < AreaMods.Num(); i++)
			{
				FAreaExportData ExportInfo;
				ExportInfo.AreaId = NavData->GetAreaID(AreaMods[i].GetAreaClass());

				if (AreaMods[i].GetShapeType() == ENavigationShapeType::Convex)
				{
					AreaMods[i].GetConvex(ExportInfo.Convex);

					TArray<FVector> ConvexVerts;
					_GrowConvexHull(NavData->AgentRadius, ExportInfo.Convex.Points, ConvexVerts);
					ExportInfo.Convex.MinZ -= NavData->CellHeight;
					ExportInfo.Convex.MaxZ += NavData->CellHeight;
					ExportInfo.Convex.Points = ConvexVerts;

					AreaExport.Add(ExportInfo);
				}
			}
		}
	}

	UWorld* NavigationWorld = _GetWorld();
	for (int32 LevelIndex = 0; LevelIndex < NavigationWorld->GetNumLevels(); ++LevelIndex)
	{
		const ULevel* const Level = NavigationWorld->GetLevel(LevelIndex);
		if (Level == NULL)
		{
			continue;
		}

		const TArray<FVector>* LevelGeom = Level->GetStaticNavigableGeometry();
		if (LevelGeom != NULL && LevelGeom->Num() > 0)
		{
			TNavStatArray<FVector> Verts;
			TNavStatArray<int32> Faces;
			// For every ULevel in World take its pre-generated static geometry vertex soup
			_TransformVertexSoupToRecast(*LevelGeom, Verts, Faces);

			IndexBuffer.Reserve(IndexBuffer.Num() + Faces.Num());
			CoordBuffer.Reserve(CoordBuffer.Num() + Verts.Num() * 3);
			for (int32 i = 0; i < Faces.Num(); i++)
			{
				IndexBuffer.Add(Faces[i] + CoordBuffer.Num() / 3);
			}
			for (int32 i = 0; i < Verts.Num(); i++)
			{
				CoordBuffer.Add(Verts[i].X);
				CoordBuffer.Add(Verts[i].Y);
				CoordBuffer.Add(Verts[i].Z);
			}
		}
	}

	original::InputGeom* geom = new original::InputGeom();
	geom->Init(CoordBuffer, IndexBuffer);
	FRecastBuildConfig config = Generator->GetConfig();
	//config.tileSize = NavData->TileSizeUU;
	original::rcVcopy(config.bmin, geom->getNavMeshBoundsMin());
	original::rcVcopy(config.bmax, geom->getNavMeshBoundsMax());
	_MeshToNavMesh(geom, config, FileName);

	delete geom;

	//AreaExportStr这个暂时不知道作用，先不处理了
	//FString AreaExportStr;
	//for (int32 i = 0; i < AreaExport.Num(); i++)
	//{
	//	const FAreaExportData& ExportInfo = AreaExport[i];
	//	AreaExportStr += FString::Printf(TEXT("\nAE %d %d %f %f\n"),
	//		ExportInfo.AreaId, ExportInfo.Convex.Points.Num(), ExportInfo.Convex.MinZ, ExportInfo.Convex.MaxZ);

	//	for (int32 iv = 0; iv < ExportInfo.Convex.Points.Num(); iv++)
	//	{
	//		FVector Pt = Unreal2RecastPoint(ExportInfo.Convex.Points[iv]);
	//		AreaExportStr += FString::Printf(TEXT("Av %f %f %f\n"), Pt.X, Pt.Y, Pt.Z);
	//	}
	//}

	//FString AdditionalData;

	//if (AreaExport.Num())
	//{
	//	AdditionalData += "# Area export\n";
	//	AdditionalData += AreaExportStr;
	//	AdditionalData += "\n";
	//}

	//AdditionalData += "# RecastDemo specific data\n";
	//// this bounds match navigation bounds from level
	//FBox RCNavBounds = Unreal2RecastBox(TotalNavBounds);
	//const FVector Center = RCNavBounds.GetCenter();
	//const FVector Extent = RCNavBounds.GetExtent();
	//const FBox Box = FBox::BuildAABB(Center, Extent);
	//AdditionalData += FString::Printf(
	//	TEXT("rd_bbox %7.7f %7.7f %7.7f %7.7f %7.7f %7.7f\n"),
	//	Box.Min.X, Box.Min.Y, Box.Min.Z,
	//	Box.Max.X, Box.Max.Y, Box.Max.Z
	//);

	//const FRecastNavMeshGenerator* CurrentGen = static_cast<const FRecastNavMeshGenerator*>(NavData->GetGenerator());
	//check(CurrentGen);
	//AdditionalData += FString::Printf(TEXT("# AgentHeight\n"));
	//AdditionalData += FString::Printf(TEXT("rd_agh %5.5f\n"), CurrentGen->Config.AgentHeight);
	//AdditionalData += FString::Printf(TEXT("# AgentRadius\n"));
	//AdditionalData += FString::Printf(TEXT("rd_agr %5.5f\n"), CurrentGen->Config.AgentRadius);

	//AdditionalData += FString::Printf(TEXT("# Cell Size\n"));
	//AdditionalData += FString::Printf(TEXT("rd_cs %5.5f\n"), CurrentGen->Config.cs);
	//AdditionalData += FString::Printf(TEXT("# Cell Height\n"));
	//AdditionalData += FString::Printf(TEXT("rd_ch %5.5f\n"), CurrentGen->Config.ch);

	//AdditionalData += FString::Printf(TEXT("# Agent max climb\n"));
	//AdditionalData += FString::Printf(TEXT("rd_amc %d\n"), (int)CurrentGen->Config.AgentMaxClimb);
	//AdditionalData += FString::Printf(TEXT("# Agent max slope\n"));
	//AdditionalData += FString::Printf(TEXT("rd_ams %5.5f\n"), CurrentGen->Config.walkableSlopeAngle);

	//AdditionalData += FString::Printf(TEXT("# Region min size\n"));
	//AdditionalData += FString::Printf(TEXT("rd_rmis %d\n"), (uint32)FMath::Sqrt(CurrentGen->Config.minRegionArea));
	//AdditionalData += FString::Printf(TEXT("# Region merge size\n"));
	//AdditionalData += FString::Printf(TEXT("rd_rmas %d\n"), (uint32)FMath::Sqrt(CurrentGen->Config.mergeRegionArea));

	//AdditionalData += FString::Printf(TEXT("# Max edge len\n"));
	//AdditionalData += FString::Printf(TEXT("rd_mel %d\n"), CurrentGen->Config.maxEdgeLen);

	//AdditionalData += FString::Printf(TEXT("# Perform Voxel Filtering\n"));
	//AdditionalData += FString::Printf(TEXT("rd_pvf %d\n"), CurrentGen->Config.bPerformVoxelFiltering);
	//AdditionalData += FString::Printf(TEXT("# Generate Detailed Mesh\n"));
	//AdditionalData += FString::Printf(TEXT("rd_gdm %d\n"), CurrentGen->Config.bGenerateDetailedMesh);
	//AdditionalData += FString::Printf(TEXT("# MaxPolysPerTile\n"));
	//AdditionalData += FString::Printf(TEXT("rd_mppt %d\n"), CurrentGen->Config.MaxPolysPerTile);
	//AdditionalData += FString::Printf(TEXT("# maxVertsPerPoly\n"));
	//AdditionalData += FString::Printf(TEXT("rd_mvpp %d\n"), CurrentGen->Config.maxVertsPerPoly);
	//AdditionalData += FString::Printf(TEXT("# Tile size\n"));
	//AdditionalData += FString::Printf(TEXT("rd_ts %d\n"), CurrentGen->Config.tileSize);

	//AdditionalData += FString::Printf(TEXT("\n"));

	////const FString FilePathName = FileName + FString::Printf(TEXT("_NavDataSet%d_%s.obj"), Index, *CurrentTimeStr);
	////ExportGeomToOBJFile(FilePathName, CoordBuffer, IndexBuffer, AdditionalData);
}

void FExportNavigationModule::PluginButtonClicked()
{
	UWorld* world = _GetWorld();
	const UNavigationSystem* NavSys = UNavigationSystem::GetCurrent(world);
	const FNavigationOctree* NavOctree = NavSys ? NavSys->GetNavOctree() : NULL;

	if (NULL == NavOctree)
	{
		UE_LOG(LogExportNavigation, Error, TEXT("Failed to export navigation data due to %s being NULL"), NavSys == NULL ? TEXT("NavigationSystem") : TEXT("NavOctree"));
	}
	else
	{
		for (int32 Index = 0; Index < NavSys->NavDataSet.Num(); ++Index)
		{
			FString filename = world->GetMapName();
			filename += "_";
			filename.AppendInt(Index);
			const ARecastNavMesh* NavData = Cast<const ARecastNavMesh>(NavSys->NavDataSet[Index]);
			if (NavData)
			{
				_ExportNavMesh(NavData, NavOctree, filename);
			}
			else
			{
				UE_LOG(LogExportNavigation, Error, TEXT("Failed to export navigation data %s"), *filename);
			}
		}
	}

}

void FExportNavigationModule::AddMenuExtension(FMenuBuilder& Builder)
{
	Builder.AddMenuEntry(FExportNavigationCommands::Get().PluginAction);
}

void FExportNavigationModule::AddToolbarExtension(FToolBarBuilder& Builder)
{
	Builder.AddToolBarButton(FExportNavigationCommands::Get().PluginAction);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FExportNavigationModule, ExportNavigation)