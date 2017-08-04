#pragma once
#include "HeadStruct.h"
#include "Configure.h"




namespace hc
{
	//signature数据放在Data之后，所以需要先进行data的填充再进行sig的填充
	class Message
	{
	public:
		enum Constant
		{
			MC_USER_DEFINED_ID_MAX = 50000,
		};
	private:
		byte*	m_pBuffer;
		uint	m_Size;
	public:
		Message();
		~Message();
	private:
		void _PrepareBuffer(uint rawlen);
	public:
		void CopyMessage(uint id, const byte* data, uint msglen);
		byte* PrepareMessage(uint id, uint msglen);
	private:
		HADES_FORCEINLINE void _SetHead(uint id, uint rawlen);
		HADES_FORCEINLINE const HeadStruct* _AsHead() const;
	public:
		HADES_FORCEINLINE const Message& operator =(const Message& rhs);
		HADES_FORCEINLINE uint GetID() const;
		HADES_FORCEINLINE bool isValid() const;
		HADES_FORCEINLINE uint GetRawLength() const;
		HADES_FORCEINLINE uint GetMessageLength() const;
		HADES_FORCEINLINE const byte* GetRawBuffer() const;
		HADES_FORCEINLINE const byte* GetMessageBuffer() const;
	};



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void Message::_SetHead(uint id, uint rawlen)
	{
		HeadStruct* head = reinterpret_cast<HeadStruct*>(m_pBuffer);
		head->Fourcc = Configure::FOURCC_NETPACKAGE_HEAD;
		head->ID = id;
		head->RawLength = rawlen;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const HeadStruct* Message::_AsHead() const
	{
		return reinterpret_cast<const HeadStruct*>(m_pBuffer);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const Message& Message::operator=(const Message& rhs)
	{
		CopyMessage(rhs.GetID(), rhs.GetMessageBuffer(), rhs.GetMessageLength());
		return *this;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint Message::GetID() const
	{
		return _AsHead()->GetID();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool Message::isValid() const
	{
		return _AsHead()->isValid();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint Message::GetMessageLength() const
	{
		return _AsHead()->GetMessageLength();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint Message::GetRawLength() const
	{
		return _AsHead()->GetRawLength();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const byte* Message::GetMessageBuffer() const
	{
		return m_pBuffer + HeadStruct::GetHeadSize();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const byte* Message::GetRawBuffer() const
	{
		return m_pBuffer;
	}
}