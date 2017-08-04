#pragma once
#include "core/Types.h"


#include <EASTL/string.h>

namespace hc
{
	//这个类目前的主要涉及目的是为了提供给脚本去序列化成员传输至keeper的
	//所以这个类内的name全部用c的形式表示
	class ISerializable;
	class HADES_CORE_API IDecoder : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IDecoder);
	public:
		IDecoder(void);
		virtual ~IDecoder(void);
	public:
		virtual uint GetLength() const = 0;
		virtual const char* GetStringField(int index) const = 0;
		virtual int GetIntField(int index) const = 0;
		virtual uint64 GetUInt64Field(int index) const = 0;
		virtual DataType GetKeyType(int index) const = 0;
		virtual DataType GetValueType(int index) const = 0;
		virtual bool AsBool(int index) const = 0;
		virtual short AsShort(int index) const = 0;
		virtual ushort AsUShort(int index) const = 0;
		virtual int AsInt(int index) const = 0;
		virtual uint AsUInt(int index) const = 0;
		virtual float AsFloat(int index) const = 0;
		virtual int64 AsInt64(int index) const = 0;
		virtual uint64 AsUInt64(int index) const = 0;
		virtual double AsDouble(int index) const = 0;
		virtual const char* AsString(int index, uint* out_len) const = 0;
		virtual ISerializable* AsNative(int index) const = 0;
		virtual const IDecoder* AsClass(int index) const = 0;
	};
}
