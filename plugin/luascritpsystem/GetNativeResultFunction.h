#pragma once
#include "script/ScriptObjectpool.h"
#include "script/ScriptValue.h"
#include "NativePushHelper.hpp"

extern "C" 
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

//native返回到脚本的返回值
namespace hlss
{
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultValueShort(void* vm, void* L, void* index)
	{
		return hc::ScriptObjectpool::Instance()->GetObject(hc::ScriptObjectpool::POOL_INDEX_SHORT);
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultValueUShort(void* vm, void* L, void* index)
	{
		return hc::ScriptObjectpool::Instance()->GetObject(hc::ScriptObjectpool::POOL_INDEX_USHORT);
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultValueInt(void* vm, void* L, void* index)
	{
		return hc::ScriptObjectpool::Instance()->GetObject(hc::ScriptObjectpool::POOL_INDEX_INT);
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultValueUInt(void* vm, void* L, void* index)
	{
		return hc::ScriptObjectpool::Instance()->GetObject(hc::ScriptObjectpool::POOL_INDEX_UINT);
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultValueFloat(void* vm, void* L, void* index)
	{
		return hc::ScriptObjectpool::Instance()->GetObject(hc::ScriptObjectpool::POOL_INDEX_FLOAT);
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultValueDouble(void* vm, void* L, void* index)
	{
		return hc::ScriptObjectpool::Instance()->GetObject(hc::ScriptObjectpool::POOL_INDEX_DOUBLE);
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultValueBool(void* vm, void* L, void* index)
	{
		return hc::ScriptObjectpool::Instance()->GetObject(hc::ScriptObjectpool::POOL_INDEX_BOOL);
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultValueString(void* vm, void* L, void* index)
	{
		return hc::ScriptObjectpool::Instance()->GetObject(hc::ScriptObjectpool::POOL_INDEX_STRING_PUSH);
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultValueBuffer(void* vm, void* L, void* index)
	{
		return hc::ScriptObjectpool::Instance()->GetObject(hc::ScriptObjectpool::POOL_INDEX_BUFFER);
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultValueInt64(void* vm, void* L, void* index)
	{
		return hc::ScriptObjectpool::Instance()->GetObject(hc::ScriptObjectpool::POOL_INDEX_INT64);
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultValueUInt64(void* vm, void* L, void* index)
	{
		return hc::ScriptObjectpool::Instance()->GetObject(hc::ScriptObjectpool::POOL_INDEX_UINT64);
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultValueClass(void* vm, void* L, void* index)
	{
		return lua_newuserdata( (lua_State*)L, sizeof(hc::ScriptValue) );
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultValueInstance(void* vm, void* L, void* index)
	{
		return hc::ScriptObjectpool::Instance()->GetObject(hc::ScriptObjectpool::POOL_INDEX_INT);
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultValueFloatArray(void* vm, void* L, void* index)
	{
		return hc::ScriptObjectpool::Instance()->GetObject(hc::ScriptObjectpool::POOL_INDEX_FLOATARRAY);
	}


	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultPushShort(void* vm, void* L, void* volue)
	{
		short* res = (short*)volue;
		lua_pushinteger((lua_State*)L, *res);
		hc::ScriptObjectpool::Instance()->FreeObject(hc::ScriptObjectpool::POOL_INDEX_SHORT, volue);
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultPushUShort(void* vm, void* L, void* volue)
	{
		ushort hv = *(ushort*)volue;
		if (hc::nullhandle == hv)
		{
			lua_pushnil((lua_State*)L);
		}
		else
		{
			lua_pushinteger((lua_State*)L, hv);
		}
		hc::ScriptObjectpool::Instance()->FreeObject(hc::ScriptObjectpool::POOL_INDEX_USHORT, volue);
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultPushInt(void* vm, void* L, void* volue)
	{
		int* res = (int*)volue;
		lua_pushinteger((lua_State*)L, *res);
		hc::ScriptObjectpool::Instance()->FreeObject(hc::ScriptObjectpool::POOL_INDEX_INT, volue);
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultPushUInt(void* vm, void* L, void* volue)
	{
		uint hv = *(uint*)volue;
		if (hc::nullhandle == hv)
		{
			lua_pushnil((lua_State*)L);
		}
		else
		{
			lua_pushnumber((lua_State*)L, hv);
		}
		hc::ScriptObjectpool::Instance()->FreeObject(hc::ScriptObjectpool::POOL_INDEX_UINT, volue);
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultPushFloat(void* vm, void* L, void* volue)
	{
		float* res = (float*)volue;
		lua_pushnumber((lua_State*)L, *res);
		hc::ScriptObjectpool::Instance()->FreeObject(hc::ScriptObjectpool::POOL_INDEX_FLOAT, volue);
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultPushDouble(void* vm, void* L, void* volue)
	{
		double* res = (double*)volue;
		lua_pushnumber((lua_State*)L, *res);
		hc::ScriptObjectpool::Instance()->FreeObject(hc::ScriptObjectpool::POOL_INDEX_DOUBLE, volue);
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultPushBool(void* vm, void* L, void* volue)
	{
		bool* res = (bool*)volue;
		lua_pushboolean((lua_State*)L, *res);
		hc::ScriptObjectpool::Instance()->FreeObject(hc::ScriptObjectpool::POOL_INDEX_BOOL, volue);
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultPushString(void* vm, void* L, void* volue)
	{
		eastl::string* str = ((eastl::string*)volue);
		lua_pushlstring((lua_State*)L, str->c_str(), str->size());
		hc::ScriptObjectpool::Instance()->FreeObject(hc::ScriptObjectpool::POOL_INDEX_STRING_PUSH, volue);
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultPushBuffer(void* vm, void* L, void* volue)
	{
		hc::StringBuffer* buf = static_cast<hc::StringBuffer*>(volue);
		if (NULL == buf->pBuffer)
		{
			lua_pushnil((lua_State*)L);
		}
		else
		{
			lua_pushlightuserdata((lua_State*)L, const_cast<void*>(buf->pBuffer));
			hc::ScriptObjectpool::Instance()->FreeObject(hc::ScriptObjectpool::POOL_INDEX_BUFFER, volue);
		}
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultPushInt64(void* vm, void* L, void* volue)
	{
		int64* res = (int64*)luaL_pushcdata((lua_State*)L, MY_CTID_INT64, sizeof(int64));
		*res = *(int64*)volue;
		hc::ScriptObjectpool::Instance()->FreeObject(hc::ScriptObjectpool::POOL_INDEX_INT64, volue);
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultPushUInt64(void* vm, void* L, void* volue)
	{
		uint64 hv = *(uint64*)volue;
		if (hc::nullid == hv)
		{
			lua_pushnil((lua_State*)L);
		}
		else
		{
			uint64* res = (uint64*)luaL_pushcdata((lua_State*)L, MY_CTID_UINT64, sizeof(uint64));
			*res = hv;
		}
		hc::ScriptObjectpool::Instance()->FreeObject(hc::ScriptObjectpool::POOL_INDEX_UINT64, volue);
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultPushClass(void* vm, void* L, void* volue)
	{
		if (NULL == ((hc::ScriptValue*)volue)->GetObject())//返回空值
		{
			lua_remove( (lua_State*)L, -1 );
			lua_pushnil( (lua_State*)L );
		}
		else
		{
			PushNative((lua_State*)L, (hc::IVirtualMachine*)vm, ((hc::ScriptValue*)volue)->GetObject());
		}
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultPushInstance(void* vm, void* L, void* volue)
	{
		int* res = (int*)volue;
		if (hc::nullhandle != *res)
		{
			lua_rawgeti((lua_State*)L, LUA_REGISTRYINDEX, *res);
		}
		hc::ScriptObjectpool::Instance()->FreeObject(hc::ScriptObjectpool::POOL_INDEX_INT, volue);
		return NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	static void* GetNativeResultPushFloatArray(void* vm, void* L, void* volue)
	{
		hc::ScriptValue* data = static_cast<hc::ScriptValue*>(volue);
		eastl::vector<float>* dataarray = (eastl::vector<float>*)data->GetObject();
		lua_State* ls = (lua_State*)L;
		lua_newtable(ls);
		for (size_t i = 0; i < dataarray->size(); i++)
		{
			lua_pushnumber(ls, dataarray->at(i));
			lua_rawseti(ls, -2, i + 1);
		}
		data->Clear();
		hc::ScriptObjectpool::Instance()->FreeObject(hc::ScriptObjectpool::POOL_INDEX_FLOATARRAY, volue);
		return NULL;
	}
}