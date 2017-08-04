#include "PushScriptArgumentFunction.h"
#include "GetScriptReturnFunctions.h"
#include "script/IScriptInstance.h"
#include "script/IScriptSystem.h"
#include "LuaCoroutineInvoker.h"
#include "LuaDobufferHelp.h"
#include "core/Types.h"




extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

#define GC_STEP 2

using namespace hc;
using namespace eastl;
namespace hlss
{
	//-------------------------------------------------------------------------------------------------------
	LuaCoroutineInvoker::LuaCoroutineInvoker(lua_State** root, LuaVirtualMachine* vm)
		:m_pCorouLuaState(NULL)
		, m_Invoker(NULL)
		, m_rppRootState(root)
		, m_ReturnFunc(NULL)
		//, m_ClassReferences(cr)
		, m_StateReferences(nullhandle)
		, m_FunctionReferences(nullhandle)
		, m_rpVirtualMachine(vm)
#ifdef _PLATFORM_WINDOWS
		DEBUGCODE(, m_isResumeInitalized(false))
#else
		DEBUGCODE((, m_isResumeInitalized(false)))
#endif
	{
	}
	//-------------------------------------------------------------------------------------------------------
	LuaCoroutineInvoker::~LuaCoroutineInvoker(void)
	{
		ASSERT(nullhandle == m_StateReferences);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::Exit()
	{
		if (*m_rppRootState)//保证系统没有退出
		{
			luaL_unref(*m_rppRootState, LUA_REGISTRYINDEX, m_StateReferences);
			m_StateReferences = nullhandle;
		}
		//lua_close(m_pCorouLuaState);不能关闭
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::_InitArguments(PusherArray& pusherarray, const hc::ScriptProperty::Arguments& args)
	{
		pusherarray.reserve(args.size());
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
			case INSTANCE_RTTI_ID: func = &PushScriptParameterInstance; break;
			case FUNCTION_RTTI_ID: ASSERT(false); break;
			case ARGUMENTS_RTTI_ID: func = &PushScriptParameterArgumens; break;
			default: func = &PushScriptParameterClass; break;
			}
			pusherarray.push_back(func);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::_InitReturnFunction(uint sid)
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
	void LuaCoroutineInvoker::_InitReferences(int cr, int fn)
	{
		ASSERT(0 == lua_gettop(*m_rppRootState));
		m_pCorouLuaState = lua_newthread(*m_rppRootState);
		m_StateReferences = luaL_ref(*m_rppRootState, LUA_REGISTRYINDEX);
		m_FunctionReferences = fn;
		ASSERT(0 == lua_gettop(*m_rppRootState));
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::Initialize(int cr, int fn, const hc::ScriptProperty::Arguments& args)
	{
		m_InputArray.resize(args.size());
		_InitArguments(m_InvokerPusherArray, args);
		_InitReferences(cr, fn);
		m_Invoker = &LuaCoroutineInvoker::_InvokeNoResults;
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::Initialize(int cr, int fn, uint res, const hc::ScriptProperty::Arguments& args)
	{
		m_InputArray.resize(args.size() + 1);
		_InitArguments(m_InvokerPusherArray, args);
		_InitReferences(cr, fn);
		_InitReturnFunction(res);
		m_Invoker = &LuaCoroutineInvoker::_InvokeWithResults;
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::_SetupResume(const hc::ScriptProperty::Arguments& args)
	{
		DEBUGCODE(m_isResumeInitalized = true);
		_InitArguments(m_ResumePusherArray, args);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::_InvokeWithResults(InputArray& input, PusherArray& pushers)
	{
		void** res = input[0];//第一个是返回值
		input[0] = NULL;
		int argusize = 0;
		ASSERT(0 == lua_gettop(m_pCorouLuaState));
		lua_rawgeti(m_pCorouLuaState, LUA_REGISTRYINDEX, m_FunctionReferences);
		ASSERT(lua_isfunction(m_pCorouLuaState, -1));
		//lua_rawgeti(m_pCorouLuaState, LUA_REGISTRYINDEX, m_ClassReferences);
		for (int i = 0; i < pushers.size(); ++i)
		{
			void** argument = input[i + 1];
			PushArgumentsFunc& func = pushers[i];
			argusize += func(m_rpVirtualMachine, m_pCorouLuaState, *argument);
			input[i + 1] = NULL;
		}
		int code = lua_resume(m_pCorouLuaState, argusize/* + 1*/);
		if (LUA_OK == code)
		{
			_SetStatus(ICoroutineInvoker::CS_OK);
			int pop = m_ReturnFunc(*res, NULL, m_pCorouLuaState, -1);
			lua_pop(m_pCorouLuaState, pop);
			ASSERT(0 == lua_gettop(m_pCorouLuaState));
			lua_gc(m_pCorouLuaState, LUA_GCSTEP, GC_STEP);
		}
		else if (LUA_YIELD == code)
		{
			_SetStatus(ICoroutineInvoker::CS_YEILD);
		}
		else
		{
			_SetStatus(ICoroutineInvoker::CS_ERROR);
			on_error(m_pCorouLuaState);
			lua_gc(m_pCorouLuaState, LUA_GCSTEP, GC_STEP);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::_InvokeNoResults(InputArray& input, PusherArray& pushers)
	{
		int argusize = 0;
		ASSERT(0 == lua_gettop(m_pCorouLuaState));
		lua_rawgeti(m_pCorouLuaState, LUA_REGISTRYINDEX, m_FunctionReferences);
		ASSERT(lua_isfunction(m_pCorouLuaState, -1));
		//lua_rawgeti(m_pCorouLuaState, LUA_REGISTRYINDEX, m_ClassReferences);
		for (int i = 0; i < pushers.size(); ++i)
		{
			void** argument = input[i];
			PushArgumentsFunc& func = pushers[i];
			argusize += func(m_rpVirtualMachine, m_pCorouLuaState, argument);
			input[i] = NULL;
		}
		int code = lua_resume(m_pCorouLuaState, argusize/* + 1*/);
		if (LUA_OK == code)
		{
			_SetStatus(ICoroutineInvoker::CS_OK);
			ASSERT(0 == lua_gettop(m_pCorouLuaState));
			lua_gc(m_pCorouLuaState, LUA_GCSTEP, GC_STEP);
		}
		else if (LUA_YIELD == code)
		{
			_SetStatus(ICoroutineInvoker::CS_YEILD);
		}
		else
		{
			_SetStatus(ICoroutineInvoker::CS_ERROR);
			on_error(m_pCorouLuaState);
			lua_gc(m_pCorouLuaState, LUA_GCSTEP, GC_STEP);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::GotError(const eastl::string& error)
	{
		luaL_error(m_pCorouLuaState, error.c_str());
	}
	//-------------------------------------------------------------------------------------------------------
	int LuaCoroutineInvoker::Pasue()
	{
		return lua_yield(m_pCorouLuaState, 0);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::Resume()
	{
		ASSERT(m_isResumeInitalized);
		(this->*m_Invoker)(m_InputArray, m_ResumePusherArray);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::_DoResume(void** p1)
	{
		ASSERT(m_isResumeInitalized);
		m_InputArray[0] = p1;
		(this->*m_Invoker)(m_InputArray, m_ResumePusherArray);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::_DoResume(void** p1, void** p2)
	{
		ASSERT(m_isResumeInitalized);
		m_InputArray[0] = p1;
		m_InputArray[1] = p2;
		(this->*m_Invoker)(m_InputArray, m_ResumePusherArray);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::_DoResume(void** p1, void** p2, void** p3)
	{
		ASSERT(m_isResumeInitalized);
		m_InputArray[0] = p1;
		m_InputArray[1] = p2;
		m_InputArray[2] = p3;
		(this->*m_Invoker)(m_InputArray, m_ResumePusherArray);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::_DoResume(void** p1, void** p2, void** p3, void** p4)
	{
		ASSERT(m_isResumeInitalized);
		m_InputArray[0] = p1;
		m_InputArray[1] = p2;
		m_InputArray[2] = p3;
		m_InputArray[3] = p4;
		(this->*m_Invoker)(m_InputArray, m_ResumePusherArray);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::Invoke()
	{
		(this->*m_Invoker)(m_InputArray, m_InvokerPusherArray);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::_DoInvoke(void** p1)
	{
		m_InputArray[0] = p1;
		(this->*m_Invoker)(m_InputArray, m_InvokerPusherArray);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::_DoInvoke(void** p1, void** p2)
	{
		m_InputArray[0] = p1;
		m_InputArray[1] = p2;
		(this->*m_Invoker)(m_InputArray, m_InvokerPusherArray);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::_DoInvoke(void** p1, void** p2, void** p3)
	{
		m_InputArray[0] = p1;
		m_InputArray[1] = p2;
		m_InputArray[2] = p3;
		(this->*m_Invoker)(m_InputArray, m_InvokerPusherArray);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::_DoInvoke(void** p1, void** p2, void** p3, void** p4)
	{
		m_InputArray[0] = p1;
		m_InputArray[1] = p2;
		m_InputArray[2] = p3;
		m_InputArray[3] = p4;
		(this->*m_Invoker)(m_InputArray, m_InvokerPusherArray);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaCoroutineInvoker::_DoInvoke(void** p1, void** p2, void** p3, void** p4, void** p5)
	{
		m_InputArray[0] = p1;
		m_InputArray[1] = p2;
		m_InputArray[2] = p3;
		m_InputArray[3] = p4;
		m_InputArray[4] = p5;
		(this->*m_Invoker)(m_InputArray, m_InvokerPusherArray);
	}
}