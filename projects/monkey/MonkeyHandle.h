#pragma once
#include "core/Nonclearvector.hpp"
#include "keeper/IAccessPackage.h"
#include "keeper/Accessor.h"
#include "thread/IJob.h"
#include "core/Types.h"

#include <EASTL/vector.h>

namespace hc
{
	enum CallbackType;
	class Message;
	class IDecoder;
	class IEncoder;
	class IThreadMutex;
	class IAccessPackage;
	class IScriptInstance;
	class IVirtualMachine;
	class IFunctionInvoker;
}
namespace hmka
{
	class MonkeyHandle : public hc::IJob
	{
	private:
		typedef moodycamel::ConcurrentQueue< hc::Message* > MessageQueue;
	private:
		MessageQueue				m_LockfreeMessageQueue;
		hc::IVirtualMachine*	    m_pIVirtualMachine;
		hc::IScriptInstance*		m_pIScriptInstance;
		hc::IFunctionInvoker*	    m_pILooperFunction;
		hc::SmartPtr<hc::ISocket>   m_spIClient;
		bool						m_isInJobQueue;
	public:
		MonkeyHandle();
		virtual ~MonkeyHandle();
	public:
		void OnReceive(hc::Message* m);
		hc::Message* CheckReceive();
	private:
		void _ReleaseInstance();
		virtual void _DoYieldJob();
		virtual void _DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* data);
	public:
		void Initialize(const eastl::string& file);
		void Exit();
	public:
		HADES_FORCEINLINE bool CheckPushJob();
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool MonkeyHandle::CheckPushJob()
	{
		return m_isInJobQueue ? false : m_isInJobQueue = true;
	}
}
