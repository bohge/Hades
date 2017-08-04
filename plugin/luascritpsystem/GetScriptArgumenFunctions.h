#pragma once
#include "script/ScriptProperty.h"
#include "script/ScriptValue.h"
#include "LuaVirtualMachine.h"
#include "LuaArguments.h"
#include "LuaInstance.h"


extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


//得到脚本传递进来的参数
namespace hlss
{
	//-------------------------------------------------------------------------------------------------------
	static void GetScriptParameterBool(void*& res, void* vm, void* L, void* index)
	{
		bool*& lres = (bool*&)res;
		ASSERT(lua_isboolean((lua_State*)L, (int)index));
		if (!lua_isboolean((lua_State*)L, (int)index))
		{
			luaL_error((lua_State*)L, "not a boolean");
		}
		*lres = lua_toboolean((lua_State*)L, (int)index);
	}
	//-------------------------------------------------------------------------------------------------------
	static void GetScriptParameterShort(void*& res, void* vm, void* L, void* index)
	{
		short*& lres = (short*&)res;
		if (!lua_isnumber((lua_State*)L, (int)index))
		{
			luaL_error((lua_State*)L, "not a number");
		}
		*lres = lua_tointeger((lua_State*)L, (int)index);
	}
	//-------------------------------------------------------------------------------------------------------
	static void GetScriptParameterUShort(void*& res, void* vm, void* L, void* index)
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
	}
	//-------------------------------------------------------------------------------------------------------
	static void GetScriptParameterInt(void*& res, void* vm, void* L, void* index)
	{
		int*& lres = (int*&)res;
		if (!lua_isnumber((lua_State*)L, (int)index))
		{
			luaL_error((lua_State*)L, "not a number");
		}
		*lres = lua_tointeger((lua_State*)L, (int)index);
	}
	//-------------------------------------------------------------------------------------------------------
	static void GetScriptParameterUInt(void*& res, void* vm, void* L, void* index)
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
	}
	//-------------------------------------------------------------------------------------------------------
	static void GetScriptParameterFloat(void*& res, void* vm, void* L, void* index)
	{
		float*& lres = (float*&)res;
		if (!lua_isnumber((lua_State*)L, (int)index))
		{
			luaL_error((lua_State*)L, "not a number");
		}
		*lres = lua_tonumber((lua_State*)L, (int)index);
	}
	//-------------------------------------------------------------------------------------------------------
	static void GetScriptParameterDouble(void*& res, void* vm, void* L, void* index)
	{
		double*& lres = (double*&)res;
		//ASSERT(lua_isnumber((lua_State*)L, (int)index));
		if (!lua_isnumber((lua_State*)L, (int)index))
		{
			luaL_error((lua_State*)L, "not a number");
		}
		*lres = lua_tonumber((lua_State*)L, (int)index);
	}
	//-------------------------------------------------------------------------------------------------------
	static void GetScriptParameterBuffer(void*& res, void* vm, void* L, void* index)
	{
		hc::StringBuffer*& lres = (hc::StringBuffer*&)res;
		//ASSERT(lua_isstring((lua_State*)L, (int)index));
		if (!lua_isstring((lua_State*)L, (int)index))
		{
			luaL_error((lua_State*)L, "not a buffer");
		}
		size_t len;
		lres->pBuffer = lua_tolstring((lua_State*)L, (int)index, &len);
		lres->Length = len;
	}
	//-------------------------------------------------------------------------------------------------------
	static void GetScriptParameterInt64(void*& res, void* vm, void* L, void* index)
	{
		int64*& lres = (int64*&)res;
		ASSERT(LUA_TCDATA == lua_type((lua_State*)L, (int)index));
		ASSERT(MY_CTID_INT64 == luaL_cdatatype((lua_State*)L, (int)index));
		*lres = *(int64*)luaL_tocdata((lua_State*)L, (int)index);
	}
	//-------------------------------------------------------------------------------------------------------
	static void GetScriptParameterUInt64(void*& res, void* vm, void* L, void* index)
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
	}
	//-------------------------------------------------------------------------------------------------------
	static void GetScriptParameterString(void*& res, void* vm, void* L, void* index)
	{
		hc::StringBuffer*& lres = (hc::StringBuffer*&)res;
		//ASSERT(lua_isstring((lua_State*)L, (int)index));
		if (!lua_isstring((lua_State*)L, (int)index))
		{
			luaL_error((lua_State*)L, "not a string");
		}
		size_t len;
		lres->pBuffer = lua_tolstring((lua_State*)L, (int)index, &len);
		lres->Length = len;
	}
	//-------------------------------------------------------------------------------------------------------
	static void GetScriptParameterFunction(void*& res, void* vm, void* L, void* index)
	{
		int*& lres = (int*&)res;
		ASSERT(lua_isfunction((lua_State*)L, (int)index));
		lua_pushvalue((lua_State*)L, (int)index);
		*lres = luaL_ref((lua_State*)L, LUA_REGISTRYINDEX);
	}
	//-------------------------------------------------------------------------------------------------------
	static void GetScriptParameterInstance(void*& res, void* vm, void* L, void* index)
	{
		int*& lres = (int*&)res;
		DEBUGCODE(int t = lua_gettop((lua_State*)L));
		ASSERT(lua_istable((lua_State*)L, (int)index));
		lua_pushvalue((lua_State*)L, (int)index);
		ASSERT(lua_istable((lua_State*)L, -1));
		*lres = luaL_ref((lua_State*)L, LUA_REGISTRYINDEX);
		ASSERT(lua_gettop((lua_State*)L) == t);
	}
	//-------------------------------------------------------------------------------------------------------
	static void GetScriptParameterClass(void*& res, void* vm, void* L, void* index)
	{
		if (lua_isuserdata((lua_State*)L, (int)index))
		{
			res = ((hc::ScriptValue*)lua_touserdata((lua_State*)L, (int)index))->GetObject();
		}
		else
		{
			res = NULL;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	static void GetScriptParameterArgumens(void*& res, void* vm, void* L, void* index)
	{
		LuaArguments*& args = (LuaArguments*&)res;
		int argucount = lua_gettop((lua_State*)L) - 1;//去掉self
		ASSERT(argucount >= 0);
		args->SetArgumentCount(argucount);
		args->SetLuaState((lua_State*)L);
		args->SetArgumentOffset(2);//去掉self,起始点从1开始
		args->Serialize();
	}
}