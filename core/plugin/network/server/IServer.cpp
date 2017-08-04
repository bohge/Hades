#include "services/IServicesSystem.h"
#include "network/message/IMessage.h"
#include "network/server/IServer.h"
#include "script/IScriptSystem.h"
#include "network/RequestPool.h"
#include "script/IArguments.h"
#include "services/IAgent.h"
#include "core/Configure.h"



#include <sstream>
using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT(IServer);
	//---------------------------------------------------------------------------------------------------------
	IServer::IServer()
		:m_isRunning(false)
		, m_Port(0)
		, m_pRequestHandleArray(NULL)
		, m_AgentIndex(0)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	IServer::~IServer()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	bool IServer::Start(int connectthread, int port)
	{
		ASSERT(false == m_isRunning);
		m_Port = port;
		m_isRunning = _OnStart(connectthread, port);
		if (m_isRunning)
		{
			m_pRequestHandleArray = NEW RequestHandleArray;
			m_pRequestHandleArray->Initialize(SNAC_CLIENT_CACHE_SIZE);
			if (!m_DefualtHandleArray.empty())
			{
				SetServerCallback(MEMBER_FUNC_PTR(&IServer::OnReciveMessage));
			}
		}
		return m_isRunning;
	}
	//---------------------------------------------------------------------------------------------------------
	void IServer::Stop()
	{
		_OnStop();
		ASSERT(true == m_isRunning);
		m_isRunning = false;
		m_DefualtHandleArray.clear();
		SAFE_DELETE(m_pRequestHandleArray);
	}
	//---------------------------------------------------------------------------------------------------------
	void IServer::_SetServerAddress(const eastl::string& ip)
	{
		std::stringstream ss;
		ss << ip.c_str() << ":" << m_Port;
		m_IPAddress = ss.str().c_str();
		HADESLOG("Server ip address is %s", m_IPAddress.c_str());
	}
	//---------------------------------------------------------------------------------------------------------
	void IServer::_RemoteInvoke(Protocol* ptc, uint64 handlegid, uint index)
	{
		SmartPtr<RequestHandle> prh;
		if (!m_pRequestHandleArray->Have(handlegid))
		{
			prh = _GetDefaultAgent();
		}
		else
		{
			prh = m_pRequestHandleArray->Get(handlegid);
		}
		if (prh.isValid())
		{
			uint fid = prh->Functions[index];
			if (nullhandle != fid)
			{
				ptc->SetFunction(fid);
				prh->pAgent->RemoteInvoker(ptc);
			}
			else
			{
				IServicesSystem::Instance()->FreeProtocol(ptc);
			}
		}
		else
		{
			HADESERROR("RequestHandle unvalid");
			IServicesSystem::Instance()->FreeProtocol(ptc);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	Protocol* IServer::_GenerateProtocol(uint64 id, IMessage* message)
	{
		Protocol* ptc = IServicesSystem::Instance()->AllocProtocol();
		ptc->SetMessage(message);
		IArguments* argus = IScriptSystem::Instance()->AllocArguments();
		argus->PushUInt64(id);
		ptc->SetBuffer(argus->GetBuffer(), argus->GetBufferSize());
		IScriptSystem::Instance()->FreeArguments(argus);
		return ptc;
	}
	//---------------------------------------------------------------------------------------------------------
	bool IServer::OnReciveMessage(IServer* ser, uint64 id, CallbackType ct, IMessage* message)
	{
		uint64 handlegid = ser->GetSpecific(id);
		if (IS_FLAG(handlegid, SD_CLIENT_FLAG))
		{
			handlegid = GET_GID(handlegid);//高16位是flag位置
			switch (ct)
			{
			case SCT_CONNECT:break;
			case SCT_REQUEST:_RemoteInvoke(_GenerateProtocol(id, message), handlegid, REQUESTINDX); break;
			case SCT_DISCONNECT:
			{
								   _RemoteInvoke(_GenerateProtocol(id, message), handlegid, DISCONNECTINDX);
								   if (m_pRequestHandleArray->Have(handlegid))
								   {
									   m_pRequestHandleArray->Remove(handlegid);
								   }
								   break;
			}
			default:ASSERT(false);
			}
			return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	SmartPtr<RequestHandle> IServer::_GetDefaultAgent()
	{
		SmartPtr<RequestHandle> res;
		if (m_DefualtHandleArray.size() > 0)
		{
			uint index = m_AgentIndex;
			res = m_DefualtHandleArray[index];
			++index;
			m_AgentIndex = index < m_DefualtHandleArray.size() ? index : 0;
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void IServer::SetClientHandle(uint64 gid, IAgent* agent)
	{
		SmartPtr<RequestHandle> prh = RequestPool::Instance()->CreateRequestHandle(agent);
		if (prh.isValid())
		{
			uint64 handlegid = m_pRequestHandleArray->Push(prh);
			handlegid = SET_FLAG(handlegid, SD_CLIENT_FLAG);
			SetSpecific(gid, handlegid);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void IServer::PushDefualtHandler(IAgent* agent)
	{
		if (NULL == agent)
		{
			HADESERROR("Try push null service into server");
		}
		else
		{
			if (m_isRunning)
			{
				HADESERROR("Push defualt handler befor star server!");
			}
			else
			{
				SmartPtr<RequestHandle> prh = RequestPool::Instance()->CreateRequestHandle(agent);
				if (prh.isValid())
				{
					m_DefualtHandleArray.push_back(prh);
				}
			}
		}
	}
}