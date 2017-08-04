#include "serialize/ISerializeSystem.h"
#include "RecastnavigationPlugin.h"
#include "script/IScriptSystem.h"
#include "ExportNavigationHead.h"
#include "thread/IThreadSystem.h"
#include "thread/IThreadRWLock.h"
#include "file/IFileSystem.h"
#include "NavigationQuery.h"
#include "NavigationData.h"
#include "log/ILogSystem.h"
#include "AllRegister.hpp"
#include "file/IFile.h"
#include "vector3.hpp"




#include "DetourNavMeshQuery.h"
#include "DetourNavMesh.h"

using namespace hc;
using namespace eastl;
namespace hrnf
{
	//---------------------------------------------------------------------------------------------------------
	static vector3 EmptyTransfer(const vector3& vec3)
	{
		return vec3;
	}
	//---------------------------------------------------------------------------------------------------------
	static vector3 UnityTransfer(const vector3& vec3)
	{
		return vector3(-vec3.m_x, vec3.m_y, vec3.m_z);
	}
	//---------------------------------------------------------------------------------------------------------
	static vector3 Unreal_Recast(const vector3& vec3)
	{
		return vector3(-vec3.m_x, vec3.m_z, -vec3.m_y);
	}
	//---------------------------------------------------------------------------------------------------------
	static vector3 Recast_Unreal(const vector3& vec3)
	{
		return vector3(-vec3.m_x, -vec3.m_z, vec3.m_y);
	}

	RTTI_TYPE_IMPLEMENT(Recastnavigation);
	SYSTEM_SINGLETON_IMPLEMENT(Recastnavigation);
	//---------------------------------------------------------------------------------------------------------
	Recastnavigation::Recastnavigation()
		:m_polyPickExt(NULL)
		, m_pFilter(NULL)
		, m_pIThreadRWLock(NULL)
		, m_pToRecast(&EmptyTransfer)
		, m_pFromRecast(&EmptyTransfer)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	Recastnavigation::~Recastnavigation()
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//---------------------------------------------------------------------------------------------------------
	void Recastnavigation::Initialize()
	{
		ASSERT(128 == sizeof(SerializData));
		m_pIThreadRWLock =
			static_cast<IThreadRWLock*>(IThreadSystem::Instance()
			->ThreadEntityFactory(ThreadProperty::ET_RWLOCK));
		m_polyPickExt = NEW vector3(2, 4, 2);
		m_pFilter = NEW dtQueryFilter();
		m_pFilter->setIncludeFlags(HADES_POLYFLAGS_ALL ^ HADES_POLYFLAGS_DISABLED);
		m_pFilter->setExcludeFlags(0);
		m_pFilter->setAreaCost(HADES_POLYAREA_GROUND, 1.0f);
		m_pFilter->setAreaCost(HADES_POLYAREA_WATER, 10.0f);
		m_pFilter->setAreaCost(HADES_POLYAREA_ROAD, 1.0f);
		m_pFilter->setAreaCost(HADES_POLYAREA_DOOR, 1.0f);
		m_pFilter->setAreaCost(HADES_POLYAREA_GRASS, 2.0f);
		m_pFilter->setAreaCost(HADES_POLYAREA_JUMP, 1.5f);

		{
			hc::IScriptSystem* sys = hc::IScriptSystem::Instance();
			Register_All("Recastnavigation", *sys);
		}
		{
			hc::ISerializeSystem* sys = hc::ISerializeSystem::Instance();
			sys->RegisterSerializable<NavigationQuery>();
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void Recastnavigation::Update()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void Recastnavigation::Exit()
	{
		IThreadSystem::Instance()->RecycleBin(m_pIThreadRWLock);
		SAFE_DELETE(m_polyPickExt);
		SAFE_DELETE(m_pFilter);
	}
	//---------------------------------------------------------------------------------------------------------
	void Recastnavigation::SetCoordinateSystem(Coordinate cn)
	{
		switch (cn)
		{
		case hrnf::Recastnavigation::RN_COORDINATE_ORIGINAL:
			m_pToRecast = &EmptyTransfer;
			m_pFromRecast = &EmptyTransfer;
			break;
		case hrnf::Recastnavigation::RN_COORDINATE_UNITY:
			m_pToRecast = &UnityTransfer;
			m_pFromRecast = &UnityTransfer;
			break;
		case hrnf::Recastnavigation::RN_COORDINATE_UNREAL:
			m_pToRecast = &Unreal_Recast;
			m_pFromRecast = &Recast_Unreal;
			break;
		default:ASSERT(false);
			break;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void Recastnavigation::SetPickExtent(const vector3& ext)
	{
		*m_polyPickExt = ToRecastCoordinate(ext);
		m_polyPickExt->m_x = abs(m_polyPickExt->m_x);
		m_polyPickExt->m_y = abs(m_polyPickExt->m_y);
		m_polyPickExt->m_z = abs(m_polyPickExt->m_z);
	}
	//---------------------------------------------------------------------------------------------------------
	NavigationQuery* Recastnavigation::LoadNavigationMesh(int maxnode, const eastl::string& path)
	{
		NavigationQuery* res = NULL;
		if (path.size() >= RECAST_MAX_FILE_LENGTH)
		{
			HADESERROR("Recastnavigation's file name is out of range. Max length is %d, now is %d\r\n", RECAST_MAX_FILE_LENGTH - 1, path.size());
		}
		else
		{
			uint hash = Utility::HashCode(path);
			if (NULL != GetNavigation(maxnode, hash, path))
			{
				res = static_cast<NavigationQuery*>(ISerializeSystem::Instance()
					->GetSerializable(NavigationQuery::RTTI().GetFactor()));
				res->Initialize(maxnode, hash, path);
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	NavigationData* Recastnavigation::GetNavigation(int maxnode, uint hash, const eastl::string& path)
	{
		NavigationData* res = NULL;
		m_pIThreadRWLock->ReadLock();
		NavMeshMap::iterator it = m_NavMeshMap.find(hash);
		m_pIThreadRWLock->Unlock();
		if (m_NavMeshMap.end() != it)
		{
			res = it->second;
		}
		else
		{
			IFile* file = IFileSystem::Instance()->FileFactory(path);
			res = NEW NavigationData(maxnode);
			if (!res->Initialize(maxnode, file))
			{
				SAFE_DELETE(res);
			}
			else
			{
				m_pIThreadRWLock->WriteLock();
				m_NavMeshMap.insert(make_pair(hash, res));
				m_pIThreadRWLock->Unlock();
			}
			file->CloseFile();
			IFileSystem::Instance()->RecycleBin(file);
		}
		return res;
	}
}
