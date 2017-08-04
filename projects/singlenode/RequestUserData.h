#pragma once
#include "script/IVirtualMachine.h"
#include "Configure.h"


#include <EASTL/vector.h>


namespace hc
{
	class IServer;
	class ISerializer;
	class IScriptInstance;
	class IFunctionInvoker;
	class ICoroutineInvoker;
}
namespace hsna
{
	class SinglenodeApplication;
	class RequestUserData : public hc::Object
	{
		RTTI_TYPE_DECLEAR(RequestUserData);
	private:
		enum Constant
		{
			RUDC_PREPARE_INVOKER_COUNT	= 100,
		};
	private:
		typedef eastl::list<hc::ICoroutineInvoker*>	CoroutineList;
	private:
		hc::IVirtualMachine*	m_pVirtualMachine;
		hc::IScriptInstance*	m_pScriptInstance;
		CoroutineList			m_ConnectCoroutineArray;
		CoroutineList			m_pEntryCoroutineArray;
		CoroutineList			m_pDisconnectCoroutineArray;
		hc::ISerializer*		m_pSerializer;
		hc::IFunctionInvoker*	m_pIFunctionInvoker;
		SinglenodeApplication*	m_pApplication;
		hc::IServer*			m_pServer;
	public:
		RequestUserData();
		virtual ~RequestUserData();
	private:
		hc::ICoroutineInvoker* _CreateOrCached(CoroutineList& corous, const char* cname);
	public:
		bool Initialize(SinglenodeApplication* app, hc::IServer* server, const eastl::string& entry);
		void Exit();
		void DsctoryCoroutine(hc::ICoroutineInvoker* corous);
		void ThreadInitialize();
	public:
		HADES_FORCEINLINE hc::IVirtualMachine* GetVirtualMachine();
		HADES_FORCEINLINE void UpdateVirtualMachine();
		HADES_FORCEINLINE hc::IScriptInstance* GetInstance(uint64 gid);
		HADES_FORCEINLINE SinglenodeApplication* GetApplication();
		HADES_FORCEINLINE hc::ISerializer* GetSerializer();
		HADES_FORCEINLINE hc::ICoroutineInvoker* GetConnectCoroutines();
		HADES_FORCEINLINE hc::ICoroutineInvoker* GetRequestCoroutines();
		HADES_FORCEINLINE hc::ICoroutineInvoker* GetDisconnectCoroutines();
		HADES_FORCEINLINE void RecycleConnectCoroutine(hc::ICoroutineInvoker* corous);
		HADES_FORCEINLINE void RecycleRequestCoroutine(hc::ICoroutineInvoker* corous);
		HADES_FORCEINLINE void RecycleDsionnectCoroutine(hc::ICoroutineInvoker* corous);
	};



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::IVirtualMachine* RequestUserData::GetVirtualMachine()
	{
		return m_pVirtualMachine;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RequestUserData::UpdateVirtualMachine()
	{
		m_pVirtualMachine->Update();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::IScriptInstance* RequestUserData::GetInstance(uint64 gid)
	{
		return m_pVirtualMachine->GetInstance(gid);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::ISerializer* RequestUserData::GetSerializer()
	{
		return m_pSerializer;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE SinglenodeApplication* RequestUserData::GetApplication()
	{
		return m_pApplication;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::ICoroutineInvoker* RequestUserData::GetConnectCoroutines()
	{
		return _CreateOrCached(m_ConnectCoroutineArray, Configure::CONNECT_FUNCTION);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::ICoroutineInvoker* RequestUserData::GetRequestCoroutines()
	{
		return _CreateOrCached(m_pEntryCoroutineArray, Configure::MESSAGE_FUNCTION);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::ICoroutineInvoker* RequestUserData::GetDisconnectCoroutines()
	{
		return _CreateOrCached(m_pDisconnectCoroutineArray, Configure::DISCONNECT_FUNCTION);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RequestUserData::RecycleConnectCoroutine(hc::ICoroutineInvoker* corous)
	{
		ASSERT(NULL != corous);
		m_ConnectCoroutineArray.push_back(corous);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RequestUserData::RecycleRequestCoroutine(hc::ICoroutineInvoker* corous)
	{
		ASSERT(NULL != corous);
		m_pEntryCoroutineArray.push_back(corous);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RequestUserData::RecycleDsionnectCoroutine(hc::ICoroutineInvoker* corous)
	{
		ASSERT(NULL != corous);
		m_pDisconnectCoroutineArray.push_back(corous);
	}
}
