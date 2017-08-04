#include "serialize/FactorConfigure.h"
#include "network/message/IMessage.h"
#include "core/Bufferpool.h"
#include "log/ILogSystem.h"


namespace hc
{
	RTTI_TYPE_IMPLEMENT_WITH_FACTOR(IMessage, FACTOR_MESSAGE);
	//---------------------------------------------------------------------------------------------------------
	IMessage::IMessage()
		:m_pBuffer(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IMessage::IMessage(const IMessage& rhs)
		: m_pBuffer(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IMessage::~IMessage()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	const byte* IMessage::GetBuffer() const
	{
		return m_pBuffer;
	}
	//---------------------------------------------------------------------------------------------------------
	uint IMessage::GetLength() const
	{
		return GetRawLength();
	}
	//---------------------------------------------------------------------------------------------------------
	bool IMessage::FromBuffer(const byte* buff, uint rawlen)
	{
		ASSERT(NULL == m_pBuffer);
		uint real = 0;
		m_pBuffer = Bufferpool::Instance()->Get(rawlen, real);
		memcpy(m_pBuffer, buff, rawlen);
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	void IMessage::OnRecycle()
	{
		if (m_pBuffer)
		{
			Bufferpool::Instance()->Free(GetRawLength(), m_pBuffer);
			m_pBuffer = NULL;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	byte* IMessage::PrepareMessage(uint id, uint msglen)
	{
		ASSERT(NULL == m_pBuffer);
		uint real = 0;
		uint rawlen = msglen + _GetHeadSize();
		m_pBuffer = Bufferpool::Instance()->Get(rawlen, real);
		byte* res = NULL == m_pBuffer ? NULL : m_pBuffer + _GetHeadSize();
		_SetHead(id, rawlen);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	byte* IMessage::_DeliverBufferControl()
	{
		byte* temp = m_pBuffer;//此处不能回收，应为buffer已经交给别人
		m_pBuffer = NULL;
		return temp;
	}
	//---------------------------------------------------------------------------------------------------------
	const IMessage& IMessage::operator=(const IMessage& rhs)
	{
		HADESERROR("there is no = operator which accept const value");
		return *this;
	}
	//---------------------------------------------------------------------------------------------------------
	const IMessage& IMessage::operator=(IMessage& rhs)
	{
		//首先回收旧buffer
		if (m_pBuffer)
		{
			Bufferpool::Instance()->Free(GetRawLength(), m_pBuffer);
		}
		m_pBuffer = rhs._DeliverBufferControl();
		return *this;
	}
}