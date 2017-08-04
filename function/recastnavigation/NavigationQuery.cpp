#include "serialize/FactorConfigure.h"
#include "NavigationQuery.h"
#include "NavigationData.h"

#include "DetourNavMeshQuery.h"
#include "DetourNavMesh.h"
#include "DetourCommon.h"
#include "log/ILogSystem.h"


#include <limits>
namespace hrnf
{
	RTTI_TYPE_IMPLEMENT_WITH_FACTOR(vector3, FACTOR_VECTOR3F);
	RTTI_TYPE_IMPLEMENT_WITH_FACTOR(NavigationQuery, FACTOR_NAVMESH);
	//---------------------------------------------------------------------------------------------------------
	NavigationQuery::NavigationQuery()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	NavigationQuery::~NavigationQuery()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	const byte* NavigationQuery::GetBuffer() const
	{
		return (const byte*)&m_NavmeshData;
	}
	//---------------------------------------------------------------------------------------------------------
	uint NavigationQuery::GetLength() const
	{
		return sizeof(m_NavmeshData);
	}
	//---------------------------------------------------------------------------------------------------------
	bool NavigationQuery::FromBuffer(const byte* buff, uint len)
	{
		SerializData data = *(SerializData*)buff;
		Initialize(data.Maxnode, data.Hash, data.FileName);
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	inline bool inRange(const float* v1, const float* v2, const float r, const float h)
	{
		const float dx = v2[0] - v1[0];
		const float dy = v2[1] - v1[1];
		const float dz = v2[2] - v1[2];
		return (dx*dx + dz*dz) < r*r && fabsf(dy) < h;
	}
	//---------------------------------------------------------------------------------------------------------	
	static int fixupCorridor(dtPolyRef* path, const int npath, const int maxPath,
		const dtPolyRef* visited, const int nvisited)
	{
		int furthestPath = -1;
		int furthestVisited = -1;

		// Find furthest common polygon.
		for (int i = npath - 1; i >= 0; --i)
		{
			bool found = false;
			for (int j = nvisited - 1; j >= 0; --j)
			{
				if (path[i] == visited[j])
				{
					furthestPath = i;
					furthestVisited = j;
					found = true;
				}
			}
			if (found)
				break;
		}

		// If no intersection found just return current path. 
		if (furthestPath == -1 || furthestVisited == -1)
			return npath;

		// Concatenate paths.	

		// Adjust beginning of the buffer to include the visited.
		const int req = nvisited - furthestVisited;
		const int orig = eastl::min(furthestPath + 1, npath);
		int size = eastl::max(0, npath - orig);
		if (req + size > maxPath)
			size = maxPath - req;
		if (size)
			memmove(path + req, path + orig, size*sizeof(dtPolyRef));

		// Store visited
		for (int i = 0; i < req; ++i)
			path[i] = visited[(nvisited - 1) - i];

		return req + size;
	}
	//---------------------------------------------------------------------------------------------------------
	static bool getSteerTarget(dtNavMeshQuery* navQuery, const float* startPos, const float* endPos,
		const float minTargetDist,
		const dtPolyRef* path, const int pathSize,
		float* steerPos, unsigned char& steerPosFlag, dtPolyRef& steerPosRef,
		float* outPoints = 0, int* outPointCount = 0)
	{
		// Find steer target.
		static const int MAX_STEER_POINTS = 3;
		float steerPath[MAX_STEER_POINTS * 3];
		unsigned char steerPathFlags[MAX_STEER_POINTS];
		dtPolyRef steerPathPolys[MAX_STEER_POINTS];
		int nsteerPath = 0;
		navQuery->findStraightPath(startPos, endPos, path, pathSize,
			steerPath, steerPathFlags, steerPathPolys, &nsteerPath, MAX_STEER_POINTS);
		if (!nsteerPath)
			return false;

		if (outPoints && outPointCount)
		{
			*outPointCount = nsteerPath;
			for (int i = 0; i < nsteerPath; ++i)
				dtVcopy(&outPoints[i * 3], &steerPath[i * 3]);
		}


		// Find vertex far enough to steer to.
		int ns = 0;
		while (ns < nsteerPath)
		{
			// Stop at Off-Mesh link or when point is further than slop away.
			if ((steerPathFlags[ns] & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ||
				!inRange(&steerPath[ns * 3], startPos, minTargetDist, 1000.0f))
				break;
			ns++;
		}
		// Failed to find good point to steer to.
		if (ns >= nsteerPath)
			return false;

		dtVcopy(steerPos, &steerPath[ns * 3]);
		steerPos[1] = startPos[1];
		steerPosFlag = steerPathFlags[ns];
		steerPosRef = steerPathPolys[ns];

		return true;
	}
	// This function checks if the path has a small U-turn, that is,
	// a polygon further in the path is adjacent to the first polygon
	// in the path. If that happens, a shortcut is taken.
	// This can happen if the target (T) location is at tile boundary,
	// and we're (S) approaching it parallel to the tile edge.
	// The choice at the vertex can be arbitrary, 
	//  +---+---+
	//  |:::|:::|
	//  +-S-+-T-+
	//  |:::|   | <-- the step can end up in here, resulting U-turn path.
	//  +---+---+
	static int fixupShortcuts(dtPolyRef* path, int npath, dtNavMeshQuery* navQuery)
	{
		if (npath < 3)
			return npath;

		// Get connected polygons
		static const int maxNeis = 16;
		dtPolyRef neis[maxNeis];
		int nneis = 0;

		const dtMeshTile* tile = 0;
		const dtPoly* poly = 0;
		if (dtStatusFailed(navQuery->getAttachedNavMesh()->getTileAndPolyByRef(path[0], &tile, &poly)))
			return npath;

		for (unsigned int k = poly->firstLink; k != DT_NULL_LINK; k = tile->links[k].next)
		{
			const dtLink* link = &tile->links[k];
			if (link->ref != 0)
			{
				if (nneis < maxNeis)
					neis[nneis++] = link->ref;
			}
		}

		// If any of the neighbour polygons is within the next few polygons
		// in the path, short cut to that polygon directly.
		static const int maxLookAhead = 6;
		int cut = 0;
		for (int i = dtMin(maxLookAhead, npath) - 1; i > 1 && cut == 0; i--) {
			for (int j = 0; j < nneis; j++)
			{
				if (path[i] == neis[j]) {
					cut = i;
					break;
				}
			}
		}
		if (cut > 1)
		{
			int offset = cut - 1;
			npath -= offset;
			for (int i = 1; i < npath; i++)
				path[i] = path[i + offset];
		}

		return npath;
	}
	//---------------------------------------------------------------------------------------------------------
	const vector3array& NavigationQuery::FindPath(int maxnode, float step, const vector3& begin, const vector3& end)
	{
		m_PointArray.Clear();
		vector3 realend = Recastnavigation::Instance()->ToRecastCoordinate(end);
		vector3 realbegin = Recastnavigation::Instance()->ToRecastCoordinate(begin);
		dtNavMeshQuery*query = m_rpNavigationData->GetQuery();
		int polycount(0);
		dtPolyRef beginRef, endRef;
		dtPolyRef polys[NC_MAX_FIND_NODE];
		const vector3* ext = Recastnavigation::Instance()->GetExtents();
		const dtQueryFilter* filter = Recastnavigation::Instance()->GetQueryFilter();

		query->findNearestPoly(realbegin.m_Data, ext->m_Data, filter, &beginRef, 0);
		query->findNearestPoly(realend.m_Data, ext->m_Data, filter, &endRef, 0);

		query->findPath(beginRef, endRef, realbegin.m_Data, realend.m_Data, filter, polys, &polycount, NC_MAX_FIND_NODE);

		if (polycount)
		{
			vector3 iterPos, targetPos, testiterPos;
			query->closestPointOnPoly(beginRef, realbegin.m_Data, iterPos.m_Data, 0);
			query->closestPointOnPoly(polys[polycount - 1], realend.m_Data, targetPos.m_Data, 0);

			const float SLOP = step * 0.2f;

			m_PointArray.PushBack(Recastnavigation::Instance()->FromRecastCoordinate(iterPos));

			// Move towards target a small advancement at a time until target reached or
			// when ran out of memory to store the path.
			while (polycount && m_PointArray.Size() < maxnode)
			{
				// Find location to steer towards.
				vector3 steerPos;
				unsigned char steerPosFlag;
				dtPolyRef steerPosRef;

				if (!getSteerTarget(query, iterPos.m_Data, targetPos.m_Data, SLOP,
					polys, polycount, steerPos.m_Data, steerPosFlag, steerPosRef))
					break;

				bool endOfPath = (steerPosFlag & DT_STRAIGHTPATH_END) ? true : false;
				bool offMeshConnection = (steerPosFlag & DT_STRAIGHTPATH_OFFMESH_CONNECTION) ? true : false;

				// Find movement delta.
				vector3 delta;
				float len;
				delta = steerPos - iterPos;
				len = dtMathSqrtf(dtVdot(delta.m_Data, delta.m_Data));
				// If the steer target is end of path or off-mesh link, do not move past the location.
				if ((endOfPath || offMeshConnection) && len < step)
					len = 1;
				else
					len = step / len;
				float moveTgt[3];
				dtVmad(moveTgt, iterPos.m_Data, delta.m_Data, len);

				// Move
				float result[3];
				dtPolyRef visited[16];
				int nvisited = 0;
				query->moveAlongSurface(polys[0], iterPos.m_Data, moveTgt, filter,
					result, visited, &nvisited, 16);

				polycount = fixupCorridor(polys, polycount, NC_MAX_FIND_NODE, visited, nvisited);
				polycount = fixupShortcuts(polys, polycount, query);

				float h = 0;
				query->getPolyHeight(polys[0], result, &h);
				result[1] = h;
				dtVcopy(iterPos.m_Data, result);

				// Handle end of path and off-mesh links when close enough.
				if (endOfPath && inRange(iterPos.m_Data, steerPos.m_Data, SLOP, 1.0f))
				{
					// Reached end of path.
					dtVcopy(iterPos.m_Data, targetPos.m_Data);
					if (m_PointArray.Size() < maxnode)
					{
						m_PointArray.PushBack(Recastnavigation::Instance()->FromRecastCoordinate(iterPos));
					}
					break;
				}
				else if (offMeshConnection && inRange(iterPos.m_Data, steerPos.m_Data, SLOP, 1.0f))
				{
					// Reached off-mesh connection.
					vector3 startPos, endPos;

					// Advance the path up to and over the off-mesh connection.
					dtPolyRef prevRef = 0, polyRef = polys[0];
					int npos = 0;
					while (npos < polycount && polyRef != steerPosRef)
					{
						prevRef = polyRef;
						polyRef = polys[npos];
						npos++;
					}
					for (int i = npos; i < polycount; ++i)
						polys[i - npos] = polys[i];
					polycount -= npos;

					// Handle the connection.
					dtNavMesh* mesh = m_rpNavigationData->GetMesh();
					dtStatus status = mesh->getOffMeshConnectionPolyEndPoints(prevRef, polyRef, startPos.m_Data, endPos.m_Data);
					if (dtStatusSucceed(status))
					{
						if (m_PointArray.Size() < maxnode)
						{
							m_PointArray.PushBack(Recastnavigation::Instance()->FromRecastCoordinate(startPos));
							// Hack to make the dotted path not visible during off-mesh connection.
							if (m_PointArray.Size() & 1)
							{
								m_PointArray.PushBack(Recastnavigation::Instance()->FromRecastCoordinate(startPos));
							}
						}
						// Move position at the other side of the off-mesh link.
						dtVcopy(iterPos.m_Data, endPos.m_Data);
						float eh = 0.0f;
						query->getPolyHeight(polys[0], iterPos.m_Data, &eh);
						iterPos.m_x = eh;
					}
				}
				// Store results.
				if (m_PointArray.Size() < maxnode)
				{
					m_PointArray.PushBack(Recastnavigation::Instance()->FromRecastCoordinate(iterPos));
				}
			}
		}

		m_rpNavigationData->FreeQuery(query);
		return m_PointArray;
	}
	//---------------------------------------------------------------------------------------------------------
	hrnf::vector3 NavigationQuery::FindClosestPoint(const vector3& point)
	{
		float dis = std::numeric_limits<float>::max();
		vector3 realpoint = Recastnavigation::Instance()->ToRecastCoordinate(point);
		vector3 res;
		int polycount(0);
		vector3 ext(2, dis, 2);
		dtPolyRef polys[NC_MAX_FIND_NODE];
		dtNavMeshQuery*query = m_rpNavigationData->GetQuery();
		const dtQueryFilter* filter = Recastnavigation::Instance()->GetQueryFilter();
		query->queryPolygons(realpoint.m_Data, ext.m_Data, filter, polys, &polycount, NC_MAX_FIND_NODE);
		for (int i = 0; i < polycount; ++i)
		{
			bool isover;
			vector3 localres;
			dtStatus status = query->closestPointOnPoly(polys[i], realpoint.m_Data, localres.m_Data, &isover);
			if (dtStatusSucceed(status))
			{
				float localdis = (localres - realpoint).LengthPow();
				if (dis > localdis)
				{
					dis = localdis;
					res = localres;
				}
			}
		}
		m_rpNavigationData->FreeQuery(query);
		return Recastnavigation::Instance()->FromRecastCoordinate(res);
	}
	//---------------------------------------------------------------------------------------------------------
	static float frand()
	{
		return (float)rand() / (float)RAND_MAX;
	}
	const vector3array& NavigationQuery::FindRandomPointAroundCircle(int points, float radius, const vector3& cen)
	{
		m_PointArray.Clear();
		dtPolyRef beginRef;
		vector3 realpoint = Recastnavigation::Instance()->ToRecastCoordinate(cen);
		dtNavMeshQuery*query = m_rpNavigationData->GetQuery();
		const vector3* ext = Recastnavigation::Instance()->GetExtents();
		const dtQueryFilter* filter = Recastnavigation::Instance()->GetQueryFilter();
		query->findNearestPoly(realpoint.m_Data, ext->m_Data, filter, &beginRef, 0);
		for (int i = 0; i < points; i++)
		{
			dtPolyRef ref;
			vector3 res;
			dtStatus status = query->findRandomPointAroundCircle(beginRef, realpoint.m_Data, radius, filter, frand, &ref, res.m_Data);
			if (dtStatusSucceed(status))
			{
				m_PointArray.PushBack(Recastnavigation::Instance()->FromRecastCoordinate(res));
			}
		}
		m_rpNavigationData->FreeQuery(query);
		return m_PointArray;
	}
}