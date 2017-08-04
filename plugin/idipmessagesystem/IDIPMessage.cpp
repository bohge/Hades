#include "IDIPMessage.h"



namespace hms
{
	RTTI_TYPE_IMPLEMENT(IDIPMessage);
	//---------------------------------------------------------------------------------------------------------
	IDIPMessage::IDIPMessage()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IDIPMessage::IDIPMessage(const IDIPMessage& rhs)
		:IMessage(rhs)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IDIPMessage::~IDIPMessage()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void IDIPMessage::_SetHead(uint id, uint rawlen)
	{
		IDIPHeadStruct* head = reinterpret_cast<IDIPHeadStruct*>(_GetRawBuffer());
		head->SetID(id);
		head->SetRawLength(rawlen);
	}
	//---------------------------------------------------------------------------------------------------------
	uint IDIPMessage::GetID() const
	{
		return _AsHead()->GetID();
	}
	//---------------------------------------------------------------------------------------------------------
	bool IDIPMessage::isValid() const
	{
		return NULL != GetRawBuffer() && _AsHead()->isValid();
	}
	//---------------------------------------------------------------------------------------------------------
	uint IDIPMessage::GetMessageLength() const
	{
		return _AsHead()->GetMessageLength();
	}
	//---------------------------------------------------------------------------------------------------------
	uint IDIPMessage::GetRawLength() const
	{
		return _AsHead()->GetRawLength();
	}
	//---------------------------------------------------------------------------------------------------------
	const byte* IDIPMessage::GetMessageBuffer() const
	{
		return GetRawBuffer() + IDIPHeadStruct::GetHeadSize();
	}
	//---------------------------------------------------------------------------------------------------------
	uint IDIPMessage::_GetHeadSize() const
	{
		return IDIPHeadStruct::GetHeadSize();
	}
}