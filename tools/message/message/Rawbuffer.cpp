#include "Rawbuffer.h"
#include "Utility.hpp"
#include "Message.h"


namespace hc
{
	//---------------------------------------------------------------------------------------------------------
	Rawbuffer::Rawbuffer()
		:m_Length(0)
		, m_Index(0)
		, m_Position(0)
		, m_BufferSize(0)
		, m_pBuffer(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	Rawbuffer::~Rawbuffer()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	bool Rawbuffer::_FindNextMessage()
	{
		uint fourcc = 0;
		while (true)
		{
			fourcc = (reinterpret_cast<uint*>(&m_pBuffer[m_Index]))[0];
			if (Configure::FOURCC_NETPACKAGE_HEAD == fourcc
				|| m_Index > m_Length - sizeof(uint))
			{
				return Configure::FOURCC_NETPACKAGE_HEAD == fourcc;
			}
			else
			{
				++m_Index;
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	bool Rawbuffer::_DoUnpackMessage(Message* msg)
	{
		bool res = false;
		uint remain = m_Length - m_Index;//计算剩余长度
		if (remain >= HeadStruct::GetHeadSize())
		{
			HeadStruct* head;
			//memcpy(&head, &m_pBuffer[m_Index], HeadStruct::GetHeadSize());
			head = (HeadStruct*)&m_pBuffer[m_Index];
			uint msglen = head->GetMessageLength();
			remain -= HeadStruct::GetHeadSize();
			if (remain >= msglen)
			{
				m_Index += HeadStruct::GetHeadSize();
				msg->CopyMessage(head->GetID(), &m_pBuffer[m_Index], msglen);
				m_Index += msglen;
				ASSERT(m_Index <= m_Length);
				res = true;
			}
			else
			{
				PrepareBuffer(msglen);// -remain);完整的需要后面-
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	bool Rawbuffer::UnpackMessage(Message* msg)
	{
		bool res = false;
		bool findres = true;
		ASSERT(m_Index <= m_Length);
		ASSERT(4 == sizeof(Configure::FOURCC_NETPACKAGE_HEAD));
		if (m_Length - m_Index > sizeof(uint))
		{
			uint fourcc = (reinterpret_cast<uint*>(&m_pBuffer[m_Index]))[0];
			if (Configure::FOURCC_NETPACKAGE_HEAD != fourcc)
			{
				//处理错误消息
				findres = _FindNextMessage();
			}
			if (!findres)//没找到
			{
				m_Index = 0;
				m_Length = 0;
				m_Position = 0;
			}
			else
			{
				res = _DoUnpackMessage(msg);
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void Rawbuffer::Cleanup()
	{
		uint remaindata = m_Length - m_Index;
		if (0 == remaindata)//如果没有粘包，则不会有剩余数据，这里归为
		{
			m_Index = 0;
			m_Length = 0;
			m_Position = 0;
		}
		else if (0 != m_Index)//如果粘包则重新整理包
		{
			memmove(m_pBuffer, &m_pBuffer[m_Index], remaindata);
			m_Length = remaindata;
			m_Position = remaindata;
			m_Index = 0;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void Rawbuffer::PrepareBuffer(uint len)
	{
		uint allsize = m_Position + len;
		if (m_BufferSize < allsize)
		{
			uint newsize = GetSize(allsize);
			byte* temp = new byte[newsize];
			if (m_Position > 0)
			{
				memcpy(temp, m_pBuffer, m_Position);
			}
			SAFE_DELETE_ARRAY(m_pBuffer);
			m_BufferSize = newsize;
			m_pBuffer = temp;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	byte* Rawbuffer::NewBuffer(uint len)
	{
		PrepareBuffer(len);
		byte* res = &m_pBuffer[m_Position];
		m_Length += len;
		m_Position += len;
		return res;
	}
}