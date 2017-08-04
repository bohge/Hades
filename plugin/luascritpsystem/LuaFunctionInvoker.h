#pragma once
#include "script/IFunctionInvoker.h"
#include "script/ScriptProperty.h"
#include "script/ScriptValue.h"

#include <EASTL/vector.h>

struct lua_State;
namespace hlss
{
	class LuaVirtualMachine;
	class LuaFunctionInvoker : public hc::IFunctionInvoker
	{
	private:
		typedef int(*PushArgumentsFunc)(LuaVirtualMachine*, lua_State*, void*);
		typedef int(*ReturnFunc)(void*&, LuaVirtualMachine*, lua_State*, int);
	private:
		typedef eastl::vector< PushArgumentsFunc > PusherArray;
		typedef eastl::vector< void** > InputArray;
	private:
		typedef void (LuaFunctionInvoker::*InvokFunc)();
	private:
		PusherArray			m_PusherArray;
		InputArray			m_InputArray;
		InvokFunc			m_Invoker;
		ReturnFunc			m_ReturnFunc;
		lua_State**			m_rppLuaState;
		LuaVirtualMachine*	m_rpVirtualMachine;
		int					m_FuncitonReferences;//º¯ÊýµÄË÷Òý
		//int				m_ClassReferences;
	public:
		LuaFunctionInvoker(lua_State** state, LuaVirtualMachine* vm);
		virtual ~LuaFunctionInvoker(void);
	private:
		LuaFunctionInvoker(const LuaFunctionInvoker&){};
		const LuaFunctionInvoker& operator = (const LuaFunctionInvoker&){};
	private:
		void _InitArguments(const hc::ScriptProperty::Arguments& args);
		void _InitReturnFunction(uint sid);
		void _InvokeWithResults();
		void _InvokeNoResults();
		void _InitReferences(int cr, const char* fn);
	private:
		virtual void _DoInvoke(void** p1);
		virtual void _DoInvoke(void** p1, void** p2);
		virtual void _DoInvoke(void** p1, void** p2, void** p3);
		virtual void _DoInvoke(void** p1, void** p2, void** p3, void** p4);
	public:
		virtual void Invoke();
	public:
		void Initialize(int cr, const char* fn, const hc::ScriptProperty::Arguments& args);
		void Initialize(int ref, const hc::ScriptProperty::Arguments& args);
		void Initialize(int cr, const char* fn, uint res, const hc::ScriptProperty::Arguments& args);
		void Initialize(int ref, uint res, const hc::ScriptProperty::Arguments& args);
		void Exit();
	};
}
