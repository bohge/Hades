#pragma once
#include "script/IVirtualMachine.h"
#include "PrintScriptClassInfo.h"

#include <EASTL/set.h>
#include <EASTL/list.h>

//#define _PRINT_LUA_CLASS_INFO
struct lua_State;
namespace hlss
{
	class LuaVirtualMachine : public hc::IVirtualMachine
	{
	private:
		typedef eastl::set<uint>	LoadedLibrarySet;
		typedef eastl::list< hc::ClassRegister* > ScriptClassList;
		typedef eastl::list< uint > FreeIndexList;
	private:
		lua_State*			m_pLua_State;
		LoadedLibrarySet	m_LoadedLibrarySet;
		ScriptClassList		m_ScriptClassList;
		FreeIndexList		m_FreeIndexList;
		uint				m_CurrentIndex;
		int					m_ValueHolderIndex;
		int					m_LastSize;
#ifdef _PRINT_LUA_CLASS_INFO
		ScriptClassMap			m_ScriptClassMap;
#endif
	public:
		LuaVirtualMachine(uint index);
		virtual ~LuaVirtualMachine();
	private:
		virtual hc::IScriptInstance* _DoLoadScript(uint hash, const eastl::string& path);
		virtual hc::IScriptInstance* _DoRegisteInstance(uint hash, int scriptdata);
		virtual hc::IScriptInstance* _DoCreateInstance();
		virtual hc::IScriptInstance* _DoCreateInstance(uint hash, const eastl::string& name);
		virtual void _DoReleaseScript(hc::IScriptInstance* instance);
		virtual void _DoReferenceClass(hc::ClassRegister* c);
		virtual void _DoUpdate();
	public:
		virtual void AddScriptPath(const eastl::string& path);
		virtual void AddNativePath(const eastl::string& path);
		virtual bool isRegisteredClass(const eastl::string& name);
		virtual void SetGlobal(const char* name);
		virtual void SetGlobal(const char* name, int obj);
		virtual void SetGlobal(const char* name, float obj);
		virtual void SetGlobal(const char* name, bool obj);
		virtual void SetGlobal(const char* name, uint64 obj);
		virtual void SetGlobal(const char* name, const eastl::string& obj);
		virtual void SetGlobal(const char* name, hc::Object* obj, void* rec, hc::ScriptValue::ClearFunction func);
		virtual hc::Object* GetGlobal(const char* name);
		virtual int SetGlobalReference(uint64 id);
		virtual uint64 GetGlobalReference(int index);
		virtual void Initialize();
		virtual void Exit();
	private:
		void _EraseOldPaths();
		eastl::string _GetRealPath(const eastl::string& path);
		void _DoRegisterClass(const char* cn);
		void _DoRegisterInheritance(hc::ClassRegister* sc);
		const char* _FunctionNameMapping(const eastl::string& name);
		void _RegisterClassFunction(hc::ClassRegister* sc, int index, hc::ScriptProperty::FunctionType ft, const eastl::string& name);
		void _DoRegisterFunction(hc::ClassRegister* sc);
		void _DoRegisterConstant(hc::ClassRegister* sc);
	public:
		void LoadNativeLibrary(const eastl::string& name);
	};
}
