#pragma once
#include "core/Object.hpp"


#include <EASTL/string.h>

namespace hc
{
	class IEncoder;
	class IDecoder;
	class HADES_CORE_API ISerializer : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(ISerializer);
	public:
		ISerializer();
		virtual ~ISerializer(void);
	public:
		//序列化
		virtual IEncoder* GetEncoder() = 0;
		virtual const byte* GetBuffer() = 0;
		virtual uint GetBufferSize() = 0;
		virtual void ClearEncoder() = 0;
		//反序列化
		virtual IDecoder* GetDecoderFromBuffer(const byte* buff) = 0;
		virtual void ClearDecoder() = 0;
	};
}