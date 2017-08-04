#include "script/ClassRegister.h"
#include "LuaFunctionIterator.h"
#include "LuaFunctionInvoker.h"
#include "LuaVirtualMachine.h"
#include "LuaChieldInstance.h"
#include "LuaMemberIterator.h"
#include "file/IFileSystem.h"
#include "LuaDobufferHelp.h"
#include "services/IAgent.h"
#include "LuaInstance.h"


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
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::IntFieldGeter::GetField(lua_State* l, int tableindex)
	{
		lua_rawgeti(l, tableindex, m_Key);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::StringFieldGeter::GetField(lua_State* l, int tableindex)
	{
		lua_getfield(l, tableindex, m_Key.c_str());
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::UInt64FieldGeter::GetField(lua_State* l, int tableindex)
	{
		uint64* cdata = (uint64*)luaL_pushcdata(l, MY_CTID_UINT64, sizeof(uint64));
		*cdata = m_Key;
		lua_gettable(l, -2);
	}


	//---------------------------------------------------------------------------------------------------------
	LuaChieldInstance::LuaChieldInstance(uint host, uint hash, lua_State** state, LuaInstance* root, LuaVirtualMachine* vm, const char* name)
		:LuaInstance(host, hash, state, vm)
		, m_RootInstance(root)
		, m_pIFieldGeter(NEW StringFieldGeter(name))
	{

	}
	//---------------------------------------------------------------------------------------------------------
	LuaChieldInstance::LuaChieldInstance(uint host, uint hash, lua_State** state, LuaInstance* root, LuaVirtualMachine* vm, int key)
		:LuaInstance(host, hash, state, vm)
		, m_RootInstance(root)
		, m_pIFieldGeter(NEW IntFieldGeter(key))
	{

	}
	//---------------------------------------------------------------------------------------------------------
	LuaChieldInstance::LuaChieldInstance(uint host, uint hash, lua_State** state, LuaInstance* root, LuaVirtualMachine* vm, uint64 name)
		:LuaInstance(host, hash, state, vm)
		, m_RootInstance(root)
		, m_pIFieldGeter(NEW UInt64FieldGeter(name))
	{

	}
	//---------------------------------------------------------------------------------------------------------
	LuaChieldInstance::~LuaChieldInstance()
	{
		SAFE_DELETE(m_pIFieldGeter);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::Exit()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	bool LuaChieldInstance::_isFunctionExist(const eastl::string& name)
	{
		ASSERT(false);
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IFunctionInvoker* LuaChieldInstance::_DoSignFunciton(const eastl::string& name, const ScriptProperty::Arguments& args)
	{
		ASSERT(false);
		return NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IFunctionInvoker* LuaChieldInstance::_DoSignFunciton(int userdata, const ScriptProperty::Arguments& args)
	{
		ASSERT(false);
		return NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	IFunctionInvoker* LuaChieldInstance::_DoSignFunciton(const eastl::string& name, uint res, const ScriptProperty::Arguments& args)
	{
		ASSERT(false);
		return NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	IFunctionInvoker* LuaChieldInstance::_DoSignFunciton(int userdata, uint res, const ScriptProperty::Arguments& args)
	{
		ASSERT(false);
		return NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::_DoReleaseFunction(hc::IFunctionInvoker* func)
	{
		ASSERT(false);
	}
	//---------------------------------------------------------------------------------------------------------
	ICoroutineInvoker* LuaChieldInstance::_DoSignCoroutine(const eastl::string& name, const ScriptProperty::Arguments& args)
	{
		ASSERT(false);
		return NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	ICoroutineInvoker* LuaChieldInstance::_DoSignCoroutine(const eastl::string& name, uint res, const ScriptProperty::Arguments& args)
	{
		ASSERT(false);
		return NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::_DoReleaseCoroutine(ICoroutineInvoker* coro)
	{
		ASSERT(false);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::SetMember(const char* name)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		m_RootInstance->PushInstance();
		//lua_getfield(*_GetState(), -1, m_FieldName.c_str());
		m_pIFieldGeter->GetField(*_GetState(), -1);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushnil(*_GetState());
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		m_RootInstance->PopInstance();
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::SetMember(const char* name, int obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		m_RootInstance->PushInstance();
		//lua_getfield(*_GetState(), -1, m_FieldName.c_str());
		m_pIFieldGeter->GetField(*_GetState(), -1);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushinteger(*_GetState(), obj);
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		m_RootInstance->PopInstance();
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::SetMember(const char* name, float obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		m_RootInstance->PushInstance();
		//lua_getfield(*_GetState(), -1, m_FieldName.c_str());
		m_pIFieldGeter->GetField(*_GetState(), -1);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushnumber(*_GetState(), obj);
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		m_RootInstance->PopInstance();
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::SetMember(const char* name, short obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		m_RootInstance->PushInstance();
		//lua_getfield(*_GetState(), -1, m_FieldName.c_str());
		m_pIFieldGeter->GetField(*_GetState(), -1);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushinteger(*_GetState(), obj);
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		m_RootInstance->PopInstance();
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::SetMember(const char* name, ushort obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		m_RootInstance->PushInstance();
		//lua_getfield(*_GetState(), -1, m_FieldName.c_str());
		m_pIFieldGeter->GetField(*_GetState(), -1);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushinteger(*_GetState(), obj);
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		m_RootInstance->PopInstance();
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::SetMember(const char* name, uint obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		m_RootInstance->PushInstance();
		//lua_getfield(*_GetState(), -1, m_FieldName.c_str());
		m_pIFieldGeter->GetField(*_GetState(), -1);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushinteger(*_GetState(), obj);
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		m_RootInstance->PopInstance();
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::SetMember(const char* name, int64 obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		m_RootInstance->PushInstance();
		//lua_getfield(*_GetState(), -1, m_FieldName.c_str());
		m_pIFieldGeter->GetField(*_GetState(), -1);
		ASSERT(lua_istable(*_GetState(), -1));
		int64* ud = (int64*)luaL_pushcdata(*_GetState(), MY_CTID_INT64, sizeof(int64));
		*ud = obj;
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		m_RootInstance->PopInstance();
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::SetMember(const char* name, uint64 obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		m_RootInstance->PushInstance();
		//lua_getfield(*_GetState(), -1, m_FieldName.c_str());
		m_pIFieldGeter->GetField(*_GetState(), -1);
		ASSERT(lua_istable(*_GetState(), -1));
		uint64* ud = (uint64*)luaL_pushcdata(*_GetState(), MY_CTID_UINT64, sizeof(uint64));
		*ud = obj;
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		m_RootInstance->PopInstance();
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::SetMember(const char* name, double obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		m_RootInstance->PushInstance();
		//lua_getfield(*_GetState(), -1, m_FieldName.c_str());
		m_pIFieldGeter->GetField(*_GetState(), -1);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushnumber(*_GetState(), obj);
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		m_RootInstance->PopInstance();
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::SetMember(const char* name, bool obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		m_RootInstance->PushInstance();
		//lua_getfield(*_GetState(), -1, m_FieldName.c_str());
		m_pIFieldGeter->GetField(*_GetState(), -1);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushboolean(*_GetState(), obj);
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		m_RootInstance->PopInstance();
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::SetMember(const char* name, const char* obj, uint len)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		m_RootInstance->PushInstance();
		//lua_getfield(*_GetState(), -1, m_FieldName.c_str());
		m_pIFieldGeter->GetField(*_GetState(), -1);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushlstring(*_GetState(), obj, len);
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		m_RootInstance->PopInstance();
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::SetMember(const char* name, hc::Object* obj, hc::ScriptValue::ClearFunction func)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		m_RootInstance->PushInstance();
		//lua_getfield(*_GetState(), -1, m_FieldName.c_str());
		ScriptValue* userdata = (ScriptValue*)lua_newuserdata(*_GetState(), sizeof(ScriptValue));
		userdata->SetValue(obj, func);
		lua_getglobal(*_GetState(), obj->GetTypeName());
		if (!lua_istable(*_GetState(), -1))
		{
			lua_pop(*_GetState(), 1);
			if (obj->isTypeOrDriverType(hc::IAgent::RTTI()))
			{
				hc::IAgent* agent = static_cast<hc::IAgent*>(obj);
				hc::ClassRegister* clr = agent->GetClassInfomation();
				m_rpVirtualMachine->ReferenceClass(clr);
				lua_getglobal(*_GetState(), obj->GetTypeName());
				ASSERT(lua_istable(*_GetState(), -1));
				lua_setmetatable(*_GetState(), -2);
			}
		}
		else
		{
			lua_setmetatable(*_GetState(), -2);
		}
		lua_rawset(*_GetState(), -3);
		m_RootInstance->PopInstance();
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	const void* LuaChieldInstance::GetMember(const char* name)
	{
		ASSERT(false);
		return NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::_DoClearMember()
	{
		ASSERT(false);
	}
	//---------------------------------------------------------------------------------------------------------
	const IMemberIterator& LuaChieldInstance::GetMemberIterator()
	{
		ASSERT(false);
		return LuaMemberIterator(0, _GetState());
	}
	//---------------------------------------------------------------------------------------------------------
	const hc::IFunctionIterator& LuaChieldInstance::GetFunctionIterator()
	{
		ASSERT(false);
		return LuaFunctionIterator(0, _GetState());
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::BeginSetMemberBulk()
	{
		m_RootInstance->PushInstance();
		//lua_getfield(*_GetState(), -1, m_FieldName.c_str());
		m_pIFieldGeter->GetField(*_GetState(), -1);
		_SetContinuously(true);
		ASSERT(lua_istable(*_GetState(), -1));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::EndSetMemberBulk()
	{
		lua_pop(*_GetState(), 1);
		m_RootInstance->PopInstance();
		_SetContinuously(false);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::PushInstance()
	{
		m_RootInstance->PushInstance();
		if (!_isContinuously())
		{
			BeginSetMemberBulk();
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::PopInstance()
	{
		if (!_isContinuously())
		{
			EndSetMemberBulk();
		}
		m_RootInstance->PopInstance();
	}
	//---------------------------------------------------------------------------------------------------------
	int LuaChieldInstance::GetScriptInstance()
	{
		ASSERT(false);
		return nullhandle;
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChieldInstance::GotError(const eastl::string& error)
	{
		ASSERT(false);
	}
}