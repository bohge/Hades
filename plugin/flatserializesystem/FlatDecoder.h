#pragma once
#include "core/Nonclearvector.hpp"
#include "serialize/IDecoder.h"

#include "generic_generated.h"

#include <EASTL/vector.h>

namespace hfbs
{
	class FlatSerializer;
	class FlatDecoder : public hc::IDecoder
	{
	private:
		typedef hc::Nonclearvector< const Member* > MemberVector;
		typedef flatbuffers::Vector<flatbuffers::Offset<Member> > Members;
	private:
		MemberVector	m_MemberVector;
		FlatSerializer*	m_rpFlatSerializer;
		uint			m_Size;
	public:
		FlatDecoder(FlatSerializer* ser);
		virtual ~FlatDecoder();
	public:
		virtual uint GetLength() const;
		virtual const char* GetStringField(int index) const;
		virtual int GetIntField(int index) const;
		virtual uint64 GetUInt64Field(int index) const;
		virtual hc::DataType GetKeyType(int index) const;
		virtual hc::DataType GetValueType(int index) const;
		virtual bool AsBool(int index) const;
		virtual short AsShort(int index) const;
		virtual ushort AsUShort(int index) const;
		virtual int AsInt(int index) const;
		virtual uint AsUInt(int index) const;
		virtual float AsFloat(int index) const;
		virtual int64 AsInt64(int index) const;
		virtual uint64 AsUInt64(int index) const;
		virtual double AsDouble(int index) const;
		virtual const char* AsString(int index, uint* out_len) const;
		virtual hc::ISerializable* AsNative(int index) const;
		virtual const hc::IDecoder* AsClass(int index) const;
	public:
		void Initializer(const TClass* c);
		void Clear();
	};
}
