#pragma once
#include "core/Delegate.hpp"
#include "core/SmartPtr.hpp"
#include "thread/IJob.h"




namespace hc
{
	class ISession;
	class IArguments;
	class ISerializer;
	class ScriptServices;
	class IScriptInstance;
	class IVirtualMachine;
	class FunctionRegister;
	class ICoroutineInvoker;
	class ExecuteJob : public hc::IJob
	{
	private:
		enum Constant
		{
			EC_PREPARE_COROUTINE_SIZE = 10,
		};
	private:
		typedef eastl::list< ICoroutineInvoker* > CoroutineList;
		typedef eastl::vector< eastl::pair<FunctionRegister*, CoroutineList*> > CoroutineAssetCache;
	private:
		CoroutineAssetCache			m_CoroutineAssetCache;
		SmartPtr<ScriptServices>	m_spScriptServices;
		ISession*					m_rpSession;
		IScriptInstance*			m_rpInstance;
		IVirtualMachine*			m_rpIVirtualMachine;
	public:
		ExecuteJob();
		virtual ~ExecuteJob();
	private:
		virtual void _DoYieldJob();
		virtual void _DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata);
	private:
		ICoroutineInvoker* _GetCoroutine(uint index);
	public:
		void Initialize(const ClassRegister* clr);
	private:
		HADES_FORCEINLINE void _RecycleCoroutine(uint index, ICoroutineInvoker* invoke);
	public:
		HADES_FORCEINLINE void SetSession(ISession* ss);
		HADES_FORCEINLINE void SetInstance(IScriptInstance* instance);
		HADES_FORCEINLINE void SetVirtualMachine(IVirtualMachine* vm);
		HADES_FORCEINLINE void SetScriptServices(const SmartPtr<ScriptServices>& ptr);
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ExecuteJob::_RecycleCoroutine(uint index, ICoroutineInvoker* invoke)
	{
		CoroutineList* que = m_CoroutineAssetCache[index].second;
		que->push_back(invoke);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ExecuteJob::SetScriptServices(const SmartPtr<ScriptServices>& ptr)
	{
		m_spScriptServices = ptr;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ExecuteJob::SetSession(ISession* ss)
	{
		m_rpSession = ss;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ExecuteJob::SetInstance(IScriptInstance* instance)
	{
		m_rpInstance = instance;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ExecuteJob::SetVirtualMachine(IVirtualMachine* vm)
	{
		m_rpIVirtualMachine = vm;
	}
}
