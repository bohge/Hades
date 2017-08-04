// Copyright 1998-2016 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "ModuleManager.h"
typedef unsigned long long dtPolyRef;
#include "AI/Navigation/NavigationTypes.h"
#include "AI/Navigation/RecastNavMesh.h"
#include "AI/NavigationModifier.h"
#include "AI/NavigationOctree.h"
#include "AI/Navigation/RecastNavMeshGenerator.h"
#include "AI/Navigation/RecastNavMesh.h"
#include "AI/Navigation/RecastHelpers.h"

#include "Recast.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "InputGeom.h"

class FToolBarBuilder;
class FMenuBuilder;
class UWorld;
class ARecastNavMesh;
class FNavigationOctree;


DECLARE_LOG_CATEGORY_EXTERN(LogExportNavigation, Log, All);
class FExportNavigationModule : public IModuleInterface
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
	/** This function will be bound to Command. */
	void PluginButtonClicked();
	
private:
	unsigned char* _BuildTileMesh(const original::InputGeom* geom, const int tx, const int ty, const float* bmin, const float* bmax, int& dataSize, const FRecastBuildConfig& config);
	bool _BuildAllTile(original::dtNavMesh* navMesh, const original::InputGeom* geom, const FRecastBuildConfig& config);
	void _MeshToNavMesh(const original::InputGeom* geom, const FRecastBuildConfig& config, const FString& FileName);
	void _ExportNavMesh(const ARecastNavMesh* navmesh, const FNavigationOctree* octree, const FString& FileName);
	void _Saveall(const FString& path, const original::dtNavMesh* mesh);

	void AddToolbarExtension(FToolBarBuilder& Builder);
	void AddMenuExtension(FMenuBuilder& Builder);

private:
	original::rcContext* m_ctx;
	original::rcHeightfield* m_solid;
	original::rcCompactHeightfield* m_chf;
	original::rcContourSet* m_cset;
	original::rcPolyMesh* m_pmesh;
	original::rcPolyMeshDetail* m_dmesh;
	TSharedPtr<class FUICommandList> PluginCommands;
};