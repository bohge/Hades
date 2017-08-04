#include "script/ClassRegister.h"
#include "LuaFunctionIterator.h"
#include "LuaCoroutineInvoker.h"
#include "LuaFunctionInvoker.h"
#include "LuaVirtualMachine.h"
#include "LuaChieldInstance.h"
#include "LuaMemberIterator.h"
#include "file/IFileSystem.h"
#include "LuaDobufferHelp.h"
#include "services/IAgent.h"
#include "LuaRootInstance.h"
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
	LuaRootInstance::LuaRootInstance(uint host, uint hash, lua_State** state, LuaVirtualMachine* vm)
		:LuaInstance(host, hash, state, vm)
		, m_pLuaMemberIterator(NULL)
		, m_pLuaFunctionIterator(NULL)
		, m_InstanceHandle(hc::nullhandle)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	LuaRootInstance::~LuaRootInstance()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::Initialize(int refindex)
	{
		DEBUGCODE(int t = lua_gettop(*_GetState()));
		m_InstanceHandle = refindex;
		m_pLuaMemberIterator = NEW LuaMemberIterator(m_InstanceHandle, _GetState());
		m_pLuaFunctionIterator = NEW LuaFunctionIterator(m_InstanceHandle, _GetState());
		m_pLuaFunctionIterator->InitializeBaseIterator();
		ASSERT(t == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	bool LuaRootInstance::Initialize(const eastl::string& file, const eastl::string& buffer)
	{
		bool res = true;
		string filename = file.substr(IFileSystem::IO_SUBPATH_POS);
		string classname = filename.substr(0, filename.find_first_of("."));
		classname = classname.substr(classname.find_last_of("/\\") + 1);
		_SetClassName(classname);

		DEBUGCODE(int t = lua_gettop(*_GetState()));
		int ref = DoBuffer(*_GetState(), classname.c_str(), filename.c_str(), buffer.c_str(), buffer.size());
		if (-1 == ref)
		{
			res = false;
		}
		else
		{
			m_InstanceHandle = ref;
			m_pLuaMemberIterator = NEW LuaMemberIterator(m_InstanceHandle, _GetState());
			m_pLuaFunctionIterator = NEW LuaFunctionIterator(m_InstanceHandle, _GetState());
			m_pLuaFunctionIterator->InitializeBaseIterator();
		}
		ASSERT(t == lua_gettop(*_GetState()));
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::Exit()
	{
		SAFE_DELETE(m_pLuaMemberIterator);
		SAFE_DELETE(m_pLuaFunctionIterator);
		if (*_GetState())
		{
			ReleaseAllInvoker();
			if (hc::nullhandle != m_InstanceHandle)
			{
				luaL_unref(*_GetState(), LUA_REGISTRYINDEX, m_InstanceHandle);
			}
			FOR_EACH(CoroutinesFunctions, it, m_CoroutinesFunctions)
			{
				luaL_unref(*_GetState(), LUA_REGISTRYINDEX, it->second);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IFunctionInvoker* LuaRootInstance::_DoSignFunciton(const eastl::string& name, const ScriptProperty::Arguments& args)
	{
		LuaFunctionInvoker* func = NEW LuaFunctionInvoker(_GetState(), m_rpVirtualMachine);
		func->Initialize(m_InstanceHandle, name.c_str(), args);
		return func;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IFunctionInvoker* LuaRootInstance::_DoSignFunciton(int userdata, const ScriptProperty::Arguments& args)
	{
		LuaFunctionInvoker* func = NEW LuaFunctionInvoker(_GetState(), m_rpVirtualMachine);
		func->Initialize(userdata, args);
		return func;
	}
	//---------------------------------------------------------------------------------------------------------
	IFunctionInvoker* LuaRootInstance::_DoSignFunciton(const eastl::string& name, uint res, const ScriptProperty::Arguments& args)
	{
		LuaFunctionInvoker* func = NEW LuaFunctionInvoker(_GetState(), m_rpVirtualMachine);
		func->Initialize(m_InstanceHandle, name.c_str(), res, args);
		return func;
	}
	//---------------------------------------------------------------------------------------------------------
	IFunctionInvoker* LuaRootInstance::_DoSignFunciton(int userdata, uint res, const ScriptProperty::Arguments& args)
	{
		LuaFunctionInvoker* func = NEW LuaFunctionInvoker(_GetState(), m_rpVirtualMachine);
		func->Initialize(userdata, res, args);
		return func;
	}
	//---------------------------------------------------------------------------------------------------------
	//hc::IFunctionInvoker* LuaRootInstance::_DoSignFunciton(int userdata, uint argCount)
	//{
	//	LuaFunctionInvoker* func = NEW LuaFunctionInvoker(m_InstanceHandle, _GetState());
	//	func->Initialize(userdata, argCount);
	//	return func;
	//}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::_DoReleaseFunction(hc::IFunctionInvoker* func)
	{
		LuaFunctionInvoker* lfunc = static_cast<LuaFunctionInvoker*>(func);
		lfunc->Exit();
		SAFE_DELETE(lfunc);
	}
	//---------------------------------------------------------------------------------------------------------
	int LuaRootInstance::_GetFunctionRefrence(const eastl::string& name)
	{
		int res;
		uint hash = Utility::HashCode(name);
		CoroutinesFunctions::iterator it = m_CoroutinesFunctions.find(hash);
		if (m_CoroutinesFunctions.end() == it)
		{
			for (const LuaFunctionIterator* fit = m_pLuaFunctionIterator;
				NULL != fit;
				fit = (const LuaFunctionIterator*)fit->GetBaseFunctionIterator())
			{
				if (fit->isFunctionExist(name))
				{
					res = fit->ReferenceFunction(name);
					break;
				}
			}
			m_CoroutinesFunctions.insert(make_pair(hash, res));
		}
		else
		{
			res = it->second;
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::ICoroutineInvoker* LuaRootInstance::_DoSignCoroutine(const eastl::string& name, const hc::ScriptProperty::Arguments& args)
	{
		int fn = _GetFunctionRefrence(name);
		LuaCoroutineInvoker* cores = new LuaCoroutineInvoker(_GetState(), m_rpVirtualMachine);
		cores->Initialize(m_InstanceHandle, fn, args);
		return cores;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::ICoroutineInvoker* LuaRootInstance::_DoSignCoroutine(const eastl::string& name, uint res, const hc::ScriptProperty::Arguments& args)
	{
		int fn = _GetFunctionRefrence(name);
		LuaCoroutineInvoker* cores = new LuaCoroutineInvoker(_GetState(), m_rpVirtualMachine);
		cores->Initialize(m_InstanceHandle, fn, res, args);
		return cores;
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::_DoReleaseCoroutine(hc::ICoroutineInvoker* coro)
	{
		LuaCoroutineInvoker* lcoro = static_cast<LuaCoroutineInvoker*>(coro);
		lcoro->Exit();
		SAFE_DELETE(lcoro);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::SetMember(const char* name)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		lua_rawgeti(*_GetState(), LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushnil(*_GetState());
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::SetMember(const char* name, int obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		lua_rawgeti(*_GetState(), LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushinteger(*_GetState(), obj);
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::SetMember(const char* name, float obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		lua_rawgeti(*_GetState(), LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushnumber(*_GetState(), obj);
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::SetMember(const char* name, bool obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		lua_rawgeti(*_GetState(), LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushboolean(*_GetState(), obj);
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::SetMember(const char* name, short obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		lua_rawgeti(*_GetState(), LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushinteger(*_GetState(), obj);
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::SetMember(const char* name, ushort obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		lua_rawgeti(*_GetState(), LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushinteger(*_GetState(), obj);
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::SetMember(const char* name, uint obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		lua_rawgeti(*_GetState(), LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushinteger(*_GetState(), obj);
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::SetMember(const char* name, int64 obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		lua_rawgeti(*_GetState(), LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*_GetState(), -1));
		int64* ud = (int64*)luaL_pushcdata(*_GetState(), MY_CTID_INT64, sizeof(int64));
		*ud = obj;
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::SetMember(const char* name, uint64 obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		lua_rawgeti(*_GetState(), LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*_GetState(), -1));
		uint64* ud = (uint64*)luaL_pushcdata(*_GetState(), MY_CTID_UINT64, sizeof(uint64));
		*ud = obj;
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::SetMember(const char* name, double obj)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		lua_rawgeti(*_GetState(), LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushnumber(*_GetState(), obj);
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::SetMember(const char* name, const char* obj, uint len)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		lua_rawgeti(*_GetState(), LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushlstring(*_GetState(), obj, len);
		lua_setfield(*_GetState(), -2, name);
		lua_pop(*_GetState(), 1);
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::SetMember(const char* name, hc::Object* obj, hc::ScriptValue::ClearFunction func)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		lua_rawgeti(*_GetState(), LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*_GetState(), -1));
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
		lua_pop(*_GetState(), 1);
		ASSERT(0 == lua_gettop(*_GetState()));
	}
	//---------------------------------------------------------------------------------------------------------
	const void* LuaRootInstance::GetMember(const char* name)
	{
		ASSERT(0 == lua_gettop(*_GetState()));
		lua_rawgeti(*_GetState(), LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_getfield(*_GetState(), -1, name);
		const void* res = NULL;
		switch (lua_type(*_GetState(), -1))
		{
			//case LUA_TNUMBER: res = lua_tonumberpf(*_GetState(), -1); break;
			//case LUA_TBOOLEAN: res = lua_tobooleanpb(*_GetState(), -1); break;
		case LUA_TSTRING: res = lua_tostring(*_GetState(), -1); break;
		case LUA_TUSERDATA: res = lua_touserdata(*_GetState(), -1); break;
		default:ASSERT(false);
		}
		lua_pop(*_GetState(), 2);
		ASSERT(0 == lua_gettop(*_GetState()));
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::_DoClearMember()
	{
		DEBUGCODE(int t = lua_gettop(*_GetState()));
		lua_rawgeti(*_GetState(), LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*_GetState(), -1));
		lua_pushnil(*_GetState());
		while (lua_next(*_GetState(), -2) != 0)
		{
			int keytype = lua_type(*_GetState(), -2);
			if (LUA_TSTRING == keytype)
			{
				int valuetype = lua_type(*_GetState(), -1);
				const char* name = lua_tostring(*_GetState(), -2);
				if (name[0] != ScriptProperty::SCRIPT_INERL_PREFIX
					&& LUA_TTHREAD != valuetype
					&& LUA_TFUNCTION != valuetype)
				{
					const char* key = lua_tostring(*_GetState(), -2);
					lua_pushnil(*_GetState());
					lua_setfield(*_GetState(), -4, key);
				}
			}
			else if (LUA_TNUMBER == keytype)
			{
				int valuetype = lua_type(*_GetState(), -1);
				if (LUA_TTHREAD != valuetype
					&& LUA_TFUNCTION != valuetype)
				{
					int key = lua_tointeger(*_GetState(), -2);
					lua_pushnil(*_GetState());
					lua_rawseti(*_GetState(), -4, key);
				}
			}
			lua_pop(*_GetState(), 1);
		}
		lua_pop(*_GetState(), 1);
		ASSERT(lua_gettop(*_GetState()) == t);
	}
	//---------------------------------------------------------------------------------------------------------
	const IMemberIterator& LuaRootInstance::GetMemberIterator()
	{
		return *m_pLuaMemberIterator;
	}
	//---------------------------------------------------------------------------------------------------------
	const hc::IFunctionIterator& LuaRootInstance::GetFunctionIterator()
	{
		return *m_pLuaFunctionIterator;
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::BeginSetMemberBulk()
	{
		_SetContinuously(true);
		lua_rawgeti(*_GetState(), LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*_GetState(), -1));
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::EndSetMemberBulk()
	{
		lua_pop(*_GetState(), 1);
		_SetContinuously(false);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::PushInstance()
	{
		if (!_isContinuously())
		{
			BeginSetMemberBulk();
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::PopInstance()
	{
		if (!_isContinuously())
		{
			EndSetMemberBulk();
		}
	}
	//---------------------------------------------------------------------------------------------------------
	int LuaRootInstance::GetScriptInstance()
	{
		return m_InstanceHandle;
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaRootInstance::GotError(const eastl::string& error)
	{
		luaL_error(*_GetState(), error.c_str());
	}
}