#include "network/socket/ISocketSystem.h"
#include "script/ICoroutineInvoker.h"
#include "script/IFunctionInvoker.h"
#include "configure/IConfigSystem.h"
#include "serialize/ISerializable.h"
#include "network/socket/ISocket.h"
#include "network/server/IServer.h"
#include "script/IMemberIterator.h"
#include "script/IScriptInstance.h"
#include "script/IVirtualMachine.h"
#include "core/ThreadSafeOprator.h"
#include "thread/IJobDispatcher.h"
#include "serialize/ISerializer.h"
#include "configure/IConfigure.h"
#include "thread/IThreadSystem.h"
#include "keeper/IKeeperSystem.h"
#include "thread/IThreadMutex.h"
#include "core/SmartPtr.hpp"
#include "keeper/Accessor.h"
#include "network/Message.h"
#include "MonkeyHandle.h"
#include "core/Types.h"
#include "Configure.h"
#include "log/ILogSystem.h"



using namespace hc;
using namespace eastl;
namespace hmka
{
	//---------------------------------------------------------------------------------------------------------
	MonkeyHandle::MonkeyHandle()
		:m_pIVirtualMachine(NULL)
		, m_pIScriptInstance(NULL)
		, m_pILooperFunction(NULL)
		, m_isInJobQueue(false)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	MonkeyHandle::~MonkeyHandle()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void MonkeyHandle::Initialize(const eastl::string& file)
	{
		ISocketSystem::SocketType ct = ISocketSystem::ST_COMMON_TL;
		m_spIClient = ISocketSystem::Instance()->SocketFactory(ct);
		m_spIClient->SetReciveCallback(MEMBER_FUNC_PTR(&MonkeyHandle::OnReceive));

		m_pIVirtualMachine = IScriptSystem::Instance()->VirtualMachineFactory();
		m_pIScriptInstance = m_pIVirtualMachine->LoadScript(file);

		m_pIVirtualMachine->SetGlobal(Configure::GLOBAL_SOCKET, m_spIClient.GetUnsafePointer(), NULL, &ScriptValue::NullCleaner);
		m_pIVirtualMachine->SetGlobal(Configure::GLOBAL_MONKEY_HANDLE, this, NULL, &ScriptValue::NullCleaner);
		m_pILooperFunction = m_pIScriptInstance->SignFunciton(Configure::LOOP_FUNCITON, Instance::SID());

		IFunctionInvoker* init = m_pIScriptInstance->SignFunciton(Configure::INITIALIZE_FUNCITON, Instance::SID());
		init->Invoke(&m_pIScriptInstance);
		m_pIScriptInstance->UnsignInvoker(init);
	}
	//---------------------------------------------------------------------------------------------------------
	void MonkeyHandle::Exit()
	{
		ISocketSystem::Instance()->RecycleBin(m_spIClient);
		m_pIScriptInstance->UnsignInvoker(m_pILooperFunction);
		m_pIVirtualMachine->ReleaseScript(m_pIScriptInstance);
		IScriptSystem::Instance()->RecycleBin(m_pIVirtualMachine);
	}
	//---------------------------------------------------------------------------------------------------------
	void MonkeyHandle::_DoJob(hc::SmartPtr<hc::IJob>& self, Object* data)
	{		
		m_pILooperFunction->Invoke(&m_pIScriptInstance);
		m_isInJobQueue = false;
	}
	//---------------------------------------------------------------------------------------------------------
	void MonkeyHandle::_DoYieldJob()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void MonkeyHandle::OnReceive(Message* mes)
	{
		ASSERT(NULL != mes);
		m_LockfreeMessageQueue.enqueue(mes);
	}
	//---------------------------------------------------------------------------------------------------------
	hc::Message* MonkeyHandle::CheckReceive()
	{
		hc::Message* mes = NULL;
		m_LockfreeMessageQueue.try_dequeue(mes);
		return mes;
	}
}