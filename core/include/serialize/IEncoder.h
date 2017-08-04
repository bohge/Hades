#pragma once
#include "core/Object.hpp"

#include <EASTL/string.h>

namespace hc
{
	//这个类目前的主要涉及目的是为了提供给脚本去序列化成员传输至keeper的
	//所以这个类内的name全部用c的形式表示
	class ISerializable;
	class HADES_CORE_API IEncoder : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IEncoder);
	public:
		IEncoder(void);
		virtual ~IEncoder(void);
	public:
		virtual IEncoder* GetChild() = 0;
		virtual void SetField(const char* name, uint nlen) = 0;//null变量
		virtual void SetField(const char* name, uint nlen, bool v) = 0;
		virtual void SetField(const char* name, uint nlen, short v) = 0;
		virtual void SetField(const char* name, uint nlen, ushort v) = 0;
		virtual void SetField(const char* name, uint nlen, int v) = 0;
		virtual void SetField(const char* name, uint nlen, uint v) = 0;
		virtual void SetField(const char* name, uint nlen, float v) = 0;
		virtual void SetField(const char* name, uint nlen, int64 v) = 0;
		virtual void SetField(const char* name, uint nlen, uint64 v) = 0;
		virtual void SetField(const char* name, uint nlen, double v) = 0;
		virtual void SetField(const char* name, uint nlen, const char* v, uint len) = 0;
		virtual void SetField(const char* name, uint nlen, const IEncoder* v) = 0;
		virtual void SetField(const char* name, uint nlen, const ISerializable* v) = 0;
		virtual void SetField(int index) = 0;//null变量
		virtual void SetField(int index, bool v) = 0;
		virtual void SetField(int index, short v) = 0;
		virtual void SetField(int index, ushort v) = 0;
		virtual void SetField(int index, int v) = 0;
		virtual void SetField(int index, uint v) = 0;
		virtual void SetField(int index, float v) = 0;
		virtual void SetField(int index, int64 v) = 0;
		virtual void SetField(int index, uint64 v) = 0;
		virtual void SetField(int index, double v) = 0;
		virtual void SetField(int index, const char* v, uint len) = 0;
		virtual void SetField(int index, const IEncoder* v) = 0;
		virtual void SetField(int index, const ISerializable* v) = 0;
		virtual void SetField(uint64 index) = 0;//null变量
		virtual void SetField(uint64 index, bool v) = 0;
		virtual void SetField(uint64 index, short v) = 0;
		virtual void SetField(uint64 index, ushort v) = 0;
		virtual void SetField(uint64 index, int v) = 0;
		virtual void SetField(uint64 index, uint v) = 0;
		virtual void SetField(uint64 index, float v) = 0;
		virtual void SetField(uint64 index, int64 v) = 0;
		virtual void SetField(uint64 index, uint64 v) = 0;
		virtual void SetField(uint64 index, double v) = 0;
		virtual void SetField(uint64 index, const char* v, uint len) = 0;
		virtual void SetField(uint64 index, const IEncoder* v) = 0;
		virtual void SetField(uint64 index, const ISerializable* v) = 0;
	};
}
