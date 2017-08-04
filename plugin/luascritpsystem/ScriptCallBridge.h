#pragma once
#include "script/FunctionRegister.h"
#include "script/IFunctionInvoker.h"
#include "script/ScriptUtility.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "LuaVirtualMachine.h"
#include "core/Types.h"


extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#define MAX_ARGUMENT 8
namespace hlss
{
	//-------------------------------------------------------------------------------------------------------
	static const hc::Rtti* ObjectDefinedHelper(lua_State* L, int index)//找到对应的参数类型
	{
		switch (lua_type(L, index))
		{
		case LUA_TBOOLEAN: return &hc::Bool::RTTI();
		case LUA_TNUMBER: return &hc::Double::RTTI();
		case LUA_TSTRING: return &hc::String::RTTI();
		case LUA_TTABLE: return &hc::Instance::RTTI();
		case LUA_TFUNCTION: return &hc::IFunctionInvoker::RTTI();
		case LUA_TUSERDATA: return &((hc::ScriptValue*)lua_touserdata(L, index))->GetObject()->GetRtti();
		case LUA_TCDATA:
		{
						   switch (luaL_cdatatype(L, index))
						   {
						   case MY_CTID_INT64: return &hc::Int64::RTTI();
						   case MY_CTID_UINT64: return &hc::UInt64::RTTI();
						   default:ASSERT(false);
						   }
		}
		default:ASSERT(false);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	static int ClassDestruct(lua_State *L)//析构函数
	{
		hc::ScriptValue* data = (hc::ScriptValue*)lua_touserdata(L, 1);
		data->~ScriptValue();
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int SingleClassConstructStatic(lua_State *L)//构造函数和静态函数
	{
		void* args[MAX_ARGUMENT]{NULL};
		hc::ClassRegister* sc =
			(hc::ClassRegister*)lua_touserdata(L, lua_upvalueindex(1));
		int index = lua_tointeger(L, lua_upvalueindex(2));
		LuaVirtualMachine* vm =
			(LuaVirtualMachine*)lua_touserdata(L, lua_upvalueindex(3));

		hc::FunctionRegister*func = sc->GetFunction(index);

		uint ac = func->GetArgumentCount();
		int retvalue = 0;
		bool hasRes = func->hasReturn();
		if (hasRes)
		{
			void* resdata = func->GetScriptResultValue(vm, L, NULL);
			switch (ac)
			{
			case 0:retvalue = func->Invoke(vm, func, resdata); break;
			case 1:
			{
					  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
					  retvalue = func->Invoke(vm, func, resdata,
						  args[0]);
			}break;
			case 2:
			{
					  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
					  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
					  retvalue = func->Invoke(vm, func, resdata,
						  args[0],
						  args[1]);
			}break;
			case 3:
			{
					  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
					  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
					  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
					  retvalue = func->Invoke(vm, func, resdata,
						  args[0],
						  args[1],
						  args[2]);
			}break;
			case 4:
			{
					  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
					  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
					  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
					  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
					  retvalue = func->Invoke(vm, func, resdata,
						  args[0],
						  args[1],
						  args[2],
						  args[3]);
			}break;
			case 5:
			{
					  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
					  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
					  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
					  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
					  args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
					  retvalue = func->Invoke(vm, func, resdata,
						  args[0],
						  args[1],
						  args[2],
						  args[3],
						  args[4]);
			}break;
			case 6:
			{
					  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
					  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
					  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
					  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
					  args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
					  args[5] = func->GetScriptParameter(5, vm, L, (void*)7);
					  retvalue = func->Invoke(vm, func, resdata,
						  args[0],
						  args[1],
						  args[2],
						  args[3],
						  args[4],
						  args[5]);
			}break;
			default:ASSERT(false);
			}
			func->PushScriptReslut(vm, L, resdata);
		}
		else
		{
			switch (ac)
			{
			case 0:retvalue = func->Invoke(vm); break;
			case 1:
			{
					  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
					  retvalue = func->Invoke(vm, func,
						  args[0]);
			}break;
			case 2:
			{
					  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
					  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
					  retvalue = func->Invoke(vm, func,
						  args[0],
						  args[1]);
			}break;
			case 3:
			{
					  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
					  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
					  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
					  retvalue = func->Invoke(vm, func,
						  args[0],
						  args[1],
						  args[2]);
			}break;
			case 4:
			{
					  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
					  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
					  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
					  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
					  retvalue = func->Invoke(vm, func,
						  args[0],
						  args[1],
						  args[2],
						  args[3]);
			}break;
			case 5:
			{
					  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
					  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
					  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
					  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
					  args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
					  retvalue = func->Invoke(vm, func,
						  args[0],
						  args[1],
						  args[2],
						  args[3],
						  args[4]);
			}break;
			case 6:
			{
					  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
					  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
					  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
					  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
					  args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
					  args[5] = func->GetScriptParameter(5, vm, L, (void*)7);
					  retvalue = func->Invoke(vm, func,
						  args[0],
						  args[1],
						  args[2],
						  args[3],
						  args[4],
						  args[5]);
			}break;
			default:ASSERT(false);
			}
		}
		func->Clearup(ac, args);
		//ASSERT(hasRes ? 1 == retvalue : retvalue <= 0);
		return retvalue;
	}

	//-------------------------------------------------------------------------------------------------------
	static int OverrideClassConstructStatic(lua_State *L)//重载的函数
	{
		void* args[MAX_ARGUMENT]{NULL};
		hc::ClassRegister* sc =
			(hc::ClassRegister*)lua_touserdata(L, lua_upvalueindex(1));
		int index = lua_tointeger(L, lua_upvalueindex(2));
		LuaVirtualMachine* vm =
			(LuaVirtualMachine*)lua_touserdata(L, lua_upvalueindex(3));

		uint ac = lua_gettop(L) - 1;//得到参数数量，减一的原因是不需要传入self
		hc::FunctionRegister* func = NULL;
		func = sc->TryGetFunction(index, ac);
		if (NULL == func)
		{
			switch (ac)
			{
			case 0:func = sc->GetFunction(index); break;
			case 1:func = sc->GetFunction(index,
				ObjectDefinedHelper(L, 2)
				); break;
			case 2:func = sc->GetFunction(index,
				ObjectDefinedHelper(L, 2),
				ObjectDefinedHelper(L, 3)
				); break;
			case 3:func = sc->GetFunction(index,
				ObjectDefinedHelper(L, 2),
				ObjectDefinedHelper(L, 3),
				ObjectDefinedHelper(L, 4)
				); break;
			case 4:func = sc->GetFunction(index,
				ObjectDefinedHelper(L, 2),
				ObjectDefinedHelper(L, 3),
				ObjectDefinedHelper(L, 4),
				ObjectDefinedHelper(L, 5)
				); break;
			case 5:func = sc->GetFunction(index,
				ObjectDefinedHelper(L, 2),
				ObjectDefinedHelper(L, 3),
				ObjectDefinedHelper(L, 4),
				ObjectDefinedHelper(L, 5),
				ObjectDefinedHelper(L, 6)
				); break;
			case 6:func = sc->GetFunction(index,
				ObjectDefinedHelper(L, 2),
				ObjectDefinedHelper(L, 3),
				ObjectDefinedHelper(L, 4),
				ObjectDefinedHelper(L, 5),
				ObjectDefinedHelper(L, 6),
				ObjectDefinedHelper(L, 7)
				); break;
			default:ASSERT(false);
			}
		}

		int retvalue = 0;
		ASSERT(func->hasReturn());
		void* resdata = func->GetScriptResultValue(vm, L, NULL);
		switch (ac)
		{
		case 0:retvalue = func->Invoke(vm, func, resdata); break;
		case 1:
		{
				  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
				  retvalue = func->Invoke(vm, func, resdata,
					  args[0]);
		}break;
		case 2:
		{
				  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
				  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
				  retvalue = func->Invoke(vm, func, resdata,
					  args[0],
					  args[1]);
		}break;
		case 3:
		{
				  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
				  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
				  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
				  retvalue = func->Invoke(vm, func, resdata,
					  args[0],
					  args[1],
					  args[2]);
		}break;
		case 4:
		{
				  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
				  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
				  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
				  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
				  retvalue = func->Invoke(vm, func, resdata,
					  args[0],
					  args[1],
					  args[2],
					  args[3]);
		}break;
		case 5:
		{
				  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
				  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
				  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
				  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
				  args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
				  retvalue = func->Invoke(vm, func, resdata,
					  args[0],
					  args[1],
					  args[2],
					  args[3],
					  args[4]);
		}break;
		case 6:
		{
				  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
				  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
				  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
				  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
				  args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
				  args[5] = func->GetScriptParameter(5, vm, L, (void*)7);
				  retvalue = func->Invoke(vm, func, resdata,
					  args[0],
					  args[1],
					  args[2],
					  args[3],
					  args[4],
					  args[5]);
		}break;
		default:ASSERT(false);
		}
		func->PushScriptReslut(vm, L, resdata);
		func->Clearup(ac, args);
		ASSERT(1 == retvalue);
		return retvalue;
	}

	//-------------------------------------------------------------------------------------------------------
	static int SingleClassFunction(lua_State *L)//只有一个函数的同名函数
	{
		void* args[MAX_ARGUMENT]{NULL};
		hc::ClassRegister* sc =
			(hc::ClassRegister*)lua_touserdata(L, lua_upvalueindex(1));
		int index = lua_tointeger(L, lua_upvalueindex(2));
		LuaVirtualMachine* vm =
			(LuaVirtualMachine*)lua_touserdata(L, lua_upvalueindex(3));

		hc::FunctionRegister*func = sc->GetFunction(index);
		uint ac = func->GetArgumentCount();
		bool hasRes = func->hasReturn();
		int retvalue = 0;
		if (NULL == lua_touserdata(L, 1))
		{
			if (hasRes)
			{
				retvalue = 1;
				lua_pushnil(L);
			}
		}
		else
		{
			if (hasRes)
			{//如果有返回值
				void* resdata = func->GetScriptResultValue(vm, L, NULL);
				switch (ac)
				{
				case 0:retvalue = func->Invoke(vm, func, resdata,
					((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject()//成员函数第一个永远是this指针
					); break;
				case 1:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
							  retvalue = func->Invoke(vm, func, resdata,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0]);
				}break;
				case 2:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  retvalue = func->Invoke(vm, func, resdata,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1]);
				}break;
				case 3:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  retvalue = func->Invoke(vm, func, resdata,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2]);
				}break;
				case 4:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
						  retvalue = func->Invoke(vm, func, resdata,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2],
							  args[3]);
				}break;
				case 5:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
						  args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
						  retvalue = func->Invoke(vm, func, resdata,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2],
							  args[3],
							  args[4]);
				}break;
				case 6:
				{
						   args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						   args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						   args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						   args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
						   args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
						   args[5] = func->GetScriptParameter(5, vm, L, (void*)7);
						   retvalue = func->Invoke(vm, func, resdata,
							   ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							   args[0],
							   args[1],
							   args[2],
							   args[3],
							   args[4],
							   args[5]);
				}break;
				default:ASSERT(false);
				}
				func->PushScriptReslut(vm, L, resdata);
			}
			else
			{
				switch (ac)
				{
				case 0:retvalue = func->Invoke(vm, func,
					((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject()
					); break;
				case 1:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  retvalue = func->Invoke(vm, func,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0]);
				}break;
				case 2:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  retvalue = func->Invoke(vm, func,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1]);
				}break;
				case 3:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  retvalue = func->Invoke(vm, func,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2]);
				}break;
				case 4:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
						  retvalue = func->Invoke(vm, func,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2],
							  args[3]);
				}break;
				case 5:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
						  args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
						  retvalue = func->Invoke(vm, func,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2],
							  args[3],
							  args[4]);
				}break;
				case 6:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
						  args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
						  args[5] = func->GetScriptParameter(5, vm, L, (void*)7);
						  retvalue = func->Invoke(vm, func,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2],
							  args[3],
							  args[4],
							  args[5]);
				}break;
				case 7:
				{
						   args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						   args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						   args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						   args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
						   args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
						   args[5] = func->GetScriptParameter(5, vm, L, (void*)7);
						   args[6] = func->GetScriptParameter(6, vm, L, (void*)8);
						   retvalue = func->Invoke(vm, func,
							   ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							   args[0],
							   args[1],
							   args[2],
							   args[3],
							   args[4],
							   args[5],
							   args[6]);
				}break;
				default:ASSERT(false);
				}
			}
		}
		func->Clearup(ac, args);
		//ASSERT(hasRes ? 1 == retvalue : retvalue <= 0);
		return retvalue;
	}
	//-------------------------------------------------------------------------------------------------------
	static int OverrideClassFunction(lua_State *L)//重载的函数
	{
		void* args[MAX_ARGUMENT]{NULL};
		hc::ClassRegister* sc =
			(hc::ClassRegister*)lua_touserdata(L, lua_upvalueindex(1));
		int index = lua_tointeger(L, lua_upvalueindex(2));
		LuaVirtualMachine* vm =
			(LuaVirtualMachine*)lua_touserdata(L, lua_upvalueindex(3));


		uint ac = lua_gettop(L) - 1;//得到参数数量，减一的原因是不需要传入self
		hc::FunctionRegister* func = NULL;
		func = sc->TryGetFunction(index, ac);
		if (NULL == func)
		{
			switch (ac)
			{
			case 1:func = sc->GetFunction(index,
				ObjectDefinedHelper(L, 2)
				); break;
			case 2:func = sc->GetFunction(index,
				ObjectDefinedHelper(L, 2),
				ObjectDefinedHelper(L, 3)
				); break;
			case 3:func = sc->GetFunction(index,
				ObjectDefinedHelper(L, 2),
				ObjectDefinedHelper(L, 3),
				ObjectDefinedHelper(L, 4)
				); break;
			case 4:func = sc->GetFunction(index,
				ObjectDefinedHelper(L, 2),
				ObjectDefinedHelper(L, 3),
				ObjectDefinedHelper(L, 4),
				ObjectDefinedHelper(L, 5)
				); break;
			case 5:func = sc->GetFunction(index,
				ObjectDefinedHelper(L, 2),
				ObjectDefinedHelper(L, 3),
				ObjectDefinedHelper(L, 4),
				ObjectDefinedHelper(L, 5),
				ObjectDefinedHelper(L, 6)
				); break;
			case 6:func = sc->GetFunction(index,
				ObjectDefinedHelper(L, 2),
				ObjectDefinedHelper(L, 3),
				ObjectDefinedHelper(L, 4),
				ObjectDefinedHelper(L, 5),
				ObjectDefinedHelper(L, 6),
				ObjectDefinedHelper(L, 7)
				); break;
			default:ASSERT(false);
			}
		}

		int retvalue = 0;
		bool hasRes = func->hasReturn();
		if (NULL == lua_touserdata(L, 1))
		{
			if (hasRes)
			{
				retvalue = 1;
				lua_pushnil(L);
			}
		}
		else
		{
			if (hasRes)
			{//如果有返回值
				void* resdata = func->GetScriptResultValue(vm, L, NULL);
				switch (ac)
				{
				case 0:retvalue = func->Invoke(vm, func, resdata,
					((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject()//this指针
					); break;
				case 1:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  retvalue = func->Invoke(vm, func, resdata,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0]);
				}break;
				case 2:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  retvalue = func->Invoke(vm, func, resdata,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1]);
				}break;
				case 3:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  retvalue = func->Invoke(vm, func, resdata,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2]);
				}break;
				case 4:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
						  retvalue = func->Invoke(vm, func, resdata,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2],
							  args[3]);
				}break;
				case 5:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
						  args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
						  retvalue = func->Invoke(vm, func, resdata,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2],
							  args[3],
							  args[4]);
				}break;
				case 6:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
						  args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
						  args[5] = func->GetScriptParameter(5, vm, L, (void*)7);
						  retvalue = func->Invoke(vm, func, resdata,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2],
							  args[3],
							  args[4],
							  args[5]);
				}break;
				case 7:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
						  args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
						  args[5] = func->GetScriptParameter(5, vm, L, (void*)7);
						  args[6] = func->GetScriptParameter(6, vm, L, (void*)8);
						  retvalue = func->Invoke(vm, func, resdata,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2],
							  args[3],
							  args[4],
							  args[5],
							  args[6]);
				}break;
				default:ASSERT(false);
				}
				func->PushScriptReslut(vm, L, resdata);
			}
			else
			{
				switch (ac)
				{
				case 1:retvalue = func->Invoke(vm, func,
					((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
					func->GetScriptParameter(0, vm, L, (void*)2)//参数
					); break;
				case 2:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  retvalue = func->Invoke(vm, func,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1]);
				}break;
				case 3:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  retvalue = func->Invoke(vm, func,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2]);
				}break;
				case 4:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
						  retvalue = func->Invoke(vm, func,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2],
							  args[3]);
				}break;
				case 5:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
						  args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
						  retvalue = func->Invoke(vm, func,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2],
							  args[3],
							  args[4]);
				}break;
				case 6:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
						  args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
						  args[5] = func->GetScriptParameter(5, vm, L, (void*)7);
						  retvalue = func->Invoke(vm, func,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2],
							  args[3],
							  args[4],
							  args[5]);
				}break;
				case 7:
				{
						  args[0] = func->GetScriptParameter(0, vm, L, (void*)2);
						  args[1] = func->GetScriptParameter(1, vm, L, (void*)3);
						  args[2] = func->GetScriptParameter(2, vm, L, (void*)4);
						  args[3] = func->GetScriptParameter(3, vm, L, (void*)5);
						  args[4] = func->GetScriptParameter(4, vm, L, (void*)6);
						  args[5] = func->GetScriptParameter(5, vm, L, (void*)7);
						  args[6] = func->GetScriptParameter(6, vm, L, (void*)8);
						  retvalue = func->Invoke(vm, func,
							  ((hc::ScriptValue*)lua_touserdata(L, 1))->GetObject(),//this指针
							  args[0],
							  args[1],
							  args[2],
							  args[3],
							  args[4],
							  args[5],
							  args[6]);
				}break;
				default:ASSERT(false);
				}
			}
		}
		//ASSERT(hasRes ? 1 == retvalue : retvalue <= 0);
		func->Clearup(ac, args);
		return retvalue;
	}
}