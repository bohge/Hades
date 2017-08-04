#pragma once
#include "serialize/ISerializer.h"


namespace hc
{
	class IDecoder;
	class ISerializer;
	class ISerializable;
	class HADES_CORE_API IArguments : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IArguments);
	private:
		ISerializer*	m_pISerializer;
	public:
		IArguments(void);
		virtual ~IArguments(void);
	private:
		virtual void _Reset() = 0;
		virtual void _Deserializer(IDecoder* de) = 0;
	public:
		virtual bool Empty() = 0;
		virtual void PushUInt(uint data) = 0;
		virtual void PushUInt64(uint64 data) = 0;
		virtual void PushString(const char* v, uint len) = 0;
		virtual void PushSerializable(ISerializable* obj) = 0;
	protected:
		HADES_FORCEINLINE ISerializer* _GetSerializer();
	public:
		HADES_FORCEINLINE const byte* GetBuffer();
		HADES_FORCEINLINE uint GetBufferSize();
		HADES_FORCEINLINE void FromBuffer(const byte* buff);
		HADES_FORCEINLINE void Reset();
	};



	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE ISerializer* IArguments::_GetSerializer()
	{
		return m_pISerializer;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const byte* IArguments::GetBuffer()
	{
		return m_pISerializer->GetBuffer();
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint IArguments::GetBufferSize()
	{
		return m_pISerializer->GetBufferSize();
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IArguments::FromBuffer(const byte* buff)
	{
		IDecoder* de = m_pISerializer->GetDecoderFromBuffer(buff);
		_Deserializer(de);
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IArguments::Reset()
	{
		_Reset();
		m_pISerializer->ClearDecoder();
		m_pISerializer->ClearEncoder();
	}
}
