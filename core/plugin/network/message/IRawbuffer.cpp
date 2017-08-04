#include "serialize/ISerializeSystem.h"
#include "network/message/IRawbuffer.h"
#include "network/message/IMessage.h"
#include "core/Bufferpool.h"


namespace hc
{
	RTTI_TYPE_IMPLEMENT(IRawbuffer);
	//---------------------------------------------------------------------------------------------------------
	IRawbuffer::IRawbuffer(uint headsize)
		:m_HeadSize(headsize)
		, m_Length(0)
		, m_Index(0)
		, m_Position(0)
		, m_BufferSize(0)
		, m_pBuffer(NULL)
		, m_ErrorCount(0)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IRawbuffer::~IRawbuffer()
	{
		Reset();
	}
	//---------------------------------------------------------------------------------------------------------
	void IRawbuffer::Reset()
	{
		Bufferpool::Instance()->Free(m_BufferSize, m_pBuffer);
		m_BufferSize = 0;
		m_pBuffer = NULL;
		m_ErrorCount = 0;
		m_Position = 0;
		m_Index = 0;
		m_Length = 0;
	}
	//---------------------------------------------------------------------------------------------------------
	bool IRawbuffer::UnpackMessage(IMessage*& msg)
	{
		msg = NULL;
		bool res = false;
		if (_haveValidMessage(m_pBuffer, m_Length, m_Index))
		{
			uint remain = m_Length - m_Index;//计算剩余长度
			if (remain >= m_HeadSize)
			{
				uint rawlen = _GetRawbufferLength(m_pBuffer + m_Index);
				if (remain >= rawlen)
				{
					msg = static_cast<IMessage*>(ISerializeSystem::Instance()
						->GetSerializable(IMessage::RTTI().GetFactor()));
					msg->FromBuffer(m_pBuffer + m_Index, rawlen);
					m_Index += rawlen;
					ASSERT(m_Index <= m_Length);
					res = true;
				}
				else
				{
					_PrepareBuffer(rawlen);// -remain);完整的需要后面-
				}
			}
			return res;
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	bool IRawbuffer::Windup()
	{
		uint remaindata = m_Length - m_Index;
		if (0 == remaindata)//如果没有粘包，则不会有剩余数据，这里归为
		{
			m_Index = 0;
			m_Length = 0;
			m_Position = 0;
		}
		else if(0 != m_Index)//如果粘包则重新整理包
		{
			memmove(m_pBuffer, &m_pBuffer[m_Index], remaindata);
			m_Length = remaindata;
			m_Position = remaindata;
			m_Index = 0;
		}
		return m_ErrorCount < RBC_MAX_ERROR_COUNT;
	}
	//---------------------------------------------------------------------------------------------------------
	bool IRawbuffer::_PrepareBuffer(uint len)
	{
		bool res = true;
		uint allsize = m_Position + len;
		if (m_BufferSize < allsize)
		{
			uint newsize = 0;
			byte* temp = Bufferpool::Instance()->Get(allsize, newsize);
			if (NULL == temp)
			{
				res = false;
				_ErrorOccurred();
			}
			else
			{
				if (m_Position > 0)
				{
					memcpy(temp, m_pBuffer, m_Position);
				}
				Bufferpool::Instance()->Free(m_BufferSize, m_pBuffer);
				m_BufferSize = newsize;
				m_pBuffer = temp;
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	byte* IRawbuffer::NewBuffer(uint len)
	{
		byte* res = NULL;
		if (_PrepareBuffer(len))
		{
			res = &m_pBuffer[m_Position];
			m_Length += len;
			m_Position += len;
		}
		return res;
	}
}