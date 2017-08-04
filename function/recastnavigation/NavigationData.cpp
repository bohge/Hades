#include "ExportNavigationHead.h"
#include "NavigationData.h"
#include "file/IFile.h"


#include "DetourNavMeshQuery.h"
#include "DetourNavMesh.h"

using namespace hc;
namespace hrnf
{
	//---------------------------------------------------------------------------------------------------------
	NavigationData::NavigationData(int maxnode)
		:m_Maxnode(maxnode)
		, m_pNavMesh(NULL)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	NavigationData::~NavigationData()		
	{
		dtFreeNavMesh(m_pNavMesh);

		dtNavMeshQuery* query;
		while (m_QueryList.try_dequeue(query))
		{
			dtFreeNavMeshQuery(query);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	bool NavigationData::Initialize(int count, IFile* file)
	{
		NavMeshSetHeader header;
		file->ReadFile(&header, sizeof(NavMeshSetHeader));
		if (header.magic == NAVMESHSET_MAGIC
			&& header.version == NAVMESHSET_VERSION)
		{
			m_pNavMesh = dtAllocNavMesh();
			dtStatus status = m_pNavMesh->init(&header.params);
			if (!dtStatusFailed(status))
			{
				for (int i = 0; i < header.numTiles; ++i)
				{
					NavMeshTileHeader tileHeader;
					file->ReadFile(&tileHeader, sizeof(NavMeshTileHeader));
					ASSERT(tileHeader.tileRef && tileHeader.dataSize);
					unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
					if (!data)
					{
						return false;
					}
					memset(data, 0, tileHeader.dataSize);
					file->ReadFile(data, tileHeader.dataSize);
					m_pNavMesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
				}
				for (int i = 0; i < count; ++i)
				{
					dtNavMeshQuery* query = dtAllocNavMeshQuery();
					m_QueryList.enqueue(query);
					dtStatus status = query->init(m_pNavMesh, m_Maxnode);
					if (dtStatusFailed(status))
					{
						return false;
					}
				}
			}
		}
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	dtNavMeshQuery* NavigationData::GetQuery()
	{
		dtNavMeshQuery* query;
		if (!m_QueryList.try_dequeue(query))
		{
			query = dtAllocNavMeshQuery();
			query->init(m_pNavMesh, m_Maxnode);
		}
		return query;
	}
}