#pragma once
#include "script/ScriptProperty.h"
#include "script/ScriptValue.h"
#include "LuaVirtualMachine.h"
#include "LuaInstance.h"


extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


//得到脚本的返回值
namespace hlss
{
	//-------------------------------------------------------------------------------------------------------
	static int GetScriptRenturnBool(void*& res, LuaVirtualMachine* vm, lua_State* L, int index)
	{
		bool*& lres = (bool*&)res;
		ASSERT(lua_isboolean((lua_State*)L, (int)index));
		*lres = lua_toboolean((lua_State*)L, (int)index);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int GetScriptRenturnShort(void*& res, LuaVirtualMachine* vm, lua_State* L, int index)
	{
		short*& lres = (short*&)res;
		ASSERT(lua_isnumber((lua_State*)L, (int)index));
		*lres = lua_tointeger((lua_State*)L, (int)index);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int GetScriptRenturnUShort(void*& res, LuaVirtualMachine* vm, lua_State* L, int index)
	{
		ushort*& lres = (ushort*&)res;
		if (lua_isnumber((lua_State*)L, (int)index))
		{
			*lres = lua_tointeger((lua_State*)L, (int)index);
		}
		else
		{
			*lres = hc::nullhandle;
		}
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int GetScriptRenturnInt(void*& res, LuaVirtualMachine* vm, lua_State* L, int index)
	{
		int*& lres = (int*&)res;
		ASSERT(lua_isnumber((lua_State*)L, (int)index));
		*lres = lua_tointeger((lua_State*)L, (int)index);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int GetScriptRenturnUInt(void*& res, LuaVirtualMachine* vm, lua_State* L, int index)
	{
		uint*& lres = (uint*&)res;
		if (lua_isnumber((lua_State*)L, (int)index))
		{
			*lres = lua_tonumber((lua_State*)L, (int)index);
		}
		else
		{
			*lres = hc::nullhandle;
		}
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int GetScriptRenturnFloat(void*& res, LuaVirtualMachine* vm, lua_State* L, int index)
	{
		float*& lres = (float*&)res;
		ASSERT(lua_isnumber((lua_State*)L, (int)index));
		*lres = lua_tonumber((lua_State*)L, (int)index);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int GetScriptRenturnDouble(void*& res, LuaVirtualMachine* vm, lua_State* L, int index)
	{
		double*& lres = (double*&)res;
		ASSERT(lua_isnumber((lua_State*)L, (int)index));
		*lres = lua_tonumber((lua_State*)L, (int)index);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int GetScriptRenturnInt64(void*& res, LuaVirtualMachine* vm, lua_State* L, int index)
	{
		int64*& lres = (int64*&)res;
		ASSERT(LUA_TCDATA == lua_type((lua_State*)L, (int)index));
		*lres = *(int64*)luaL_tocdata((lua_State*)L, (int)index);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int GetScriptRenturnUInt64(void*& res, LuaVirtualMachine* vm, lua_State* L, int index)
	{
		uint64*& lres = (uint64*&)res;
		if (LUA_TCDATA == lua_type((lua_State*)L, (int)index))
		{
			ASSERT(MY_CTID_UINT64 == luaL_cdatatype((lua_State*)L, (int)index));
			*lres = *(uint64*)luaL_tocdata((lua_State*)L, (int)index);
		}
		else
		{
			*lres = hc::nullid;
		}
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int GetScriptRenturnString(void*& res, LuaVirtualMachine* vm, lua_State* L, int index)
	{
		const char*& lres = (const char*&)res;
		ASSERT(lua_isstring((lua_State*)L, (int)index));
		lres = lua_tostring((lua_State*)L, (int)index);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int GetScriptRenturnClass(void*& res, LuaVirtualMachine* vm, lua_State* L, int index)
	{
		hc::Object*& lres = (hc::Object*&)res;
		switch (lua_type(L, index))
		{
		case LUA_TUSERDATA:
			lres = ((hc::ScriptValue*)lua_touserdata((lua_State*)L, (int)index))->GetObject();
			break;
		case LUA_TNIL:
			lres = NULL;
			break;
		default:ASSERT(false && "just nil and usedata");
		}
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int GetScriptRenturnArguments(void*& res, LuaVirtualMachine* vm, lua_State* L, int index)
	{
		int count = lua_gettop(L);
		LuaArguments*& args = (LuaArguments*&)res;
		args->SetLuaState(L);
		args->SetArgumentCount(count);
		args->SetArgumentOffset(-count);
		args->Serialize();
		return count;
	}
}