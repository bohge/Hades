#include "serialize/ISerializeSystem.h"
#include "HadesHeadStruct.h"
#include "HadesRawbuffer.h"
#include "HadesMessage.h"

using namespace hc;
namespace hms
{
	//---------------------------------------------------------------------------------------------------------
	HadesRawbuffer::HadesRawbuffer()
		:IRawbuffer(HeadStruct::GetHeadSize())
	{
	}
	//---------------------------------------------------------------------------------------------------------
	HadesRawbuffer::~HadesRawbuffer()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	bool HadesRawbuffer::_FindNextMessage(byte* buffer, uint& length, uint& index)
	{
		uint fourcc = 0;
		while (true)
		{
			fourcc = (reinterpret_cast<uint*>(&buffer[index]))[0];
			if (FOURCC_NETPACKAGE_HEAD == fourcc
				|| index > length - sizeof(uint))
			{
				return FOURCC_NETPACKAGE_HEAD == fourcc;
			}
			else
			{
				++index;
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	bool HadesRawbuffer::_haveValidMessage(byte* buffer, uint& length, uint& index)
	{
		ASSERT(index <= length);
		ASSERT(4 == sizeof(FOURCC_NETPACKAGE_HEAD));
		if (length - index > sizeof(uint))
		{
			uint fourcc = (reinterpret_cast<uint*>(&buffer[index]))[0];
			if (FOURCC_NETPACKAGE_HEAD != fourcc)
			{
				//处理错误消息
				if (_FindNextMessage(buffer, length, index))
				{
					return true;
				}
				else
				{
					_ErrorOccurred();
				}
			}
			else
			{
				return true;
			}
		}
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	uint HadesRawbuffer::_GetRawbufferLength(byte* buffer)
	{
		HeadStruct* head;
		head = reinterpret_cast<HeadStruct*>(buffer);
		return head->GetRawLength();
	}
}