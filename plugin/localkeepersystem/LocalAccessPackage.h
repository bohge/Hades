#pragma once
#include "keeper/IAccessPackage.h"
#include "LocalKeeperSystem.h"



namespace hc
{
	class IThreadMutex;
}
namespace hlks
{
	class LocalKeeperSystem;
	class LocalAccessPackage : public hc::IAccessPackage
	{
	private:
		hc::AccessinfoArray::iterator	m_SendIterator;
		LocalKeeperSystem*				m_rpLocalKeeperSystem;
		hc::IThreadMutex*				m_pIThreadMutex;
		AccessCallback::Callback		m_AcessCallback;
	public:
		LocalAccessPackage(LocalKeeperSystem* sys);
		virtual ~LocalAccessPackage();
	private:
		void _ExecuteRequest();
	private:
		virtual void _DoRequest();
		virtual void _DoReleaseAccessores();
		virtual void _DoReleaseAccessor(uint64 gid);
	public:
		virtual uint64 CreateAccessor();//只创建，不请求，需要再调用请求
		virtual void DestoryAccessor(uint64 id);
	public:
		void OnAccessorCallback(hc::Accessor* acc);
	};
}
