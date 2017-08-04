#pragma once
#include "LuaInstance.h"


#include <EASTL/map.h>

struct lua_State;
namespace hlss
{
	class LuaMemberIterator;
	class LuaChieldInstance : public LuaInstance
	{
	private:
		class IFieldGeter
		{
		public:
			IFieldGeter(){}
			virtual ~IFieldGeter(){}
		public:
			virtual void GetField(lua_State* l, int tableindex) = 0;
		};
		class IntFieldGeter : public IFieldGeter
		{
		private:
			int		m_Key;
		public:
			IntFieldGeter(int key) :m_Key(key){}
			virtual ~IntFieldGeter(){}
		public:
			virtual void GetField(lua_State* l, int tableindex);
		};
		class StringFieldGeter : public IFieldGeter
		{
		private:
			eastl::string		m_Key;
		public:
			StringFieldGeter(const eastl::string& key) :m_Key(key){}
			virtual ~StringFieldGeter(){}
		public:
			virtual void GetField(lua_State* l, int tableindex);
		};
		class UInt64FieldGeter : public IFieldGeter
		{
		private:
			uint64		m_Key;
		public:
			UInt64FieldGeter(uint64 key) :m_Key(key){}
			virtual ~UInt64FieldGeter(){}
		public:
			virtual void GetField(lua_State* l, int tableindex);
		};
		LuaInstance*		m_RootInstance;
		IFieldGeter*		m_pIFieldGeter;		
	public:
		LuaChieldInstance(uint host, uint hash, lua_State** state, LuaInstance* root, LuaVirtualMachine* vm, int key);
		LuaChieldInstance(uint host, uint hash, lua_State** state, LuaInstance* root, LuaVirtualMachine* vm, const char* name);
		LuaChieldInstance(uint host, uint hash, lua_State** state, LuaInstance* root, LuaVirtualMachine* vm, uint64 name);
		virtual ~LuaChieldInstance();
	private:
		virtual bool _isFunctionExist(const eastl::string& name);
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
		void Exit();		
	};
}
