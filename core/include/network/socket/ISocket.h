#pragma once
#include "core/Recycleablevector.hpp"
#include "core/SmartPtr.hpp"
#include "core/Delegate.hpp"
#include "core/Object.hpp"


#include <EASTL/string.h>

namespace hc
{
	class IAgent;
	class ISocket;
	class IMessage;
	class Protocol;
	struct RequestHandle;
	typedef Unidelegate1<void, ISocket*> ConnectCallback;
	typedef Unidelegate1<void, IMessage*> SocketCallback;
	class HADES_CORE_API ISocket : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(ISocket);
	private:
		enum Constant
		{
			SNAC_HANDLE_CACHE_SIZE = 5,
		};
	private:
		typedef eastl::vector< SmartPtr<RequestHandle> > DefualtHandleArray;
	private:
		DefualtHandleArray	m_DefualtHandleArray;
		ConnectCallback		m_ConnectCallback;
		SocketCallback		m_SocketCallback;
		eastl::string		m_IPAddress;
		uint				m_AgentIndex;
		bool				m_isConnected;
	public:
		ISocket();
		virtual ~ISocket();
	private:
		virtual void _DoConnect(const eastl::string& ipport) = 0;
		virtual void _DoDisconnect() = 0;
		virtual void _DoSend(IMessage* msg) = 0;
	public:
		void PushDefualtHandler(IAgent* agent);
		void Connect(const eastl::string& ipport, ConnectCallback::Callback callback);
		void Disconnect();
	private:
		void _RemoteInvoke(Protocol* ptc, uint index);
	protected:
		void _OnReciveMessage(IMessage* msg);
		void _OnConnected();
		void _OnDisconnected();
	public:
		HADES_FORCEINLINE void SetReciveCallback(SocketCallback::Callback func);
		HADES_FORCEINLINE void Send(IMessage* msg);
		HADES_FORCEINLINE const eastl::string& GetSocketAddress();
		HADES_FORCEINLINE bool isConnected();
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ISocket::SetReciveCallback(SocketCallback::Callback func)
	{
		m_SocketCallback.Add(func);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ISocket::Connect(const eastl::string& ipport, ConnectCallback::Callback callback)
	{
		m_IPAddress = ipport;
		m_ConnectCallback.Add(callback);
		_DoConnect(ipport);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ISocket::Send(IMessage* msg)
	{
		if (m_isConnected)
		{
			_DoSend(msg);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const eastl::string& ISocket::GetSocketAddress()
	{
		return m_IPAddress;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool ISocket::isConnected()
	{
		return m_isConnected;
	}
}