#pragma once
#include "script/ICoroutineInvoker.h"
#include "script/ScriptProperty.h"
#include "script/ScriptValue.h"

#include <EASTL/vector.h>

struct lua_State;
namespace hlss
{
	class LuaVirtualMachine;
	class LuaCoroutineInvoker : public hc::ICoroutineInvoker
	{
	private:
		typedef int(*PushArgumentsFunc)(LuaVirtualMachine*, lua_State*, void*);
		typedef int(*ReturnFunc)(void*&, LuaVirtualMachine*, lua_State*, int);
	private:
		typedef eastl::vector< PushArgumentsFunc > PusherArray;
		typedef eastl::vector< void** > InputArray;
	private:
		typedef void (LuaCoroutineInvoker::*InvokFunc)(InputArray& input, PusherArray& args);
	private:
		PusherArray			m_InvokerPusherArray;
		PusherArray			m_ResumePusherArray;
		InputArray			m_InputArray;
		InvokFunc			m_Invoker;
		ReturnFunc			m_ReturnFunc;
		lua_State**			m_rppRootState;
		lua_State*			m_pCorouLuaState;
		LuaVirtualMachine*	m_rpVirtualMachine;
		int					m_StateReferences;
		//int				m_ClassReferences;
		int					m_FunctionReferences;
		DEBUGCODE(bool		m_isResumeInitalized;)
	public:
		LuaCoroutineInvoker(lua_State** root, LuaVirtualMachine* vm);
		virtual ~LuaCoroutineInvoker(void);
	private:
		LuaCoroutineInvoker(const LuaCoroutineInvoker&){};
		const LuaCoroutineInvoker& operator = (const LuaCoroutineInvoker&){};
	private:
		void _InitArguments(PusherArray& pusherarray, const hc::ScriptProperty::Arguments& args);
		void _InitReturnFunction(uint sid);
		void _InvokeWithResults(InputArray& input, PusherArray& pushers);
		void _InvokeNoResults(InputArray& input, PusherArray& pushers);
		void _InitReferences(int cr, int fn);
	private:
		virtual void _SetupResume(const hc::ScriptProperty::Arguments& args);
	private:
		virtual void _DoResume(void** p1);
		virtual void _DoResume(void** p1, void** p2);
		virtual void _DoResume(void** p1, void** p2, void** p3);
		virtual void _DoResume(void** p1, void** p2, void** p3, void** p4);
		virtual void _DoInvoke(void** p1);
		virtual void _DoInvoke(void** p1, void** p2);
		virtual void _DoInvoke(void** p1, void** p2, void** p3);
		virtual void _DoInvoke(void** p1, void** p2, void** p3, void** p4);
		virtual void _DoInvoke(void** p1, void** p2, void** p3, void** p4, void** p5);
	public:
		virtual int Pasue();
		virtual void Resume();
		virtual void Invoke();
		virtual void GotError(const eastl::string& error);
	public:
		void Initialize(int cr, int fn, const hc::ScriptProperty::Arguments& args);
		void Initialize(int cr, int fn, uint res, const hc::ScriptProperty::Arguments& args);
		void Exit();
	};
}
