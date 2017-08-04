#include "services/DispatchJob.h"
#include "thread/IThreadSystem.h"
#include "thread/IThreadEvent.h"





namespace hc
{
	//---------------------------------------------------------------------------------------------------------
	DispatchJob::Action::Action()
		:GID(nullid)
		, Type(SA_INVOKE)
		, Data(NULL)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	DispatchJob::DispatchJob(IServicesArray* sa)
		:m_rpIServicesArray(sa)
		, m_pCondition(NULL)
		, m_isWorking(false)
		, m_isLiveing(true)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	DispatchJob::~DispatchJob(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void DispatchJob::_DoYieldJob()
	{
		m_isLiveing = false;
		m_pCondition->Signal();
	}
	//---------------------------------------------------------------------------------------------------------
	void DispatchJob::_DoJob(SmartPtr<IJob>& self, Object* userdata)
	{
		while (m_isLiveing)
		{
			m_pCondition->Wait();
			m_isWorking = true;

			Action* res = NULL;
			while (m_ActionQueue.try_dequeue(res))
			{
				m_isWorking = true;

				m_isWorking = false;
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void DispatchJob::_PushAction(Action* a)
	{
		m_ActionQueue.enqueue(a);
		//m_pCondition->Lock();打算用非安全模式，防止多线互相锁对方降低效率
		if (!m_isWorking)
		{
			m_pCondition->Signal();
		}
		//else
		//{
		//	m_pCondition->Unlock();
		//}
	}
	//---------------------------------------------------------------------------------------------------------
	void DispatchJob::Initialize()
	{
		for (size_t i = 0; i < DC_ACTION_SIZE; i++)
		{
			m_RecycleCache.enqueue(NEW Action());
		}
		m_pCondition = static_cast<IThreadEvent*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_EVENT));
	}
	//---------------------------------------------------------------------------------------------------------
	void DispatchJob::Exit()
	{
		Action* res = NULL;
		while (!m_RecycleCache.try_dequeue(res))
		{
			SAFE_DELETE(res);
		}
		IThreadSystem::Instance()->RecycleBin(m_pCondition);
	}
	//---------------------------------------------------------------------------------------------------------
	void DispatchJob::RemoteInvoke(uint64 gid, Protocol* ptc)
	{
		Action* res = _GetAction();
		res->GID = gid;
		res->Type = SA_INVOKE;
		res->Data = ptc;
		_PushAction(res);
	}
	//---------------------------------------------------------------------------------------------------------
	void DispatchJob::RemoteCallBack(Protocol* ptc)
	{
		Action* res = _GetAction();
		res->GID = nullid;
		res->Type = SA_CALLBACK;
		res->Data = ptc;
		_PushAction(res);
	}
	//---------------------------------------------------------------------------------------------------------
	void DispatchJob::RemoveService(uint64 gid)
	{
		Action* res = _GetAction();
		res->GID = gid;
		res->Type = SA_REMOVE;
		res->Data = NULL;
		_PushAction(res);
	}
}
