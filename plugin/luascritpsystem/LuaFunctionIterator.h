#pragma once
#include "script/IFunctionIterator.h"


#include <EASTL/vector.h>

struct lua_State;
namespace hlss
{
	class LuaBaseFunctionIterator;
	class LuaFunctionIterator : public hc::IFunctionIterator
	{
	protected:
		int							m_InstanceHandle;
		lua_State**					m_rppLuaState;
		LuaBaseFunctionIterator*	m_rpBaseLuaFunctionIterator;
	public:
		LuaFunctionIterator(int index, lua_State** l);
		virtual ~LuaFunctionIterator();
	public:
		virtual void Being() const;
		virtual bool Next() const;
		virtual hc::DataType GetFieldType() const;
		virtual const char* GetStringField(uint& len) const;
		virtual int GetIntField() const;
		virtual uint64 GetUInt64Field() const;
		virtual bool HaveRenturnValue() const;
		virtual void End() const;
		virtual const IFunctionIterator* GetBaseFunctionIterator() const;
		virtual bool isFunctionExist(const eastl::string& name) const;
	public:
		void InitializeBaseIterator();
		int ReferenceFunction(const eastl::string& name) const;
	};
}
