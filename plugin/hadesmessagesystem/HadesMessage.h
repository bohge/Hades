#pragma once
#include "network/message/IMessage.h"
#include "HadesHeadStruct.h"



namespace hms
{
	class HadesMessage : public hc::IMessage
	{
		RTTI_DRIVER_TYPE_DECLEAR(HadesMessage, IMessage);
	private:
		HadesMessage(const HadesMessage& rhs);
	public:
		HadesMessage();
		virtual ~HadesMessage();
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
		HADES_FORCEINLINE const HeadStruct* _AsHead() const;
	};



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const HeadStruct* HadesMessage::_AsHead() const
	{
		return reinterpret_cast<const HeadStruct*>(GetRawBuffer());
	}
}