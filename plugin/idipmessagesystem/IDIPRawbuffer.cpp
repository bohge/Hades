#include "serialize/ISerializeSystem.h"
#include "IDIPHeadStruct.h"
#include "IDIPRawbuffer.h"
#include "IDIPMessage.h"

using namespace hc;
namespace hms
{
	//---------------------------------------------------------------------------------------------------------
	IDIPRawbuffer::IDIPRawbuffer()
		:IRawbuffer(IDIPHeadStruct::GetHeadSize())
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IDIPRawbuffer::~IDIPRawbuffer()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	bool IDIPRawbuffer::_haveValidMessage(byte* buffer, uint& length, uint& index)
	{
		ASSERT(index <= length);
		if (length - index > IDIPHeadStruct::GetHeadSize())
		{
			return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	uint IDIPRawbuffer::_GetRawbufferLength(byte* buffer)
	{
		IDIPHeadStruct* head;
		head = reinterpret_cast<IDIPHeadStruct*>(buffer);
		return head->GetRawLength();
	}

}