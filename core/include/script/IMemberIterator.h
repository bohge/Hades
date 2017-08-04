#pragma once
#include "core/Object.hpp"
#include "core/Types.h"


namespace hc
{
	//该类的Begin next end函数需要完整调用，不会然导致虚拟机出错
	class HADES_CORE_API IMemberIterator : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IMemberIterator);
	public:
		IMemberIterator(void);
		virtual ~IMemberIterator(void);
	public:
		virtual void Being() const = 0;
		virtual bool Next() const = 0;
		virtual const char* GetStringField(uint* len) const = 0;
		virtual int GetIntField() const = 0;
		virtual uint64 GetUInt64Field() const = 0;
		virtual DataType GetKeyType() const = 0;
		virtual DataType GetValueType() const = 0;
		virtual bool AsBool() const = 0;
		virtual short AsShort() const = 0;
		virtual ushort AsUShort() const = 0;
		virtual int AsInt() const = 0;
		virtual uint AsUInt() const = 0;
		virtual float AsFloat() const = 0;
		virtual double AsDouble() const = 0;
		virtual int64 AsInt64() const = 0;
		virtual uint64 AsUInt64() const = 0;
		virtual const char* AsString(uint* len) const = 0;
		virtual const Object* AsNative() const = 0;
		virtual const IMemberIterator& AsClass() const = 0;
		virtual void End() const = 0;
	};
}
