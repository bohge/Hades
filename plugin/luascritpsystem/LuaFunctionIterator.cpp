#include "LuaBaseFunctionIterator.h"
#include "script/ScriptProperty.h"
#include "LuaFunctionIterator.h"
#include "script/ScriptValue.h"


extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

struct lua_State;
using namespace hc;
namespace hlss
{
	//---------------------------------------------------------------------------------------------------------
	LuaFunctionIterator::LuaFunctionIterator(int index, lua_State** l)
		:m_rppLuaState(l)
		, m_InstanceHandle(index)
		, m_rpBaseLuaFunctionIterator(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LuaFunctionIterator::~LuaFunctionIterator()
	{
		SAFE_DELETE(m_rpBaseLuaFunctionIterator);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaFunctionIterator::Being() const
	{
		lua_rawgeti(*m_rppLuaState, LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushnil(*m_rppLuaState);
	}
	//---------------------------------------------------------------------------------------------------------
	bool LuaFunctionIterator::Next() const
	{
		bool isnext;
		bool iscontinue;
		do 
		{
			iscontinue = false;
			isnext = lua_next(*m_rppLuaState, -2) != 0;
			if (isnext)
			{
				int valuetype = lua_type(*m_rppLuaState, -1);
				const char* name = lua_tostring(*m_rppLuaState, -2);
				if (LUA_TFUNCTION != valuetype
					|| NULL == name 
					|| name[0] == ScriptProperty::SCRIPT_INERL_PREFIX
					)
				{
					iscontinue = true;
					lua_pop(*m_rppLuaState, 1);
				}
			}
		} while (iscontinue && isnext);		
		return isnext;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::DataType LuaFunctionIterator::GetFieldType() const
	{
		DataType res = DT_NULL;
		switch (lua_type(*m_rppLuaState, -2))
		{
		case LUA_TNUMBER: res = DT_INT; break;
		case LUA_TSTRING: res = DT_STRING; break;
		case LUA_TCDATA:
		{
						   switch (luaL_cdatatype(*m_rppLuaState, -2))
						   {
						   case MY_CTID_INT64: res = DT_INT64; break;
						   case MY_CTID_UINT64: res = DT_UINT64; break;
						   default:ASSERT(false);
						   }
						   break;
		}
		default:ASSERT(false);
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	const char* LuaFunctionIterator::GetStringField(uint& len) const
	{
		size_t slen;
		const char* pstr = lua_tolstring(*m_rppLuaState, -2, &slen);
		len = slen;
		return pstr;
	}
	//---------------------------------------------------------------------------------------------------------
	int LuaFunctionIterator::GetIntField() const
	{
		return lua_tointeger(*m_rppLuaState, -2);
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 LuaFunctionIterator::GetUInt64Field() const
	{
		return*(uint64*)luaL_tocdata(*m_rppLuaState, -2);
	}
	//---------------------------------------------------------------------------------------------------------
	bool LuaFunctionIterator::HaveRenturnValue() const
	{
		ASSERT(lua_isfunction(*m_rppLuaState, -1));
		bool res = LuaL_havereturn(*m_rppLuaState);
		lua_pop(*m_rppLuaState, 1);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaFunctionIterator::End() const
	{
		lua_pop(*m_rppLuaState, 1);
	}
	//---------------------------------------------------------------------------------------------------------
	const IFunctionIterator* LuaFunctionIterator::GetBaseFunctionIterator() const
	{
		return m_rpBaseLuaFunctionIterator;
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaFunctionIterator::InitializeBaseIterator()
	{
		DEBUGCODE(int t = lua_gettop(*m_rppLuaState));
		lua_rawgeti(*m_rppLuaState, LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		if (1 == lua_getmetatable(*m_rppLuaState, -1))
		{
			int ref = luaL_ref(*m_rppLuaState, LUA_REGISTRYINDEX);//将元表绑定到索引列表
			m_rpBaseLuaFunctionIterator = NEW LuaBaseFunctionIterator(ref, m_rppLuaState);
			m_rpBaseLuaFunctionIterator->InitializeBaseIterator();
		}
		lua_pop(*m_rppLuaState, 1);
		ASSERT(t == lua_gettop(*m_rppLuaState));
	}
	//---------------------------------------------------------------------------------------------------------
	bool LuaFunctionIterator::isFunctionExist(const eastl::string& name) const
	{
		DEBUGCODE(int t = lua_gettop(*m_rppLuaState));
		lua_rawgeti(*m_rppLuaState, LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_getfield(*m_rppLuaState, -1, name.c_str());
		bool res = lua_isfunction(*m_rppLuaState, -1);
		lua_pop(*m_rppLuaState, 2);
		ASSERT(t == lua_gettop(*m_rppLuaState));
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	int LuaFunctionIterator::ReferenceFunction(const eastl::string& name) const
	{
		DEBUGCODE(int t = lua_gettop(*m_rppLuaState));
		lua_rawgeti(*m_rppLuaState, LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_getfield(*m_rppLuaState, -1, name.c_str());
		ASSERT(lua_isfunction(*m_rppLuaState, -1));
		int ref = luaL_ref(*m_rppLuaState, LUA_REGISTRYINDEX);//将函数绑定到索引列表
		lua_pop(*m_rppLuaState, 1);
		ASSERT(t == lua_gettop(*m_rppLuaState));
		return ref;
	}
}
