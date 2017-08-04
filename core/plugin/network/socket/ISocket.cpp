#include "services/IServicesSystem.h"
#include "network/socket/ISocket.h"
#include "network/RequestHandle.h"
#include "network/RequestPool.h"
#include "services/Protocol.h"
#include "services/IAgent.h"


namespace hc
{
	RTTI_TYPE_IMPLEMENT(ISocket)
	//---------------------------------------------------------------------------------------------------------
	ISocket::ISocket()
		:m_isConnected(false)
		, m_AgentIndex(0)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	ISocket::~ISocket()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void ISocket::PushDefualtHandler(IAgent* agent)
	{
		if (NULL == agent)
		{
			HADESERROR("Try push null service into socket");
		}
		else
		{
			if (m_isConnected)
			{
				HADESERROR("Push defualt handler befor socket connect!");
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
	//---------------------------------------------------------------------------------------------------------
	void ISocket::_OnConnected()
	{
		m_isConnected = true;
		if (!m_ConnectCallback.Empty())
		{
			m_ConnectCallback.Unicast(this);
		}
		if (m_DefualtHandleArray.size() > 0)
		{
			Protocol* ptc = IServicesSystem::Instance()->AllocProtocol();
			_RemoteInvoke(ptc, CONNECTINDX);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ISocket::_RemoteInvoke(Protocol* ptc, uint func)
	{
		SmartPtr<RequestHandle> prh;
		uint index = m_AgentIndex;
		prh = m_DefualtHandleArray[index];
		++index;
		m_AgentIndex = index < m_DefualtHandleArray.size() ? index : 0;
		if (prh.isValid())
		{
			uint fid = prh->Functions[func];
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
			HADESERROR("socke defualt handle unvalid");
			IServicesSystem::Instance()->FreeProtocol(ptc);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ISocket::_OnReciveMessage(IMessage* msg)
	{
		if (!m_SocketCallback.Empty())
		{
			m_SocketCallback.Unicast(msg);
		}
		if (m_DefualtHandleArray.size() > 0)
		{
			Protocol* ptc = IServicesSystem::Instance()->AllocProtocol();
			ptc->SetMessage(msg);
			_RemoteInvoke(ptc, REQUESTINDX);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ISocket::Disconnect()
	{
		m_isConnected = false;
		m_DefualtHandleArray.clear();
		_DoDisconnect();
	}
}