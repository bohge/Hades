#include "FlatSerializer.h"
#include "FlatEncoder.h"
#include "FlatDecoder.h"


using namespace hc;
namespace hfbs
{
	//---------------------------------------------------------------------------------------------------------
	FlatSerializer::FlatSerializer()
		:m_EncoderIndex(0)
		, m_isEncoderFinish(false)
		, m_DecoderIndex(0)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	FlatSerializer::~FlatSerializer()
	{
		FOR_EACH(FlatEncoderArray, it, m_FlatEncoderArray)
		{
			SAFE_DELETE(*it);
		}
		FOR_EACH(FlatDecoderArray, it, m_FlatDecoderArray)
		{
			SAFE_DELETE(*it);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void FlatSerializer::_EncoderFinish()
	{
		if (!m_isEncoderFinish)
		{
			m_isEncoderFinish = true;
			if (0 != m_EncoderIndex)
			{
				auto members = m_FlatEncoderArray[0]->GetMemberVector();
				auto tclass = CreateTClass(m_Builder, members);
				m_Builder.Finish(tclass);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IEncoder* FlatSerializer::GetEncoder()
	{
		FlatEncoder* encoder = NULL;
		if (m_FlatEncoderArray.size() > m_EncoderIndex)
		{
			encoder = m_FlatEncoderArray[m_EncoderIndex];
		}
		else
		{
			encoder = NEW FlatEncoder(this, &m_Builder);
			m_FlatEncoderArray.push_back(encoder);
		}
		++m_EncoderIndex;
		return encoder;
	}
	//---------------------------------------------------------------------------------------------------------
	const byte* FlatSerializer::GetBuffer()
	{
		_EncoderFinish();
		return m_Builder.GetBufferPointer();
	}
	//---------------------------------------------------------------------------------------------------------
	uint FlatSerializer::GetBufferSize()
	{
		_EncoderFinish();
		return m_Builder.GetSize();
	}
	//---------------------------------------------------------------------------------------------------------
	void FlatSerializer::ClearEncoder()
	{
		m_isEncoderFinish = false;
		m_EncoderIndex = 0;
		m_Builder.Clear();
		FOR_EACH(FlatEncoderArray, it, m_FlatEncoderArray)
		{
			(*it)->Clear();
		}
	}
	//---------------------------------------------------------------------------------------------------------
	IDecoder* FlatSerializer::GetDecoderFromBuffer(const byte* buff)
	{
		auto rpclass = GetTClass(buff);
		IDecoder* res = GetDecoder(rpclass);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IDecoder* FlatSerializer::GetDecoder(const TClass* c)
	{
		FlatDecoder* decoder = NULL;
		if (m_FlatDecoderArray.size() > m_DecoderIndex)
		{
			decoder = m_FlatDecoderArray[m_DecoderIndex];
		}
		else
		{
			decoder = NEW FlatDecoder(this);
			m_FlatDecoderArray.push_back(decoder);
		}
		decoder->Initializer(c);
		++m_DecoderIndex;
		return decoder;
	}
	//---------------------------------------------------------------------------------------------------------
	void FlatSerializer::ClearDecoder()
	{
		m_DecoderIndex = 0;		
		FOR_EACH(FlatDecoderArray, it, m_FlatDecoderArray)
		{
			(*it)->Clear();
		}
	}
}