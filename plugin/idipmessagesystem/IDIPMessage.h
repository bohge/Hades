#pragma once
#include "network/message/IMessage.h"
#include "IDIPHeadStruct.h"



namespace hms
{
	class IDIPMessage : public hc::IMessage
	{
		RTTI_DRIVER_TYPE_DECLEAR(IDIPMessage, IMessage);
	private:
		IDIPMessage(const IDIPMessage& rhs);
	public:
		IDIPMessage();
		virtual ~IDIPMessage();
	private:
		virtual void _SetHead(uint id, uint rawlen);
		virtual uint _GetHeadSize() const;
	public:
		virtual uint GetID() const;
		virtual bool isValid() const;
		virtual uint GetRawLength() const;
		virtual uint GetMessageLength() const;
		virtual const byte* GetMessageBuffer() const;
	private:
		HADES_FORCEINLINE const IDIPHeadStruct* _AsHead() const;
	public:
		HADES_FORCEINLINE int32_t GetResult() const;
	};



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const IDIPHeadStruct* IDIPMessage::_AsHead() const
	{
		return reinterpret_cast<const IDIPHeadStruct*>(GetRawBuffer());
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE int32_t IDIPMessage::GetResult() const
	{
		return _AsHead()->GetResult();
	}
}