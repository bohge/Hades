#pragma once
#include "serialize/ISerializable.h"
#include "network/socket/ISocket.h"
#include "core/SmartPtr.hpp"

namespace hc
{
	class ISocket;
	class HADES_CORE_API SocketProxy : public ISerializable
	{
		SERIALIZABL_TYPE_DECLEAR(SocketProxy);
		RTTI_DRIVER_TYPE_DECLEAR(SocketProxy, ISerializable);
	private:
		SmartPtr<ISocket>	m_spSocket;
		uint64				m_GIndex;
	public:
		SocketProxy();
		virtual ~SocketProxy();
	public:
		virtual const byte* GetBuffer() const;
		virtual uint GetLength() const;
		virtual bool FromBuffer(const byte* buff, uint rawlen);
		virtual void OnRecycle();
		virtual ScriptValue::ClearFunction GetClearFunction();
	public:
		void SetIndex(uint64 gid);
	public:
		HADES_FORCEINLINE uint64 GetIndex();
		HADES_FORCEINLINE void SetReciveCallback(SocketCallback::Callback func);
		HADES_FORCEINLINE void Connect(const eastl::string& ipport, ConnectCallback::Callback callback);
		HADES_FORCEINLINE void Send(IMessage* msg);
		HADES_FORCEINLINE void Disconnect();
		HADES_FORCEINLINE const eastl::string& GetSocketAddress();
		HADES_FORCEINLINE bool isConnected();
		HADES_FORCEINLINE void PushDefualtHandler(IAgent* agent);
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint64 SocketProxy::GetIndex()
	{
		return m_GIndex;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void SocketProxy::SetReciveCallback(SocketCallback::Callback func)
	{
		m_spSocket->SetReciveCallback(func);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void SocketProxy::Connect(const eastl::string& ipport, ConnectCallback::Callback callback)
	{
		m_spSocket->Connect(ipport, callback);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void SocketProxy::Send(IMessage* msg)
	{
		m_spSocket->Send(msg);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void SocketProxy::Disconnect()
	{
		m_spSocket->Disconnect();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const eastl::string& SocketProxy::GetSocketAddress()
	{
		return m_spSocket->GetSocketAddress();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool SocketProxy::isConnected()
	{
		return m_spSocket->isConnected();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void SocketProxy::PushDefualtHandler(IAgent* agent)
	{
		m_spSocket->PushDefualtHandler(agent);
	}
}