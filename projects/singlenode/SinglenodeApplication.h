#pragma once
#include "application/IApplication.h"
#include "core/Recycleablevector.hpp"
#include "core/SmartPtr.hpp"
#include "concurrentqueue.h"
#include "RequestHandle.h"
#include "BalanceHandle.h"


#include <EASTL/string.h>

namespace hc
{	
	enum CallbackType;
	class IServer;
	class Message;
	class Accessor;
	class IThreadMutex;
	class IJobThreadArray;
	class IVirtualMachine;
	class IScriptInstance;
	class IFunctionInvoker;
}
namespace hsna
{
	class SinglenodeApplication : public hc::IApplication
	{
	private:
		enum  Constant
		{
			SAC_PREPARE_HANDLE_SIZE = 100,
		};
	private:
		typedef eastl::pair< CallbackHandle, hc::IFunctionInvoker* > EventPair;
		typedef hc::Recycleablevector< EventPair > EventVector;
		typedef moodycamel::ConcurrentQueue< hc::SmartPtr<RequestHandle> > FreeRequestQueue;
		typedef moodycamel::ConcurrentQueue< hc::SmartPtr<BalanceHandle> > FreeBalanceQueue;
	private:
		hc::IThreadMutex*		m_pMutex;
		EventVector*			m_pEventVector;
		FreeRequestQueue		m_FreeRequestQueue;
		FreeBalanceQueue		m_FreeBalanceQueue;
		hc::IServer*			m_pIServer;
		hc::IJobThreadArray*	m_pRequestThread;
		hc::IJobThreadArray*	m_pBalanceThread;
		eastl::string*			m_pRootScript;
		eastl::string*			m_pBalanceScript;
		hc::IVirtualMachine*	m_pIVirtualMachine;
		hc::IScriptInstance*	m_pIScriptInstance;
		uint					m_WorkThreadCount;
		bool					m_isUpdate;
	public:
		SinglenodeApplication(void);
		virtual ~SinglenodeApplication(void);
	private:
		virtual eastl::string _Configurepath();
		virtual void _OnInitialize( );
		virtual bool _OnDowork();
		virtual void _OnExit();
	public:
		void OnTimerCallback(uint64 h);
		void ScriptInitialized();
		void OnReciveMessage(uint64 id, hc::CallbackType ct, hc::Message* message);
		void DispatchRequest(uint thread, uint64 id, hc::CallbackType ct, hc::Message* message);
		Object* CreateRequestUserdata();
		void RecycleRequestUserdata(Object* vm);
		Object* CreateBalanceUserdata();
		void RecycleBalanceUserdata(Object* vm);
		uint64 RegisterTimer(float sec, int funcid);
		void UnregisterTimer(uint64 h);
	private:
		HADES_FORCEINLINE void _GetRequestHandle(hc::SmartPtr<RequestHandle>& handle);
		HADES_FORCEINLINE void _GetBalanceHandle(hc::SmartPtr<BalanceHandle>& handle);
	public:
		HADES_FORCEINLINE void RecycleRequestHandle(hc::SmartPtr<RequestHandle>& handle);
		HADES_FORCEINLINE void RecycleBalanceHandle(hc::SmartPtr<BalanceHandle>& handle);
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void SinglenodeApplication::_GetRequestHandle(hc::SmartPtr<RequestHandle>& handle)
	{
		if (!m_FreeRequestQueue.try_dequeue(handle))
		{
			handle = hc::SmartPtr<RequestHandle>(NEW RequestHandle);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void SinglenodeApplication::RecycleRequestHandle(hc::SmartPtr<RequestHandle>& handle)
	{
		ASSERT(!handle->hasRequest());
		m_FreeRequestQueue.enqueue(handle);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void SinglenodeApplication::_GetBalanceHandle(hc::SmartPtr<BalanceHandle>& handle)
	{
		if (!m_FreeBalanceQueue.try_dequeue(handle))
		{
			handle = hc::SmartPtr<BalanceHandle>(NEW BalanceHandle);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void SinglenodeApplication::RecycleBalanceHandle(hc::SmartPtr<BalanceHandle>& handle)
	{
		m_FreeBalanceQueue.enqueue(handle);
	}
}

