#pragma once
#include "serialize/ISerializable.h"
#include "RecastnavigationPlugin.h"
#include "vector3array.hpp"
#include "SerializData.h"

class dtNavMesh;
class dtNavMeshQuery;
namespace hrnf
{
	class NavigationData;
	class NavigationQuery : public hc::ISerializable
	{
		SERIALIZABL_TYPE_DECLEAR(NavigationQuery);
		RTTI_DRIVER_TYPE_DECLEAR(NavigationQuery, hc::ISerializable);
	private:
		enum Constant
		{
			NC_MAX_FIND_NODE = 512,
		};
	private:
		vector3array		m_PointArray;
		NavigationData*		m_rpNavigationData;
		SerializData		m_NavmeshData;
	public:
		NavigationQuery();
		~NavigationQuery();
	public:
		virtual const byte* GetBuffer() const;
		virtual uint GetLength() const;
		virtual bool FromBuffer(const byte* buff, uint len);
	public:
		const vector3array& FindPath(int maxnode, float step, const vector3& begin, const vector3& end);
		vector3 FindClosestPoint(const vector3& point);
		const vector3array& FindRandomPointAroundCircle(int points, float radius, const vector3& cen);
	public:
		HADES_FORCEINLINE void Initialize(int maxnode, uint hash, const eastl::string& file);
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void NavigationQuery::Initialize(int maxnode, uint hash, const eastl::string& file)
	{
		m_NavmeshData.Maxnode = maxnode;
		m_NavmeshData.Hash = hash;
		memcpy(m_NavmeshData.FileName, file.c_str(), file.size() + 1);
		m_rpNavigationData = Recastnavigation::Instance()->GetNavigation(maxnode, hash, file);
	}
}
