#include "core/ThreadSafeOprator.h"
#include "thread/IThreadSystem.h"
#include "thread/IThreadMutex.h"
#include "ObjectHolder.h"
#include "HolderThread.h"
#include "Accessorpool.h"
#include "SaveJob.h"
#include "log/ILogSystem.h"



using namespace hc;
using namespace eastl;
namespace hlks
{
	RTTI_TYPE_IMPLEMENT(ObjectHolder);
	//---------------------------------------------------------------------------------------------------------
	ObjectHolder::ObjectHolder()
		:m_rpMainAccessor(NULL)
		, m_eHolderState(HS_NON_ACESS)
		, m_SaveCallback(NULL)
		, m_ReadReferenceCount(0)
		, m_Timestamp(0)
	{
		m_SaveCallback = MEMBER_FUNC_PTR(&ObjectHolder::OnSaved);
		m_pIThreadMutex = static_cast<IThreadMutex*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_MUTEX));
	}
	//---------------------------------------------------------------------------------------------------------
	ObjectHolder::~ObjectHolder()
	{
		IThreadSystem::Instance()->RecycleBin(m_pIThreadMutex);
		ASSERT(NULL == m_rpMainAccessor);
		SAFE_DELETE(m_SaveCallback);
	}
	//---------------------------------------------------------------------------------------------------------
	bool ObjectHolder::_TryWriteAcess()
	{
		bool res;
		//m_pIThreadMutex->Lock();
		res = HS_NON_ACESS == m_eHolderState;
		if (res)
		{
			m_eHolderState = HS_READ_WRITE;
		}
		//m_pIThreadMutex->Unlock();
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	bool ObjectHolder::_TryReadAcess()
	{
		bool res = true;
		//m_pIThreadMutex->Lock();
		if (!m_AccessList.empty())
		{
			res = m_AccessList.front().Mode != Accessor::AM_READ_WRITE;
		}
		res &= HS_READ_ONLY == m_eHolderState
			|| HS_NON_ACESS == m_eHolderState;
		if (res)
		{
			//HADESLOG("read gid %llu, count %d\r\n", m_rpMainAccessor->GetID(), m_ReadReferenceCount + 1);
			++m_ReadReferenceCount;
			m_eHolderState = HS_READ_ONLY;
		}
		//m_pIThreadMutex->Unlock();
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	bool ObjectHolder::_TryUnrigorousAcess()
	{
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	void ObjectHolder::_ActiveStoragedJob()
	{
		while (true)
		{
			//m_pIThreadMutex->Lock();
			if (m_AccessList.empty())
			{
				//HADESLOG("_ActiveStoragedJob empty gid %llu\n", m_rpMainAccessor->GetID());
				//m_pIThreadMutex->Unlock();
				break;
			}
			else
			{
				const HolderInfo& hi = m_AccessList.front();
				//m_pIThreadMutex->Unlock();
				if (!_AccessAction(hi.Mode))
				{
					//HADESLOG("_ActiveStoragedJob gid %llu mode %d faile\n", m_rpMainAccessor->GetID(), hi.Mode);
					break;
				}
				else
				{
					//m_pIThreadMutex->Lock();
					//HADESLOG("_ActiveStoragedJob gid %llu mode %d success\n", m_rpMainAccessor->GetID(), hi.Mode);
					_RequestJob(hi.Timestamp, hi.Mode, hi.Func);
					m_AccessList.pop_front();
					//m_pIThreadMutex->Unlock();
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ObjectHolder::_WriteRelease()
	{
		m_pIThreadMutex->Lock();
		m_Timestamp = IKeeperSystem::IncreaseTimestamp(m_Timestamp);
		m_eHolderState = HS_NON_ACESS;
		_ActiveStoragedJob();
		m_pIThreadMutex->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void ObjectHolder::_ReadRelease()
	{
		//if (0 == ThreadSafeOprator::Decrement(m_ReadReferenceCount))
		{
			m_pIThreadMutex->Lock();
			ASSERT(HS_READ_ONLY == m_eHolderState);
			//HADESLOG("released gid %llu, count %d\r\n", m_rpMainAccessor->GetID(), m_ReadReferenceCount - 1);
			if (0 == --m_ReadReferenceCount)
			{
				m_eHolderState = HS_NON_ACESS;
				_ActiveStoragedJob();

			}
			m_pIThreadMutex->Unlock();
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ObjectHolder::Initialize(uint64 id)
	{
		ASSERT(NULL == m_rpMainAccessor);
		m_rpMainAccessor = Accessorpool::Instance()->Get();
		m_rpMainAccessor->Reset();
		m_rpMainAccessor->SetID(id);
	}
	//---------------------------------------------------------------------------------------------------------
	void ObjectHolder::Exit()
	{
		Accessorpool::Instance()->Free(m_rpMainAccessor);
		m_rpMainAccessor = NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	bool ObjectHolder::_AccessAction(hc::Accessor::AccessMode am)
	{
		bool b;
		switch (am)
		{
		case hc::Accessor::AM_UNRIGOROUS:
			b = _TryUnrigorousAcess();
			break;
		case hc::Accessor::AM_READ_ONLY:
			b = _TryReadAcess();
			break;
		case hc::Accessor::AM_READ_WRITE:
			b = _TryWriteAcess();
			break;
		default:ASSERT(false); break;
		}
		return b;
	}
	//---------------------------------------------------------------------------------------------------------
	void ObjectHolder::_StorageJob(int timestamp, hc::Accessor::AccessMode am, AccessCallback::Callback func)
	{
		//m_pIThreadMutex->Lock();
		m_AccessList.push_back(HolderInfo(timestamp, am, func));
		//m_pIThreadMutex->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void ObjectHolder::_RequestJob(int timestamp, hc::Accessor::AccessMode am, AccessCallback::Callback func)
	{
		hc::SmartPtr<CopyJob> job = HolderThread::Instance()->GetCopyJob();
		job->SetAccessor(m_rpMainAccessor);
		job->SetCallback(func);
		job->SetMode(am);
		job->SetCopyFlag(timestamp != m_Timestamp);
		job->SetTimestamp(m_Timestamp);
		HolderThread::Instance()->PushJob(job);
	}
	//---------------------------------------------------------------------------------------------------------
	bool ObjectHolder::UnrigorousAccessor(int timestamp, AccessCallback::Callback func)
	{
		//由于memcpy当读写同时进行的时候会产生问题
		//也不能使用swap的方式，在极端情况下也会出现问题
		bool res = true;
		m_pIThreadMutex->Lock();
		if (!_AccessAction(hc::Accessor::AM_UNRIGOROUS))
		{
			_StorageJob(timestamp, hc::Accessor::AM_UNRIGOROUS, func);
			m_pIThreadMutex->Unlock();
			res = false;
		}
		else
		{
			m_pIThreadMutex->Unlock();
			_RequestJob(timestamp, hc::Accessor::AM_UNRIGOROUS, func);
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	bool ObjectHolder::ReadonlyAccessor(int timestamp, AccessCallback::Callback func)
	{
		bool res = true;
		m_pIThreadMutex->Lock();
		if (!_AccessAction(hc::Accessor::AM_READ_ONLY))
		{
			//HADESLOG("ReadonlyAccessor %llu fail\r\n", m_rpMainAccessor->GetID());
			_StorageJob(timestamp, hc::Accessor::AM_READ_ONLY, func);
			m_pIThreadMutex->Unlock();
			res = false;
		}
		else
		{
			//HADESLOG("ReadonlyAccessor %llu sucess\r\n", m_rpMainAccessor->GetID());
			m_pIThreadMutex->Unlock();
			_RequestJob(timestamp, hc::Accessor::AM_READ_ONLY, func);
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	bool ObjectHolder::ReadwriteAccessor(int timestamp, AccessCallback::Callback func)
	{
		bool res = true;
		m_pIThreadMutex->Lock();
		if (!_AccessAction(hc::Accessor::AM_READ_WRITE))
		{
			//HADESLOG("ReadwriteAccessor %llu fail\r\n", m_rpMainAccessor->GetID());
			_StorageJob(timestamp, hc::Accessor::AM_READ_WRITE, func);
			m_pIThreadMutex->Unlock();
			res = false;
		}
		else
		{
			//HADESLOG("ReadwriteAccessor %llu sucess\r\n", m_rpMainAccessor->GetID());
			m_pIThreadMutex->Unlock();
			_RequestJob(timestamp, hc::Accessor::AM_READ_WRITE, func);
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void ObjectHolder::ReleaseAccessor(hc::Accessor* acc)
	{
		ASSERT(HS_NON_ACESS != m_eHolderState);
		hc::SmartPtr<SaveJob> job = HolderThread::Instance()->GetSaveJob();
		job->SetAccessors(m_rpMainAccessor, acc);
		job->SetCallback(m_SaveCallback);
		HolderThread::Instance()->PushJob(job);
	}

	//---------------------------------------------------------------------------------------------------------
	void ObjectHolder::OnSaved(hc::Accessor::AccessMode am)
	{
		switch (am)
		{
		case hc::Accessor::AM_UNRIGOROUS:
		{
			break;
		}
		case hc::Accessor::AM_READ_ONLY:
		{
			_ReadRelease();
			break;
		}
		case hc::Accessor::AM_READ_WRITE:
		{
			_WriteRelease();
			break;
		}
		default:ASSERT(false); break;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	uint ObjectHolder::Count()
	{
		m_pIThreadMutex->Lock();
		uint res = m_AccessList.size();
		m_pIThreadMutex->Unlock();
		return res;
	}
}