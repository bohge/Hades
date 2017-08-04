#pragma once
#include "script/ScriptValue.h"
#include "serialize/IEncoder.h"
#include "serialize/IDecoder.h"

#include <EASTL/string.h>

namespace hc
{
	class ClassRegister;
	class HADES_CORE_API ISerializable : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(ISerializable);
	public:
		ISerializable();
		virtual ~ISerializable(void);
	public:
		virtual uint GetFactor() const = 0;
		virtual const byte* GetBuffer() const = 0;
		virtual uint GetLength() const = 0;
		virtual bool FromBuffer(const byte* buff, uint len) = 0;
		virtual void OnRecycle();
		virtual ScriptValue::ClearFunction GetClearFunction();
	};


	#define SERIALIZABL_TYPE_DECLEAR(type)\
		public:\
		virtual uint GetFactor() const\
		{\
			return type::s_Rtti.GetFactor();\
		}
}