#pragma once
#include "core/SmartPtr.hpp"
#include "ConnectionJob.h"
#include "ListenerJob.h"

#include <event2/event.h>
#include <event2/bufferevent.h>


namespace hc
{
	class IMessage;
	class IRawbuffer;
	class IThreadRWLock;
}
struct ssl_st;
typedef struct ssl_st SSL;
namespace hles
{
	class LibeventConnection : public hc::Object
	{
		RTTI_TYPE_DECLEAR(LibeventConnection);
	private:
		ConnectionJob*					m_rpConnectionHost;
		uint64							m_Specific;
		uint64							m_ID;
		hc::IRawbuffer*					m_pRawbuffer;
		hc::IThreadRWLock*				m_pIThreadRWLock;
		ListenerJob*					m_rpHost;
		struct bufferevent*				m_pBufferevent;
		SSL*							m_pSSL;
		uint							m_ThreadIndex;
	public:
		LibeventConnection(void);
		virtual ~LibeventConnection(void);
	public:
		void Connect(ListenerJob* host, ConnectionJob* conn, evutil_socket_t fd);
		void Disconnect();
		void OnReceive(struct bufferevent* bev);
		bool SendBuffer(const byte* buf, uint size);
	public:
		HADES_FORCEINLINE uint64 GetIndex();
		HADES_FORCEINLINE void SetIndex(uint64 index);
		HADES_FORCEINLINE void OnDisconnect();
		HADES_FORCEINLINE void SetSpecific(uint64 id);
		HADES_FORCEINLINE uint64 GetSpecific();
		HADES_FORCEINLINE void SetThreadIndex(uint index);
		HADES_FORCEINLINE uint GetThreadIndex();
	};

	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint64 LibeventConnection::GetIndex()
	{
		return m_ID;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LibeventConnection::SetIndex(uint64 index)
	{
		m_ID = index;
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventConnection::OnDisconnect()
	{
		m_rpHost->Disconnect(m_ID);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LibeventConnection::SetSpecific(uint64 id)
	{
		ASSERT(hc::nullid == m_Specific);
		m_Specific = id;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint64 LibeventConnection::GetSpecific()
	{
		return m_Specific;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LibeventConnection::SetThreadIndex(uint index)
	{
		m_ThreadIndex = index;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint LibeventConnection::GetThreadIndex()
	{
		return m_ThreadIndex;
	}
}