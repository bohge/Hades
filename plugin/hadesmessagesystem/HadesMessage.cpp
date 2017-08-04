#include "HadesMessage.h"


namespace hms
{
	RTTI_TYPE_IMPLEMENT(HadesMessage);
	//---------------------------------------------------------------------------------------------------------
	HadesMessage::HadesMessage()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	HadesMessage::HadesMessage(const HadesMessage& rhs)
		:IMessage(rhs)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	HadesMessage::~HadesMessage()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void HadesMessage::_SetHead(uint id, uint rawlen)
	{
		HeadStruct* head = reinterpret_cast<HeadStruct*>(_GetRawBuffer());
		head->Fourcc = FOURCC_NETPACKAGE_HEAD;
		head->ID = id;
		head->RawLength = rawlen;
	}
	//---------------------------------------------------------------------------------------------------------
	uint HadesMessage::GetID() const
	{
		return _AsHead()->GetID();
	}
	//---------------------------------------------------------------------------------------------------------
	bool HadesMessage::isValid() const
	{
		return NULL != GetRawBuffer() && _AsHead()->isValid();
	}
	//---------------------------------------------------------------------------------------------------------
	uint HadesMessage::GetMessageLength() const
	{
		return _AsHead()->GetMessageLength();
	}
	//---------------------------------------------------------------------------------------------------------
	uint HadesMessage::GetRawLength() const
	{
		return _AsHead()->GetRawLength();
	}
	//---------------------------------------------------------------------------------------------------------
	const byte* HadesMessage::GetMessageBuffer() const
	{
		return GetRawBuffer() + HeadStruct::GetHeadSize();
	}
	//---------------------------------------------------------------------------------------------------------
	uint HadesMessage::_GetHeadSize() const
	{
		return HeadStruct::GetHeadSize();
	}
}