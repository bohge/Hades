#include "ThreadMutex.h"
#include "Threading.h"
#include "log/ILogSystem.h"




using namespace hc;
namespace hpts
{
	//------------------------------------------------------------------------------------------------------
	Threading::Threading(void)
		:m_isRunning(false)
		, m_pMutex(NEW ThreadMutex)
		, m_pThreadID(NEW pthread_t)
		, m_pThreadAttr(NEW pthread_attr_t)
		, m_ePriority(ThreadProperty::TP_NORMAL)
	{
		pthread_attr_init(m_pThreadAttr);
	}
	//------------------------------------------------------------------------------------------------------
	Threading::~Threading(void)
	{
		pthread_attr_destroy(m_pThreadAttr);
		SAFE_DELETE(m_pThreadAttr);
		SAFE_DELETE(m_pThreadID);
		SAFE_DELETE(m_pMutex);
	}
	//------------------------------------------------------------------------------------------------------
	void* Threading::ThreadProc(void* param)
	{
		Threading* ptr = static_cast<Threading*>(param);
		ptr->m_pMutex->Unlock();
		return ptr->DoWork();
	}
	//------------------------------------------------------------------------------------------------------
	void Threading::Start()
	{
		//ASSERT( false == m_isRunning );		
		if (!m_isRunning)//可能是reset之后的线程，这里不需要初始化
		{//初始化线程参数
			m_pMutex->Lock();
			m_isRunning = true;
			pthread_attr_setschedpolicy(m_pThreadAttr, SCHED_FIFO);
			int policy = -1;
			pthread_attr_getschedpolicy(m_pThreadAttr, &policy);
			int max = sched_get_priority_max(policy);
			int min = sched_get_priority_min(policy);
			int half = min + (max - min) / 2;
			int pri = 0;
			switch (m_ePriority)
			{
			case ThreadProperty::TP_LOW: pri = min; break;
			case ThreadProperty::TP_NORMAL: pri = half; break;
			case ThreadProperty::TP_HIGH: pri = max; break;
			default:ASSERT(false);
			}
			sched_param sched;
			sched.sched_priority = pri;
			pthread_attr_setscope(m_pThreadAttr, PTHREAD_SCOPE_SYSTEM);
			pthread_attr_setschedparam(m_pThreadAttr, &sched);
			pthread_create(m_pThreadID, m_pThreadAttr, ThreadProc, (void*)this);
		}
		_OnStart();
		m_pMutex->Lock();
		if (!m_Name.empty())
		{
			pthread_setname_np(*m_pThreadID, m_Name.c_str());
		}
		m_pMutex->Unlock();
	}
	//------------------------------------------------------------------------------------------------------
	void Threading::Reset()
	{
		ASSERT(m_isRunning);
		_OnReset();
	}
	//------------------------------------------------------------------------------------------------------
	void Threading::Stop()
	{
		ASSERT(m_isRunning);
		_StopWork();
		//if( !_StopWork( ) )
		{
			void* ptr = 0;
			pthread_join(*m_pThreadID, &ptr);
			if (NULL != ptr)
			{
				pthread_exit(ptr);
			}
		}
		m_isRunning = false;
	}
	//------------------------------------------------------------------------------------------------------
	void Threading::SetPriority(ThreadProperty::ThreadPriority tp)
	{
		m_ePriority = tp;
	}
	//------------------------------------------------------------------------------------------------------
	void Threading::SetName(const eastl::string& name)
	{
		m_Name = name;
		if (m_isRunning)
		{
			pthread_setname_np(*m_pThreadID, m_Name.c_str());
		}
	}

}