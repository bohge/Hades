#pragma once
#include "network/socket/ISocket.h"
#include "thread/IThreadSystem.h"
#include "ClientJob.h"


#ifdef _PLATFORM_WINDOWS
#include <WS2tcpip.h>
#include <windows.h>
#else
#include <arpa/inet.h>
#include <unistd.h>
#endif
namespace hc
{
	class IMessage;
	class IRawbuffer;
	class IThreadRWLock;
}

struct evbuffer;
struct event_base;
struct bufferevent;
struct evconnlistener;
namespace hlec
{
	class LibeventClient : public hc::ISocket
	{
	private:
		hc::SmartPtr<hc::ISocket>	m_SelfReferences;//出现这个是应为由可能存在一种情况，连接没有断开，只能指针都NULL了，然后被系统回收再次利用，这样异步的断开会有问题
		eastl::string				m_IP;
		uint64						m_Gid;
		event_base*					m_rpBase;
		evconnlistener*				m_pListener;
		bufferevent*				m_pBufferevent;
		hc::IRawbuffer*				m_pRawbuffer;
		hc::IThreadRWLock*			m_pIThreadRWLock;
		struct sockaddr_in			m_sSin;
		int							m_Port;
	public:
		LibeventClient(void);
		virtual ~LibeventClient(void);
	private:
		virtual void _DoConnect(const eastl::string& ipport);
		virtual void _DoSend(hc::IMessage* msg);
		virtual void _DoDisconnect();
	public:
		virtual bool OnStart();
	public:
		void OnReceive(bufferevent* bev);
		void ConnectReslut(bool res);
	private:
		HADES_FORCEINLINE void _SetIP(const eastl::string& str);
		HADES_FORCEINLINE void _SetPort(int port);
	public:
		HADES_FORCEINLINE void SetBase(event_base* base);
		HADES_FORCEINLINE void SetGID(uint64 gid);
		HADES_FORCEINLINE void SetReferences(const hc::SmartPtr<hc::ISocket>& self);
		HADES_FORCEINLINE uint64 GetGID();
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LibeventClient::SetReferences(const hc::SmartPtr<hc::ISocket>& self)
	{
		m_SelfReferences = self;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LibeventClient::_SetIP(const eastl::string& str)
	{
		m_IP = str;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LibeventClient::_SetPort(int port)
	{
		m_Port = port;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LibeventClient::SetGID(uint64 gid)
	{
		m_Gid = gid;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LibeventClient::SetBase(event_base* base)
	{
		m_rpBase = base;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint64 LibeventClient::GetGID()
	{
		return m_Gid;
	}
}

