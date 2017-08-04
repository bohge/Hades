#include "PushScriptArgumentFunction.h"
#include "GetScriptReturnFunctions.h"
#include "script/IScriptInstance.h"
#include "LuaFunctionInvoker.h"
#include "LuaDobufferHelp.h"
#include "core/Types.h"



extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

using namespace hc;
using namespace eastl;
namespace hlss
{
	//-------------------------------------------------------------------------------------------------------
	LuaFunctionInvoker::LuaFunctionInvoker(lua_State** state, LuaVirtualMachine* vm)
		:m_rppLuaState(state)
		, m_Invoker(NULL)
		//, m_ClassReferences(cr)
		, m_ReturnFunc(NULL)
		, m_FuncitonReferences(nullhandle)
		, m_rpVirtualMachine(vm)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	LuaFunctionInvoker::~LuaFunctionInvoker(void)
	{
		ASSERT(nullhandle == m_FuncitonReferences);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::Exit()
	{
		if (*m_rppLuaState)//保证系统没有退出
		{
			luaL_unref(*m_rppLuaState, LUA_REGISTRYINDEX, m_FuncitonReferences);
			m_FuncitonReferences = nullhandle;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::_InitArguments(const hc::ScriptProperty::Arguments& args)
	{
		m_InputArray.resize(args.size());
		m_PusherArray.reserve(args.size());
		FOR_EACH_CONST(hc::ScriptProperty::Arguments, rtti, args)
		{
			PushArgumentsFunc func = NULL;
			switch ((*rtti))
			{
			case BOOL_RTTI_ID: func = &PushScriptParameterBool; break;
			case STRING_RTTI_ID:func = &PushScriptParameterString; break;
			case SHORT_RTTI_ID: func = &PushScriptParameterShort; break;
			case USHORT_RTTI_ID: func = &PushScriptParameterUShort; break;
			case INT_RTTI_ID: func = &PushScriptParameterInt; break;
			case UINT_RTTI_ID: func = &PushScriptParameterUInt; break;
			case FLOAT_RTTI_ID: func = &PushScriptParameterFloat; break;
			case DOUBLE_RTTI_ID: func = &PushScriptParameterDouble; break;
			case BUFFER_RTTI_ID: func = &PushScriptParameterBuffer; break;
			case INT64_RTTI_ID: func = &PushScriptParameterInt64; break;
			case UINT64_RTTI_ID: func = &PushScriptParameterUInt64; break;
			case INSTANCE_RTTI_ID: func = &PushScriptParameterInstance; break;//注册的脚本函数在c++端不需要传递函数和实例参数
			case FUNCTION_RTTI_ID: ASSERT(false); break;
			case ARGUMENTS_RTTI_ID: ASSERT(false); break;
			default: func = &PushScriptParameterClass; break;
			}
			m_PusherArray.push_back(func);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::_InitReferences(int cr, const char* fn)
	{
		DEBUGCODE(int t = lua_gettop(*m_rppLuaState));
		lua_rawgeti(*m_rppLuaState, LUA_REGISTRYINDEX, cr);
		lua_getfield(*m_rppLuaState, -1, fn);
		ASSERT(lua_isfunction(*m_rppLuaState, -1));
		m_FuncitonReferences = luaL_ref(*m_rppLuaState, LUA_REGISTRYINDEX);
		lua_pop(*m_rppLuaState, 1);
		ASSERT(t == lua_gettop(*m_rppLuaState));
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::_InitReturnFunction(uint sid)
	{
		switch (sid)
		{
		case BOOL_RTTI_ID: m_ReturnFunc = &GetScriptRenturnBool; break;
		case SHORT_RTTI_ID:	m_ReturnFunc = &GetScriptRenturnShort; break;
		case USHORT_RTTI_ID: m_ReturnFunc = &GetScriptRenturnUShort; break;
		case INT_RTTI_ID: m_ReturnFunc = &GetScriptRenturnInt; break;
		case UINT_RTTI_ID: m_ReturnFunc = &GetScriptRenturnUInt; break;
		case FLOAT_RTTI_ID: m_ReturnFunc = &GetScriptRenturnFloat; break;
		case DOUBLE_RTTI_ID: m_ReturnFunc = &GetScriptRenturnDouble; break;
		case STRING_RTTI_ID: m_ReturnFunc = &GetScriptRenturnString; break;
		case INT64_RTTI_ID: m_ReturnFunc = &GetScriptRenturnInt64; break;
		case UINT64_RTTI_ID: m_ReturnFunc = &GetScriptRenturnUInt64; break;
		case ARGUMENTS_RTTI_ID:m_ReturnFunc = &GetScriptRenturnArguments; break;
		default: m_ReturnFunc = &GetScriptRenturnClass; break;
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::Initialize(int cr, const char* fn, const hc::ScriptProperty::Arguments& args)
	{
		_InitArguments(args);
		_InitReferences(cr, fn);
		m_Invoker = &LuaFunctionInvoker::_InvokeNoResults;
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::Initialize(int ref, const hc::ScriptProperty::Arguments& args)
	{
		_InitArguments(args);
		m_FuncitonReferences = ref;
		m_Invoker = &LuaFunctionInvoker::_InvokeNoResults;
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::Initialize(int cr, const char* fn, uint res, const hc::ScriptProperty::Arguments& args)
	{
		_InitArguments(args);
		_InitReferences(cr, fn);
		_InitReturnFunction(res);
		m_Invoker = &LuaFunctionInvoker::_InvokeWithResults;
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::Initialize(int ref, uint res, const hc::ScriptProperty::Arguments& args)
	{
		_InitArguments(args);
		m_FuncitonReferences = ref;
		_InitReturnFunction(res);
		m_Invoker = &LuaFunctionInvoker::_InvokeWithResults;
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::_InvokeWithResults()
	{
		void** res = m_InputArray[0];//第一个是返回值
		m_InputArray[0] = NULL;
		int argusize = 0;
		lua_pushcclosure(*m_rppLuaState, on_error, 0);
		int errfunc = lua_gettop(*m_rppLuaState);
		lua_rawgeti(*m_rppLuaState, LUA_REGISTRYINDEX, m_FuncitonReferences);
		ASSERT(lua_isfunction(*m_rppLuaState, -1));
		//lua_rawgeti(*m_rppLuaState, LUA_REGISTRYINDEX, m_ClassReferences);
		for (int i = 0; i < m_PusherArray.size(); ++i)
		{
			void** argument = m_InputArray[i + 1];
			PushArgumentsFunc& func = m_PusherArray[i];
			argusize += func(m_rpVirtualMachine, *m_rppLuaState, *argument);
			m_InputArray[i + 1] = NULL;
		}
		lua_pcall(*m_rppLuaState, argusize, 1, errfunc);
		int pop = m_ReturnFunc(*res, NULL, *m_rppLuaState, -1);
		lua_pop(*m_rppLuaState, pop + 1);
		ASSERT(0 == lua_gettop(*m_rppLuaState));
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::_InvokeNoResults()
	{
		int argusize = 0;
		lua_pushcclosure(*m_rppLuaState, on_error, 0);
		int errfunc = lua_gettop(*m_rppLuaState);
		lua_rawgeti(*m_rppLuaState, LUA_REGISTRYINDEX, m_FuncitonReferences);
		ASSERT(lua_isfunction(*m_rppLuaState, -1));
		//lua_rawgeti(*m_rppLuaState, LUA_REGISTRYINDEX, m_ClassReferences);
		for (int i = 0; i < m_PusherArray.size(); ++i)
		{
			void** argument = m_InputArray[i];
			PushArgumentsFunc& func = m_PusherArray[i];
			argusize += func(m_rpVirtualMachine, *m_rppLuaState, *argument);
			m_InputArray[i] = NULL;
		}
		lua_pcall(*m_rppLuaState, argusize, 0, errfunc);
		lua_pop(*m_rppLuaState, 1);
		ASSERT(0 == lua_gettop(*m_rppLuaState));
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::Invoke()
	{
		ASSERT(0 == m_InputArray.size());
		(this->*m_Invoker)();
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::_DoInvoke(void** p1)
	{
		ASSERT(1 == m_InputArray.size());
		m_InputArray[0] = p1;
		(this->*m_Invoker)();
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::_DoInvoke(void** p1, void** p2)
	{
		ASSERT(2 == m_InputArray.size());
		m_InputArray[0] = p1;
		m_InputArray[1] = p2;
		(this->*m_Invoker)();
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::_DoInvoke(void** p1, void** p2, void** p3)
	{
		ASSERT(3 == m_InputArray.size());
		m_InputArray[0] = p1;
		m_InputArray[1] = p2;
		m_InputArray[2] = p3;
		(this->*m_Invoker)();
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaFunctionInvoker::_DoInvoke(void** p1, void** p2, void** p3, void** p4)
	{
		ASSERT(4 == m_InputArray.size());
		m_InputArray[0] = p1;
		m_InputArray[1] = p2;
		m_InputArray[2] = p3;
		m_InputArray[3] = p4;
		(this->*m_Invoker)();
	}
}