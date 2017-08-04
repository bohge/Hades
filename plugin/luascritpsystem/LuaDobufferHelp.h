#pragma once
#include "core/Predefine.h"
#include "log/ILogSystem.h"



extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

static void print_error(lua_State *L, const char* fmt, ...)
{
	char text[4096];

	va_list args;
	va_start(args, fmt);
	vsprintf(text, fmt, args);
	va_end(args);

	lua_getglobal(L, "_ALERT");
	if (lua_isfunction(L, -1))
	{
		lua_pushstring(L, text);
		lua_call(L, 1, 0);
	}
	else
	{
		HADESERROR("%s", text);
		lua_pop(L, 1);
	}
	HADESERROR("\n");
}

static void call_stack(lua_State* L, int n)
{
	lua_Debug ar;
	if (lua_getstack(L, n, &ar) == 1)
	{
		lua_getinfo(L, "nSlu", &ar);

		const char* indent;
		if (n == 0)
		{
			indent = "->\t";
			print_error(L, "\t<call stack>");
		}
		else
		{
			indent = "\t";
		}

		if (ar.name)
			print_error(L, "%s%s() : line %d [%s : line %d]", indent, ar.name, ar.currentline, ar.source, ar.linedefined);
		else
			print_error(L, "%sunknown : line %d [%s : line %d]", indent, ar.currentline, ar.source, ar.linedefined);

		call_stack(L, n + 1);
	}
}

static int on_error(lua_State *L)
{
	print_error(L, "%s", lua_tostring(L, -1));

	call_stack(L, 0);

	return 0;
}
static int DoBuffer(lua_State *L, const char *classname, const char *filename, const char* buff, size_t len)
{
	lua_pushcclosure(L, on_error, 0);
	int errfunc = lua_gettop(L);
	//luaL_getsubtable( L, LUA_REGISTRYINDEX, "_LOADED" );
	lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
	lua_getfield(L, -1, filename);  /* _LOADED[name] */
	ASSERT(!lua_toboolean(L, -1) && "already loaded!!");//必须是没有加载的
	if (luaL_loadbuffer(L, buff, len, filename) == 0)
	{
		lua_pushvalue(L, -1);
		lua_setfield(L, -4, filename);
		if (lua_pcall(L, 0, 1, errfunc) == 0)
		{
			if (!lua_istable(L, -1))
			{
				lua_pop(L, 1);//弹出nil
				lua_getglobal(L, classname);//得到创建实例的函数
			}
			if (lua_istable(L, -1))
			{
				int res = luaL_ref(L, LUA_REGISTRYINDEX);//将该函数绑定到索引列表
				lua_pop(L, 3);
				return res;
			}
		}
	}
	const char* error = lua_tostring(L, -1);
	if (error)
	{
		print_error(L, "%s", error);
	}
	else
	{
		print_error(L, "file %s must return class value or be a global", filename);
	}
	lua_pop(L, 4);
	return -1;
}