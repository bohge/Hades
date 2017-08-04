#pragma once
#include "core/Configure.h"
#include "core/Object.hpp"



namespace hc
{
	class IMessage;
	class HADES_CORE_API Protocol : public Object
	{
		RTTI_TYPE_DECLEAR(Protocol);
	private:
		struct
		{
			uint64			CallerID;
			uint64			CallerSessionID;
			uint			CallerHash;
			uint			RealSize;
			uint			BufferSize;
			uint			FunctionID;
		}m_DataHead;
		byte*			m_rpBuffer;

		//可能是临时代码，为了避免再次的拷贝
		IMessage*		m_pMessage;
	public:
		Protocol(void);
		virtual ~Protocol(void);
	public:
		uint Serialize(byte* buffer);
		void Parse(const byte* buffer, uint len);
		void SetBuffer(const byte* buff, uint size);
		void Reset();
	public:
		HADES_FORCEINLINE void SetCallerGID(uint64 id);
		HADES_FORCEINLINE uint64 GetCallerGID();
		HADES_FORCEINLINE void SetCallerSession(uint64 id);
		HADES_FORCEINLINE uint64 GetCallerSession();
		HADES_FORCEINLINE void SetFunction(uint id);
		HADES_FORCEINLINE uint GetFunction();
		HADES_FORCEINLINE const byte* GetBuffer();
		HADES_FORCEINLINE uint GetBufferSize();
		HADES_FORCEINLINE uint GetUsingSize();
		HADES_FORCEINLINE bool EmptyBuffer();
		HADES_FORCEINLINE void ClearBuffer();
		HADES_FORCEINLINE void SetMessage(IMessage* mes);
		HADES_FORCEINLINE IMessage* GetMessage();
		HADES_FORCEINLINE void SetCallerHash(uint hash);
		HADES_FORCEINLINE uint GetCallerHash();
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint Protocol::GetUsingSize()
	{
		uint off = sizeof(m_DataHead);
		return off + m_DataHead.BufferSize;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Protocol::SetMessage(IMessage* mes)
	{
		m_pMessage = mes;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE IMessage* Protocol::GetMessage()
	{
		return m_pMessage;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool Protocol::EmptyBuffer()
	{
		return 0 == m_DataHead.BufferSize;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Protocol::ClearBuffer()
	{
		m_DataHead.BufferSize = 0;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Protocol::SetCallerGID(uint64 id)
	{
		m_DataHead.CallerID = id;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint64 Protocol::GetCallerGID()
	{
		return m_DataHead.CallerID;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Protocol::SetCallerSession(uint64 id)
	{
		m_DataHead.CallerSessionID = id;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint64 Protocol::GetCallerSession()
	{
		return m_DataHead.CallerSessionID;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint Protocol::GetFunction()
	{
		return m_DataHead.FunctionID;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Protocol::SetFunction(uint id)
	{
		m_DataHead.FunctionID = id;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const byte* Protocol::GetBuffer()
	{
		return m_rpBuffer;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint Protocol::GetBufferSize()
	{
		return m_DataHead.BufferSize;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Protocol::SetCallerHash(uint hash)
	{
		m_DataHead.CallerHash = hash;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint Protocol::GetCallerHash()
	{
		return m_DataHead.CallerHash;
	}
}
