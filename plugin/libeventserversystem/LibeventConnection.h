#pragma once
#include "core/SmartPtr.hpp"
#include "ServerJob.h"

#include <event2/event.h>
#include <event2/bufferevent.h>


namespace hc
{
	class IMessage;
	class IRawbuffer;
}
namespace hles
{
	class LibeventConnection : public hc::Object
	{
		RTTI_TYPE_DECLEAR(LibeventConnection);
	private:
		uint64							m_Specific;
		uint64							m_ID;
		hc::IRawbuffer*					m_pRawbuffer;
		ServerJob*						m_rpHost;
		struct evbuffer*				m_rpInputBuffer;
		struct evbuffer*				m_rpOutputBuffer;
		struct bufferevent*				m_pBufferevent;
	public:
		LibeventConnection(void);
		virtual ~LibeventConnection(void);
	public:
		void Initialize(ServerJob* host, struct event_base* base, evutil_socket_t fd);
		void OnReceive(struct bufferevent* bev);
		bool SendBuffer(const byte* buf, uint size);
	public:
		HADES_FORCEINLINE uint64 GetIndex();
		HADES_FORCEINLINE void SetIndex(uint64 index);
		HADES_FORCEINLINE void OnConnect();
		HADES_FORCEINLINE void Disconnect();
		HADES_FORCEINLINE void SetSpecific(uint64 id);
		HADES_FORCEINLINE uint64 GetSpecific();
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
	HADES_FORCEINLINE void LibeventConnection::OnConnect()
	{
		m_rpHost->OnConnect(m_ID);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventConnection::Disconnect()
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
}