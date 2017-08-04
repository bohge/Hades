#pragma once
#include "serialize/ISerializable.h"
#include "RecastnavigationPlugin.h"
#include "vector3array.hpp"


class dtNavMesh;
class dtNavMeshQuery;
namespace hc{ class IFile; }
namespace hrnf
{
	class NavigationData
	{
	public:
		typedef moodycamel::ConcurrentQueue< dtNavMeshQuery* > dtNavMeshQueryList;
	private:
		int						m_Maxnode;
		dtNavMesh*				m_pNavMesh;
		dtNavMeshQueryList		m_QueryList;
	public:
		NavigationData(int maxnode);
		~NavigationData();
	public:
		bool Initialize(int count, hc::IFile* file);		
		dtNavMeshQuery* GetQuery();		
	public:
		HADES_FORCEINLINE dtNavMesh* GetMesh();
		HADES_FORCEINLINE void FreeQuery(dtNavMeshQuery* query);
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE dtNavMesh* NavigationData::GetMesh()
	{
		return m_pNavMesh;
	}
	//---------------------------------------------------------------------------------------------------------
	void NavigationData::FreeQuery(dtNavMeshQuery* query)
	{
		m_QueryList.enqueue(query);
	}
}
