#pragma once
#include "core/Recycleablevector.hpp"
#include "thread/IJob.h"




namespace hc
{
	class Protocol;
	class IServices;
	class IThreadEvent;
	class DispatchJob : public IJob
	{
	private:
		enum Constant
		{
			DC_ACTION_SIZE = 10000,
		};
		enum ActionType
		{
			SA_INVOKE	= 0,
			SA_CALLBACK = 1,
			SA_REMOVE	= 2,
		};
	private:
		struct Action
		{
			uint64		GID;
			Protocol*	Data;
			ActionType	Type;
			Action();
		};
	private:
		typedef moodycamel::ConcurrentQueue< Action* > ActionQueue;
		typedef Recycleablevector< SmartPtr< IServices> > IServicesArray;
	public:
		ActionQueue			m_ActionQueue;
		ActionQueue			m_RecycleCache;
		IThreadEvent*		m_pCondition;
		IServicesArray*		m_rpIServicesArray;
		bool				m_isWorking;
		bool				m_isLiveing;
	public:
		DispatchJob(IServicesArray* sa);
		virtual ~DispatchJob(void);
	private:
		virtual void _DoYieldJob();
		virtual void _DoJob(SmartPtr<IJob>& self, Object* userdata);
	private:
		void _PushAction(Action* a);
	public:
		void Initialize();
		void RemoteInvoke(uint64 gid, Protocol* ptc);
		void RemoteCallBack(Protocol* ptc);
		void RemoveService(uint64 gid);
		void Exit();

	private:
		HADES_FORCEINLINE Action* _GetAction();
		HADES_FORCEINLINE void _FreeAction(Action* a);
	};

	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE DispatchJob::Action* DispatchJob::_GetAction()
	{
		Action* res = NULL;
		if (!m_RecycleCache.try_dequeue(res))
		{
			res = NEW Action();
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void DispatchJob::_FreeAction(Action* a)
	{
		m_RecycleCache.enqueue(a);
	}
}