#pragma once
#include "core/Recycleablevector.hpp"
#include "script/IScriptInstance.h"
#include "services/ExecuteJob.h"
#include "core/SmartPtr.hpp"


#include <atomic>

namespace hc
{
	class IAgent;
	class ISession;
	class Protocol;
	class IThreadMutex;
	class IJobDispatcher;
	class IScriptInstance;
	class IVirtualMachine;
	class ICoroutineInvoker;
	class HADES_CORE_API ScriptServices : public Object
	{
		RTTI_TYPE_DECLEAR(ScriptServices);
	private:
		enum Constant
		{
			SS_SESSION_SIZE = 1000,
		};
	public:
		typedef eastl::pair<uint64, ISession*> MachineUserData;
		typedef Recycleablevector< ISession* > SessionArray;
	private:
		typedef eastl::list<uint64>	UnhandleSessiones;
	private:
		MachineUserData				m_MachineUserData;
		SmartPtr<ExecuteJob>		m_spExecuteJob;
		eastl::string				m_ServiceName;
		UnhandleSessiones*			m_pUnhandleSessiones;
		SessionArray*				m_pSessionArray;
		IVirtualMachine*			m_pVirtualMachine;
		IScriptInstance*			m_pIScriptInstance;
		IThreadMutex*				m_pIThreadMutex;
		IJobDispatcher*				m_rpIJobDispatcher;
		volatile int				m_SessionCount;
		const uint					m_CacheSize;
		bool						m_isExecuting;
		bool						m_isLiving;
	public:
		ScriptServices(uint size, IJobDispatcher* thread);
		virtual ~ScriptServices(void);
	public:
		void OnExecuteDone(ISession* ss);
	private:
		void _DoCleanup();
		void _ExecuteSessionWithUnlock();
	public:
		void Initialize(const ClassRegister* clr);
		bool LoadScript(const eastl::string& file);
		bool RemoteInvoker(Protocol* ptc);
		bool RemoteCallBack(Protocol* ptc);
		void SetSelfHandle(const SmartPtr<ScriptServices>& self);
		void Cleanup();
		void FixedThread();
	public:
		HADES_FORCEINLINE void Relive();
		HADES_FORCEINLINE void SetGID(uint64 id);
		HADES_FORCEINLINE void SetActiveSession(ISession* ss);
		HADES_FORCEINLINE IVirtualMachine* GetVirtualMachine();
		HADES_FORCEINLINE IScriptInstance* GetScriptInstance();
		HADES_FORCEINLINE const eastl::string& GetServiceName();
	};



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ScriptServices::Relive()
	{
		m_isLiving = true;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE IScriptInstance* ScriptServices::GetScriptInstance()
	{
		return m_pIScriptInstance;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const eastl::string& ScriptServices::GetServiceName()
	{
		return m_ServiceName;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ScriptServices::SetGID(uint64 id)
	{
		m_MachineUserData.first = id;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ScriptServices::SetActiveSession(ISession* ss)
	{
		m_MachineUserData.second = ss;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE IVirtualMachine* ScriptServices::GetVirtualMachine()
	{
		return m_pVirtualMachine;
	}
}
