#pragma once
#include "serialize/ISerializable.h"




namespace hc
{
	//signature数据放在Data之后，所以需要先进行data的填充再进行sig的填充
	class HADES_CORE_API IMessage : public ISerializable
	{
		SERIALIZABL_TYPE_DECLEAR(IMessage);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IMessage, ISerializable);
	private:
		byte*	m_pBuffer;
	protected:
		IMessage(const IMessage& rhs);
	public:
		IMessage();
		virtual ~IMessage();
	public:
		virtual const byte* GetBuffer() const;
		virtual uint GetLength() const;
		virtual bool FromBuffer(const byte* buff, uint rawlen);
		virtual void OnRecycle();
	private:
		const IMessage& operator =(const IMessage& rhs);
		byte* _DeliverBufferControl();
	public:
		byte* PrepareMessage(uint id, uint msglen);
		const IMessage& operator =(IMessage& rhs);
	private:
		virtual void _SetHead(uint id, uint rawlen) = 0;
		virtual uint _GetHeadSize() const = 0;
	public:
		virtual uint GetID() const = 0;
		virtual bool isValid() const = 0;
		virtual uint GetRawLength() const = 0;
		virtual uint GetMessageLength() const = 0;
		virtual const byte* GetMessageBuffer() const = 0;
	protected:
		HADES_FORCEINLINE byte* _GetRawBuffer();
	public:
		HADES_FORCEINLINE const byte* GetRawBuffer() const;
		HADES_FORCEINLINE uint GetHeadSize() const;
	};


	
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const byte* IMessage::GetRawBuffer() const
	{
		return m_pBuffer;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE byte* IMessage::_GetRawBuffer()
	{
		return m_pBuffer;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint IMessage::GetHeadSize() const
	{
		return _GetHeadSize();
	}
}