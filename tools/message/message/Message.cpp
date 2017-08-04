#include "Utility.hpp"
#include "Message.h"




namespace hc
{
	//---------------------------------------------------------------------------------------------------------
	Message::Message()
		:m_pBuffer(NULL)
		, m_Size(0)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	Message::~Message()
	{
		SAFE_DELETE_ARRAY(m_pBuffer);
	}
	//---------------------------------------------------------------------------------------------------------
	void Message::_PrepareBuffer(uint rawlen)
	{
		if (m_Size < rawlen)
		{
			m_Size = GetSize(rawlen);
			SAFE_DELETE_ARRAY(m_pBuffer);
			m_pBuffer = new byte[m_Size];
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void Message::CopyMessage(uint id, const byte* data, uint len)
	{
		uint rawlen = len + HeadStruct::GetHeadSize();
		_PrepareBuffer(rawlen);
		_SetHead(id, rawlen);
		memcpy(m_pBuffer + HeadStruct::GetHeadSize(), data, len);
	}
	//---------------------------------------------------------------------------------------------------------
	byte* Message::PrepareMessage(uint id, uint msglen)
	{
		ASSERT(NULL == m_pBuffer);
		uint rawlen = msglen + HeadStruct::GetHeadSize();
		_PrepareBuffer(rawlen);
		_SetHead(id, rawlen);
		return m_pBuffer + HeadStruct::GetHeadSize();
	}
}