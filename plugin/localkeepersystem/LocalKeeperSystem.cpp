#include "LocalAccessPackage.h"
#include "LocalKeeperSystem.h"
#include "keeper/Accessor.h"
#include "ObjectHolder.h"
#include "Accessorpool.h"
#include "HolderThread.h"
#include "log/ILogSystem.h"
#include "NoneJob.h"


using namespace hc;
using namespace eastl;

namespace hlks
{
	//---------------------------------------------------------------------------------------------------------
	LocalKeeperSystem::LocalKeeperSystem(void)
		:m_pFreeHolderpool(NULL)
		, m_pObjectHoldervector(NULL)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	LocalKeeperSystem::~LocalKeeperSystem(void)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void LocalKeeperSystem::Initialize()
	{
		IKeeperSystem::Initialize();
		HolderThread::Create();
		Accessorpool::Create();
		m_pFreeHolderpool = NEW FreeHolderPool;
		m_pObjectHoldervector = NEW ObjectHoldervector;
		m_pObjectHoldervector->Initialize(LKSC_PREPARE_HOLDER_SIZE);
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalKeeperSystem::Exit()
	{
		ObjectHolder* holder;
		while (m_pFreeHolderpool->try_dequeue(holder))
		{
			holder->Exit();
			SAFE_DELETE(holder);
		}
		for (uint i = 0; i < m_pObjectHoldervector->Size(); ++i)
		{
			if (m_pObjectHoldervector->UncheckHave(i))
			{
				holder = m_pObjectHoldervector->Get(i);
				holder->Exit();
				SAFE_DELETE(holder);
			}
		}		
		SAFE_DELETE(m_pFreeHolderpool);
		SAFE_DELETE(m_pObjectHoldervector);
		HolderThread::Destroy();
		Accessorpool::Destroy();
		IKeeperSystem::Exit();
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 LocalKeeperSystem::CreateAccessor()
	{
		ObjectHolder* holder;
		if (!m_pFreeHolderpool->try_dequeue(holder))
		{
			holder = NEW ObjectHolder;
		}
		uint64 key = m_pObjectHoldervector->Push(holder);
		holder->Initialize(key);
		return key;
	}
	//---------------------------------------------------------------------------------------------------------
	bool LocalKeeperSystem::DestoryAccessor(uint64 id)
	{
		ASSERT(false);//多线程不安全，当别的线程已经读取了该数据
		if (m_pObjectHoldervector->Have(id))
		{
			ObjectHolder* holder = m_pObjectHoldervector->Get(id);
			m_pObjectHoldervector->Remove(id);
			holder->Exit();
			m_pFreeHolderpool->enqueue(holder);
		}
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	bool LocalKeeperSystem::UnrigorousAccessor(uint64 id, int timestamp, AccessCallback::Callback func)
	{
		bool res;
		if (m_pObjectHoldervector->Have(id))
		{
			ObjectHolder* holder = m_pObjectHoldervector->Get(id);
			res = holder->UnrigorousAccessor(timestamp, func);
		}
		else
		{
			hc::SmartPtr<NoneJob> job = hc::SmartPtr<NoneJob>(NEW NoneJob);
			job->SetCallback(func);
			HolderThread::Instance()->PushJob(job);
			res = true;
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	bool LocalKeeperSystem::ReadonlyAccessor(uint64 id, int timestamp, AccessCallback::Callback func)
	{
		bool res;
		if (m_pObjectHoldervector->Have(id))
		{
			ObjectHolder* holder = m_pObjectHoldervector->Get(id);
			res = holder->ReadonlyAccessor(timestamp, func);
		}
		else
		{
			hc::SmartPtr<NoneJob> job = hc::SmartPtr<NoneJob>(NEW NoneJob);
			job->SetCallback(func);
			HolderThread::Instance()->PushJob(job);
			res = true;
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	bool LocalKeeperSystem::ReadwriteAccessor(uint64 id, int timestamp, AccessCallback::Callback func)
	{
		bool res;
		if (m_pObjectHoldervector->Have(id))
		{
			ObjectHolder* holder = m_pObjectHoldervector->Get(id);
			res = holder->ReadwriteAccessor(timestamp, func);
		}
		else
		{
			hc::SmartPtr<NoneJob> job = hc::SmartPtr<NoneJob>(NEW NoneJob);
			job->SetCallback(func);
			HolderThread::Instance()->PushJob(job);
			res = true;
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalKeeperSystem::ReleaseAccessor(hc::Accessor* acc)
	{
		if (NULL != acc)
		{
			uint64 id = acc->GetID();
			if (m_pObjectHoldervector->Have(id))
			{
				ObjectHolder* holder = m_pObjectHoldervector->Get(id);
				holder->ReleaseAccessor(acc);
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LocalKeeperSystem::Update()
	{
		//可能会导致崩溃
		typedef eastl::map<uint, uint64, eastl::greater<uint>> localmap;
		localmap stack;
		for (int i = 0; i < m_pObjectHoldervector->Size(); ++i)
		{
			if (m_pObjectHoldervector->UncheckHave(i))
			{
				ObjectHolder* holder = m_pObjectHoldervector->Get(i);
				uint count = holder->Count();
				if (count > 0)
				{
					stack.insert(eastl::make_pair(count, holder->GetMainAccessor()->GetID()));
				}
			}
		}

		int i = 0;
		if (!stack.empty())
		{
			HADESLOG("---------------------------------------------------------------\r\n");
			FOR_EACH(localmap, it, stack)
			{
				if (++i > 3)
				{
					break;
				}
				HADESLOG("In stack memory access %d, %llu\r\n", it->first, it->second);
			}
			HADESLOG("---------------------------------------------------------------\r\n\r\n\r\n\r\n");
		}
	}
	//---------------------------------------------------------------------------------------------------------
	IAccessPackage* LocalKeeperSystem::AccessPackageFactory()
	{
		return NEW LocalAccessPackage(this);
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalKeeperSystem::RecycleBin(IAccessPackage* package)
	{
		SAFE_DELETE(package);
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalKeeperSystem::RequestAccessor(IAccessPackage* pack)
	{
		pack->BeginRequest();
	}
}
