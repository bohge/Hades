#pragma once
#include "LuaInstance.h"


#include <EASTL/map.h>

struct lua_State;
namespace hlss
{
	class LuaMemberIterator;
	class LuaFunctionIterator;
	class LuaRootInstance : public LuaInstance
	{
	private:
		typedef eastl::map<uint, int>	CoroutinesFunctions;
	private:
		CoroutinesFunctions		m_CoroutinesFunctions;
		LuaMemberIterator*		m_pLuaMemberIterator;
		LuaFunctionIterator*	m_pLuaFunctionIterator;
		int						m_InstanceHandle;
	public:
		LuaRootInstance(uint host, uint hash, lua_State** state, LuaVirtualMachine* vm);
		virtual ~LuaRootInstance();
	private:
		virtual hc::IFunctionInvoker* _DoSignFunciton(const eastl::string& name, const hc::ScriptProperty::Arguments& args);
		virtual hc::IFunctionInvoker* _DoSignFunciton(int userdata, const hc::ScriptProperty::Arguments& args);
		virtual hc::IFunctionInvoker* _DoSignFunciton(const eastl::string& name, uint res, const hc::ScriptProperty::Arguments& args);
		virtual hc::IFunctionInvoker* _DoSignFunciton(int userdata, uint res, const hc::ScriptProperty::Arguments& args);
		virtual void _DoReleaseFunction(hc::IFunctionInvoker* func);
		virtual hc::ICoroutineInvoker* _DoSignCoroutine(const eastl::string& name, const hc::ScriptProperty::Arguments& args);
		virtual hc::ICoroutineInvoker* _DoSignCoroutine(const eastl::string& name, uint res, const hc::ScriptProperty::Arguments& args);
		virtual void _DoReleaseCoroutine(hc::ICoroutineInvoker* coro);
		virtual void _DoClearMember();
	public:
		virtual void SetMember(const char* name);
		virtual void SetMember(const char* name, bool obj);
		virtual void SetMember(const char* name, short obj);
		virtual void SetMember(const char* name, ushort obj);
		virtual void SetMember(const char* name, int obj);
		virtual void SetMember(const char* name, uint obj);
		virtual void SetMember(const char* name, float obj);
		virtual void SetMember(const char* name, int64 obj);
		virtual void SetMember(const char* name, uint64 obj);
		virtual void SetMember(const char* name, double obj);
		virtual void SetMember(const char* name, const char* obj, uint len);
		virtual void SetMember(const char* name, hc::Object* obj, hc::ScriptValue::ClearFunction func);
		virtual void BeginSetMemberBulk();
		virtual void EndSetMemberBulk();
		virtual const void* GetMember(const char* name);
		virtual const hc::IMemberIterator& GetMemberIterator();
		virtual const hc::IFunctionIterator& GetFunctionIterator();
		virtual int GetScriptInstance();
		virtual void GotError(const eastl::string& error);
	public:
		virtual void PushInstance();
		virtual void PopInstance();
	private:
		int _GetFunctionRefrence(const eastl::string& name);
	public:
		void Initialize(int refindex);
		bool Initialize(const eastl::string& file, const eastl::string& buffer);
		void Exit();
	};
}
