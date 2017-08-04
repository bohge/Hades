#include "keeper/IAccessPackage.h"


#include <EASTL/sort.h>

using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT(IAccessPackage);
	//---------------------------------------------------------------------------------------------------------
	IAccessPackage::IAccessPackage()
		:m_haveRequest(false)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IAccessPackage::~IAccessPackage()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	bool IAccessPackage::UnrigorousAccessor(uint64 id, int timestamp)
	{
		if (nullid != id
			&& !_isRquested(id))
		{
			m_haveRequest = true;
			m_AccessOrder.insert(make_pair(id, Accessinfo(id, timestamp, Accessor::AM_UNRIGOROUS)));
			return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	bool IAccessPackage::ReadonlyAccessor(uint64 id, int timestamp)
	{
		if (nullid != id
			&& !_isRquested(id))
		{
			m_haveRequest = true;
			m_AccessOrder.insert(make_pair(id, Accessinfo(id, timestamp, Accessor::AM_READ_ONLY)));
			return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	bool IAccessPackage::ReadwriteAccessor(uint64 id, int timestamp)
	{
		if (nullid != id
			&& !_isRquested(id))
		{
			m_haveRequest = true;
			m_AccessOrder.insert(make_pair(id, Accessinfo(id, timestamp, Accessor::AM_READ_WRITE)));
			return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	void IAccessPackage::MayChangeAccesorMode(uint64 id, Accessor::AccessMode am)
	{
		if (nullid != id)
		{
			AccessinfoArray::iterator it = m_AccessOrder.find(id);
			ASSERT(m_AccessOrder.end() != it);
			if (Accessor::isHigh(am, it->second.eAccessMode))
			{
				it->second.eAccessMode = am;
				m_haveRequest = true;
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void IAccessPackage::BeginRequest()
	{
		if (m_haveRequest)
		{
			_DoRequest();
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void IAccessPackage::ReleaseAccessores()
	{
		_DoReleaseAccessores();
		m_AccessOrder.clear();
	}
	//---------------------------------------------------------------------------------------------------------
	void IAccessPackage::ReleaseAccessor(uint64 gid)
	{
		if (nullid != gid)
		{
			ASSERT(m_AccessOrder.end() != m_AccessOrder.find(gid));
			_DoReleaseAccessor(gid);
			m_AccessOrder.erase(gid);
		}
	}
}