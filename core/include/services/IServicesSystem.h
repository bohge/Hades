#pragma once
#include "core/Recycleablevector.hpp"
#include "services/IServices.h"
#include "services/ISession.h"
#include "services/Protocol.h"
#include "core/Singleton.hpp"
#include "core/SmartPtr.hpp"
#include "core/Delegate.hpp"
#include "log/ILogSystem.h"
#include "core/IPlugin.h"



#include <EASTL/string.h>
#include <EASTL/map.h>

namespace hc
{
	class IAgent;
	class IResumer;
	class IThreadRWLock;
	class ClassRegister;
	class ScriptServices;
	class IJobDispatcher;
	class IVirtualMachine;
	class HADES_CORE_API IServicesSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(IServicesSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IServicesSystem, IPlugin);
	protected:
		enum Constant
		{
			SSC_SERVICES_SIZE = 10000,
			SSC_PREPARE_SIZE = 10000,
		};
	private:
		typedef moodycamel::ConcurrentQueue< ScriptServices* > ScriptServicesCache;
		typedef eastl::map< uint, ScriptServicesCache* > ScriptServicesCacheMap;
		typedef moodycamel::ConcurrentQueue< Protocol* > ProtocolQueue;
		typedef Recycleablevector< SmartPtr< IServices> > IServicesArray;
		typedef moodycamel::ConcurrentQueue< ISession* > SessionQueue;
	private:
		ScriptServicesCacheMap*		m_pScriptServicesCacheMap;
		SessionQueue*				m_pSessionQueue;
		ProtocolQueue*				m_pProtocolQueue;
		IServicesArray*				m_pIServicesArray;
		IJobDispatcher*				m_pWorkThread;
		IJobDispatcher*				m_pDispatchThread;
		IThreadRWLock*				m_pIThreadRWLock;
		uint						m_ServiceCacheSize;
		bool						m_isOptimization;
	public:
		IServicesSystem(void);
		virtual ~IServicesSystem(void);
	private:
		virtual SmartPtr<IServices> _OnServiceFactory(const eastl::string& file, ScriptServices* ss) = 0;
		virtual ISession* _DoAllocSession() = 0;
		virtual void _DoFreeSession(ISession* ss) = 0;
		virtual void _OnServicesStart() = 0;
	public:
		virtual IResumer* AllocResumer(IVirtualMachine* vm) = 0;
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	public:
		Object* OnThreadCreate();
		void OnThreadDestory(Object* obj);
	private:
		IAgent* _ServiceFactory(ScriptServices*& ss, const eastl::string& file);
		ScriptServices* _GetScriptServices(const eastl::string& file);
	public:
		IAgent* ServiceFactory(const eastl::string& file);
		IAgent* FixedServiceFactory(const eastl::string& file);
		void RecycleBin(IAgent* agent);
		void RecycleScriptService(ScriptServices* ss);
		void StartServices(byte threadcount);
		bool RemoteInvoke(uint64 gid, Protocol* ptc);
		bool RemoteCallBack(Protocol* ptc);
		void RemoveService(uint64 gid);
	protected:
		HADES_FORCEINLINE uint64 _PushServices(const SmartPtr<IServices>& ser);
		HADES_FORCEINLINE void _SetUUID(ushort uuid);
	public:
		HADES_FORCEINLINE void SetRPCOptimization(bool isopti);
		HADES_FORCEINLINE bool GetRPCOptimization();
		HADES_FORCEINLINE Protocol* AllocProtocol();
		HADES_FORCEINLINE void FreeProtocol(Protocol* ptc);
		HADES_FORCEINLINE void SetCacheSize(uint s);
		HADES_FORCEINLINE bool hasSerivices(uint64 gid);
		HADES_FORCEINLINE SmartPtr<IServices> GetSerivices(uint64 gid);
		HADES_FORCEINLINE ISession* AllocSession();
		HADES_FORCEINLINE void FreeSession(ISession* ss);
	};





	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE ISession* IServicesSystem::AllocSession()
	{
		ISession* ss;
		if (!m_pSessionQueue->try_dequeue(ss))
		{
			ss = _DoAllocSession();
		}
		return ss;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IServicesSystem::FreeSession(ISession* ss)
	{
		if (NULL != ss)
		{
			ss->Reset();
			m_pSessionQueue->enqueue(ss);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IServicesSystem::_SetUUID(ushort uuid)
	{
		m_pIServicesArray->SetBeginUUID(uuid);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint64 IServicesSystem::_PushServices(const SmartPtr<IServices>& ser)
	{
		return m_pIServicesArray->Push(ser);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IServicesSystem::SetRPCOptimization(bool isopti)
	{
		m_isOptimization = isopti;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool IServicesSystem::GetRPCOptimization()
	{
		return m_isOptimization;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IServicesSystem::SetCacheSize(uint s)
	{
		m_ServiceCacheSize = s;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE Protocol* IServicesSystem::AllocProtocol()
	{
		Protocol* ptc;
		if (!m_pProtocolQueue->try_dequeue(ptc))
		{
			ptc = NEW Protocol;
		}		
		return ptc;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IServicesSystem::FreeProtocol(Protocol* ptc)
	{
		if (NULL != ptc)
		{
			ptc->Reset();
			m_pProtocolQueue->enqueue(ptc);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool IServicesSystem::hasSerivices(uint64 gid)
	{
		return m_pIServicesArray->Have(gid);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE SmartPtr<IServices> IServicesSystem::GetSerivices(uint64 gid)
	{
		SmartPtr<IServices> service;
		if (!m_pIServicesArray->Have(gid))
		{
			HADESERROR("Try get unkown service gid %llu\r\n", gid);
		}
		else
		{
			service = m_pIServicesArray->Get(gid);
		}
		return service;
	}
}
