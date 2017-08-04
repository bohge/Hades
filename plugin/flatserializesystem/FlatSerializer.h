#pragma once
#include "serialize/ISerializer.h"


#include "generic_generated.h"

#include <EASTL/vector.h>

namespace hfbs
{
	class FlatEncoder;
	class FlatDecoder;
	class FlatSerializer : public hc::ISerializer
	{
	private:
		typedef eastl::vector<FlatEncoder*> FlatEncoderArray;
		typedef eastl::vector<FlatDecoder*> FlatDecoderArray;
	private:
		flatbuffers::FlatBufferBuilder	m_Builder;
		FlatEncoderArray				m_FlatEncoderArray;
		uint							m_EncoderIndex;
		bool							m_isEncoderFinish;

		FlatDecoderArray				m_FlatDecoderArray;
		uint							m_DecoderIndex;
	public:
		FlatSerializer();
		virtual ~FlatSerializer();
	private:
		void _EncoderFinish();
	public:
		virtual hc::IEncoder* GetEncoder();
		virtual const byte* GetBuffer();
		virtual uint GetBufferSize();
		virtual void ClearEncoder();
		virtual hc::IDecoder* GetDecoderFromBuffer(const byte* buff);
		virtual void ClearDecoder();
	public:
		hc::IDecoder* GetDecoder(const TClass* c);
	};
}