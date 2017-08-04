#pragma once
#include "core/Singleton.hpp"
#include "core/IFunction.h"
#include "vector3.hpp"


#include "concurrentqueue.h"



//这个功能插件没有办法进行跨进程使用
//如果需要扩展可选用一个随机id的方式+保存文件名的方式在进程之间使用
class dtQueryFilter;
namespace hc
{
	class IThreadRWLock;
}
namespace hrnf
{
	class vector3;
	class NavigationData;
	class NavigationQuery;
	class Recastnavigation : public hc::IFunction
	{
		SYSTEM_SINGLETON_DECLEAR(Recastnavigation);
		RTTI_DRIVER_TYPE_DECLEAR(Recastnavigation, hc::IFunction);
	public:
		enum Coordinate
		{
			RN_COORDINATE_ORIGINAL = 0,
			RN_COORDINATE_UNITY,
			RN_COORDINATE_UNREAL,
		};
	private:
		typedef vector3(*Transfer)(const vector3& vec3);
		typedef eastl::map<uint, NavigationData*> NavMeshMap;
	private:
		NavMeshMap			m_NavMeshMap;
		hc::IThreadRWLock*	m_pIThreadRWLock;
		dtQueryFilter*		m_pFilter;
		vector3*			m_polyPickExt;
		Transfer			m_pToRecast;
		Transfer			m_pFromRecast;
	public:
		Recastnavigation();
		~Recastnavigation();
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	public:
		void SetCoordinateSystem(Coordinate cn);
		void SetPickExtent(const vector3& ext);
		NavigationQuery* LoadNavigationMesh(int maxnode, const eastl::string& path);
		NavigationData* GetNavigation(int maxnode, uint hash, const eastl::string& path);
	public:		
		HADES_FORCEINLINE const dtQueryFilter* GetQueryFilter();
		HADES_FORCEINLINE const vector3* GetExtents();
		HADES_FORCEINLINE vector3 ToRecastCoordinate(const vector3& vec3);
		HADES_FORCEINLINE vector3 FromRecastCoordinate(const vector3& vec3);
	};



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const dtQueryFilter* Recastnavigation::GetQueryFilter()
	{
		return m_pFilter;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const vector3* Recastnavigation::GetExtents()
	{
		return m_polyPickExt;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE vector3 Recastnavigation::ToRecastCoordinate(const vector3& vec3)
	{
		return m_pToRecast(vec3);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE vector3 Recastnavigation::FromRecastCoordinate(const vector3& vec3)
	{
		return m_pFromRecast(vec3);
	}
}