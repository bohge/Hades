#pragma once
#include "application/IApplication.h"
#include "core/Recycleablevector.hpp"
#include "network/server/IServer.h"


#include <EASTL/string.h>

namespace hc
{
	class IAgent;
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
	struct RequestHandle;
	class ClusternodeApplication : public hc::IApplication
	{
	private:
		enum Constant
		{
			SNAC_MESSAGE_CACHE_SIZE = 1000,
			SNAC_CLIENT_CACHE_SIZE = 5000,
		};
	private:
		typedef hc::Recycleablevector< hc::SmartPtr<RequestHandle> > RequestHandleArray;
		typedef eastl::map<uint, uint> RequestHandleMap;
		typedef eastl::pair<uint64, uint> ServiceFunctionPair;
		typedef eastl::list<ServiceFunctionPair> ServiceFunctionList;
		typedef eastl::map<uint, ServiceFunctionList*> UpdateMap;
	private:
		RequestHandleMap		m_RequestHandleMap;
		UpdateMap				m_UpdateMap;
		hc::IThreadMutex*		m_pTimerMutex;
		RequestHandleArray*		m_pRequestHandleArray;
		hc::IServer*			m_pIServer;
		hc::IAgent*				m_pIAgent;
		uint					m_FuncInitIndex;
		uint					m_FuncConnectIndex;
		uint					m_FuncDisconnectIndex;
	public:
		ClusternodeApplication(void);
		virtual ~ClusternodeApplication(void);
	private:
		virtual eastl::string _Configurepath();
		virtual void _OnInitialize( );
		virtual bool _OnDowork();
		virtual void _OnExit();
	public:
		void OnTimerCallback(uint64 gid);
		void OnCreateServices(hc::IVirtualMachine* vm);
		void OnReciveMessage(uint64 id, hc::CallbackType ct, hc::Message* message);
	public:
		void ScriptInitialized();
		void SetClientHandle(uint64 gid, hc::IAgent* agent);
		void RegisterTimer(hc::IAgent* agent, uint freq, const eastl::string& function);
	};
}

