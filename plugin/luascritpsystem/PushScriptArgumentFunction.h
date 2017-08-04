#pragma once
#include "script/IScriptInstance.h"
#include "script/ScriptProperty.h"
#include "script/ScriptValue.h"
#include "NativePushHelper.hpp"
#include "LuaArguments.h"
#include "core/Types.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


//脚本函数调用时，传递给脚本的参数
namespace hlss
{
	//-------------------------------------------------------------------------------------------------------
	static int PushScriptParameterShort(LuaVirtualMachine* vm, lua_State* l, void* data)
	{
		const short* n = static_cast<const short*>(data);
		lua_pushinteger(l, *n);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int PushScriptParameterUShort(LuaVirtualMachine* vm, lua_State* l, void* data)
	{
		const ushort* n = static_cast<const ushort*>(data);
		if (ushort(hc::nullhandle) == *n)
		{
			lua_pushnil(l);
		}
		else
		{
			lua_pushinteger(l, *n);
		}
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int PushScriptParameterInt(LuaVirtualMachine* vm, lua_State* l, void* data)
	{
		const int* n = static_cast<const int*>(data);
		lua_pushinteger(l, *n);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int PushScriptParameterUInt(LuaVirtualMachine* vm, lua_State* l, void* data)
	{
		const uint* n = static_cast<const uint*>(data);
		if ( hc::nullhandle == *n)
		{
			lua_pushnil(l);
		}
		else
		{
			lua_pushnumber(l, *n);
		}
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int PushScriptParameterFloat(LuaVirtualMachine* vm, lua_State* l, void* data)
	{
		const float* n = static_cast<const float*>(data);
		lua_pushnumber(l, *n);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int PushScriptParameterDouble(LuaVirtualMachine* vm, lua_State* l, void* data)
	{
		const double* n = static_cast<const double*>(data);
		lua_pushnumber(l, *n);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int PushScriptParameterBool(LuaVirtualMachine* vm, lua_State* l, void* data)
	{
		const bool* n = static_cast<const bool*>(data);
		lua_pushboolean(l, *n);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int PushScriptParameterString(LuaVirtualMachine* vm, lua_State* l, void* data)
	{
		const char* n = static_cast<const char*>(data);
		lua_pushstring(l, n);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int PushScriptParameterBuffer(LuaVirtualMachine* vm, lua_State* l, void* data)
	{
		lua_pushlightuserdata(l, data);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int PushScriptParameterInt64(LuaVirtualMachine* vm, lua_State* l, void* data)
	{
		const int64* n = static_cast<const int64*>(data);
		int64* ud = (int64*)luaL_pushcdata(l, MY_CTID_INT64, sizeof(int64));
		*ud = *n;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int PushScriptParameterUInt64(LuaVirtualMachine* vm, lua_State* l, void* data)
	{
		const uint64* n = static_cast<const uint64*>(data);
		if (hc::nullid == *n)
		{
			lua_pushnil(l);
		}
		else
		{
			uint64* ud = (uint64*)luaL_pushcdata(l, MY_CTID_UINT64, sizeof(uint64));
			*ud = *n;
		}
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int PushScriptParameterClass(LuaVirtualMachine* vm, lua_State* l, void* data)
	{
		if (NULL == data)
		{
			lua_pushnil(l);
		}
		else
		{
			hc::Object* obj = static_cast<hc::Object*>(data);
			hc::ScriptValue* sv = static_cast<hc::ScriptValue*>(lua_newuserdata(l, sizeof(hc::ScriptValue)));
			sv->SetValue(obj, &hc::ScriptValue::NullCleaner);
			PushNative((lua_State*)l, (hc::IVirtualMachine*)vm, obj);
		}
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int PushScriptParameterInstance(LuaVirtualMachine* vm, lua_State* l, void* data)
	{
		if (NULL == data)
		{
			lua_pushnil(l);
		}
		else
		{
			hc::IScriptInstance* res = (hc::IScriptInstance*)data;
			int index = res->GetScriptInstance();
			lua_rawgeti(l, LUA_REGISTRYINDEX, index);
		}
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int PushScriptParameterArgumens(LuaVirtualMachine* vm, lua_State* l, void* data)
	{
		LuaArguments* args = (LuaArguments*)data;
		args->SetLuaState(l);		
		return args->RealDeserialize(vm);
	}
}