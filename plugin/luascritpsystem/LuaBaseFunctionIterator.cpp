#include "LuaBaseFunctionIterator.h"
#include "script/ScriptProperty.h"
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
	LuaBaseFunctionIterator::LuaBaseFunctionIterator(int index, lua_State** l)
		:LuaFunctionIterator(index, l)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LuaBaseFunctionIterator::~LuaBaseFunctionIterator()
	{
		if (*m_rppLuaState)
		{
			luaL_unref(*m_rppLuaState, LUA_REGISTRYINDEX, m_InstanceHandle);
		}
	}
}
