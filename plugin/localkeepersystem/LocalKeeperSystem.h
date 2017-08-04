#pragma once
#include "core/Recycleablevector.hpp"
#include "keeper/IKeeperSystem.h"
#include "concurrentqueue.h"
#include "core/SmartPtr.hpp"
#include "thread/IJob.h"


namespace hlks
{	
	class ObjectHolder;
	typedef hc::Unidelegate1<void, hc::Accessor*> AccessCallback;
	typedef hc::Unidelegate1<void, hc::SmartPtr<hc::IJob>&> RecycleCallback;
	class LocalKeeperSystem : public hc::IKeeperSystem
	{
	private:
		enum Constant
		{
			LKSC_PREPARE_HOLDER_SIZE = 100000,
		};
	private:
		typedef hc::Recycleablevector< ObjectHolder* > ObjectHoldervector;
		typedef moodycamel::ConcurrentQueue< ObjectHolder* > FreeHolderPool;
	private:
		FreeHolderPool*			m_pFreeHolderpool;
		ObjectHoldervector*		m_pObjectHoldervector;
	public:
		LocalKeeperSystem(void);
		virtual ~LocalKeeperSystem(void);
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
		virtual hc::IAccessPackage* AccessPackageFactory();
		virtual void RecycleBin(hc::IAccessPackage* package);
		virtual void RequestAccessor(hc::IAccessPackage* pack);
	public:		
		uint64 CreateAccessor();
		bool DestoryAccessor(uint64 id);
		bool UnrigorousAccessor(uint64 id, int timestamp, AccessCallback::Callback func);//这三个函数的返回值存在多线程bug，暂时不改了，重构这里吧
		bool ReadonlyAccessor(uint64 id, int timestamp, AccessCallback::Callback func);
		bool ReadwriteAccessor(uint64 id, int timestamp, AccessCallback::Callback func);
		void ReleaseAccessor(hc::Accessor* acc);
	};
}

