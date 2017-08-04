#include "script/ClassRegister.h"
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
	LuaInstance::LuaInstance(uint host, uint hash, lua_State** state, LuaVirtualMachine* vm)
		:IScriptInstance(host, hash)
		, m_rppLuaState(state)
		, m_isContinuously(false)
		, m_rpVirtualMachine(vm)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	LuaInstance::~LuaInstance()
	{
		FOR_EACH(ChieldMap, it, m_ChieldMap)
		{
			SAFE_DELETE(it->second);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(const char* name)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushnil(*m_rppLuaState);
		lua_setfield(*m_rppLuaState, -2, name);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(const char* name, int obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushinteger(*m_rppLuaState, obj);
		lua_setfield(*m_rppLuaState, -2, name);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(const char* name, float obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushnumber(*m_rppLuaState, obj);
		lua_setfield(*m_rppLuaState, -2, name);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(const char* name, bool obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushboolean(*m_rppLuaState, obj);
		lua_setfield(*m_rppLuaState, -2, name);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(const char* name, short obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushinteger(*m_rppLuaState, obj);
		lua_setfield(*m_rppLuaState, -2, name);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(const char* name, ushort obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushinteger(*m_rppLuaState, obj);
		lua_setfield(*m_rppLuaState, -2, name);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(const char* name, uint obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushinteger(*m_rppLuaState, obj);
		lua_setfield(*m_rppLuaState, -2, name);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(const char* name, int64 obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		int64* ud = (int64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_INT64, sizeof(int64));
		*ud = obj;
		lua_setfield(*m_rppLuaState, -2, name);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(const char* name, uint64 obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		uint64* ud = (uint64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_UINT64, sizeof(uint64));
		*ud = obj;
		lua_setfield(*m_rppLuaState, -2, name);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(const char* name, double obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushnumber(*m_rppLuaState, obj);
		lua_setfield(*m_rppLuaState, -2, name);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(const char* name, const char* obj, uint len)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushlstring(*m_rppLuaState, obj, len);
		lua_setfield(*m_rppLuaState, -2, name);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(const char* name, hc::Object* obj, hc::ScriptValue::ClearFunction func)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushstring(*m_rppLuaState, name);//将名称推入堆栈
		ScriptValue* userdata = (ScriptValue*)lua_newuserdata(*m_rppLuaState, sizeof(ScriptValue));
		userdata->SetValue(obj, func);
		lua_getglobal(*m_rppLuaState, obj->GetTypeName());
		if (!lua_istable(*m_rppLuaState, -1))
		{
			lua_pop(*m_rppLuaState, 1);
			if (obj->isTypeOrDriverType(hc::IAgent::RTTI()))
			{
				hc::IAgent* agent = static_cast<hc::IAgent*>(obj);
				hc::ClassRegister* clr = agent->GetClassInfomation();
				m_rpVirtualMachine->ReferenceClass(clr);
				lua_getglobal(*m_rppLuaState, obj->GetTypeName());
				ASSERT(lua_istable(*m_rppLuaState, -1));
				lua_setmetatable(*m_rppLuaState, -2);
			}
		}
		else
		{
			lua_setmetatable(*m_rppLuaState, -2);
		}
		lua_rawset(*m_rppLuaState, -3);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(int key)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushnil(*m_rppLuaState);
		lua_rawseti(*m_rppLuaState, -2, key);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(int key, bool obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushboolean(*m_rppLuaState, obj);
		lua_rawseti(*m_rppLuaState, -2, key);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(int key, short obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushinteger(*m_rppLuaState, obj);
		lua_rawseti(*m_rppLuaState, -2, key);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(int key, ushort obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushinteger(*m_rppLuaState, obj);
		lua_rawseti(*m_rppLuaState, -2, key);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(int key, int obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushinteger(*m_rppLuaState, obj);
		lua_rawseti(*m_rppLuaState, -2, key);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(int key, uint obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushinteger(*m_rppLuaState, obj);
		lua_rawseti(*m_rppLuaState, -2, key);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(int key, float obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushnumber(*m_rppLuaState, obj);
		lua_rawseti(*m_rppLuaState, -2, key);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(int key, int64 obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		int64* ud = (int64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_INT64, sizeof(int64));
		*ud = obj;
		lua_rawseti(*m_rppLuaState, -2, key);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(int key, uint64 obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		uint64* ud = (uint64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_UINT64, sizeof(uint64));
		*ud = obj;
		lua_rawseti(*m_rppLuaState, -2, key);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(int key, double obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushnumber(*m_rppLuaState, obj);
		lua_rawseti(*m_rppLuaState, -2, key);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(int key, const char* obj, uint len)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushlstring(*m_rppLuaState, obj, len);
		lua_rawseti(*m_rppLuaState, -2, key);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(int key, hc::Object* obj, hc::ScriptValue::ClearFunction func)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		ScriptValue* userdata = (ScriptValue*)lua_newuserdata(*m_rppLuaState, sizeof(ScriptValue));
		userdata->SetValue(obj, func);
		lua_getglobal(*m_rppLuaState, obj->GetTypeName());
		if (!lua_istable(*m_rppLuaState, -1))
		{
			lua_pop(*m_rppLuaState, 1);
			if (obj->isTypeOrDriverType(hc::IAgent::RTTI()))
			{
				hc::IAgent* agent = static_cast<hc::IAgent*>(obj);
				hc::ClassRegister* clr = agent->GetClassInfomation();
				m_rpVirtualMachine->ReferenceClass(clr);
				lua_getglobal(*m_rppLuaState, obj->GetTypeName());
				ASSERT(lua_istable(*m_rppLuaState, -1));
				lua_setmetatable(*m_rppLuaState, -2);
			}
		}
		else
		{
			lua_setmetatable(*m_rppLuaState, -2);
		}
		lua_rawseti(*m_rppLuaState, -2, key);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(uint64 key)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		uint64* res = (uint64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_UINT64, sizeof(uint64));
		*res = key;
		lua_pushnil(*m_rppLuaState);
		lua_settable(*m_rppLuaState, -3);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(uint64 key, bool obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		uint64* res = (uint64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_UINT64, sizeof(uint64));
		*res = key;
		lua_pushboolean(*m_rppLuaState, obj);
		lua_settable(*m_rppLuaState, -3);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(uint64 key, short obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		uint64* res = (uint64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_UINT64, sizeof(uint64));
		*res = key;
		lua_pushinteger(*m_rppLuaState, obj);
		lua_settable(*m_rppLuaState, -3);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(uint64 key, ushort obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		uint64* res = (uint64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_UINT64, sizeof(uint64));
		*res = key;
		lua_pushinteger(*m_rppLuaState, obj);
		lua_settable(*m_rppLuaState, -3);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(uint64 key, int obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		uint64* res = (uint64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_UINT64, sizeof(uint64));
		*res = key;
		lua_pushinteger(*m_rppLuaState, obj);
		lua_settable(*m_rppLuaState, -3);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(uint64 key, uint obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		uint64* res = (uint64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_UINT64, sizeof(uint64));
		*res = key;
		lua_pushinteger(*m_rppLuaState, obj);
		lua_settable(*m_rppLuaState, -3);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(uint64 key, float obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		uint64* res = (uint64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_UINT64, sizeof(uint64));
		*res = key;
		lua_pushnumber(*m_rppLuaState, obj);
		lua_settable(*m_rppLuaState, -3);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(uint64 key, int64 obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		uint64* res = (uint64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_UINT64, sizeof(uint64));
		*res = key;
		int64* ud = (int64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_INT64, sizeof(int64));
		*ud = obj;
		lua_settable(*m_rppLuaState, -3);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(uint64 key, uint64 obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		uint64* res = (uint64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_UINT64, sizeof(uint64));
		*res = key;
		uint64* ud = (uint64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_UINT64, sizeof(uint64));
		*ud = obj;
		lua_settable(*m_rppLuaState, -3);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(uint64 key, double obj)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		uint64* res = (uint64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_UINT64, sizeof(uint64));
		*res = key;
		lua_pushnumber(*m_rppLuaState, obj);
		lua_settable(*m_rppLuaState, -3);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(uint64 key, const char* obj, uint len)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		uint64* res = (uint64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_UINT64, sizeof(uint64));
		*res = key;
		lua_pushlstring(*m_rppLuaState, obj, len);
		lua_settable(*m_rppLuaState, -3);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaInstance::ContinuouslySetMember(uint64 key, hc::Object* obj, hc::ScriptValue::ClearFunction func)
	{
		ASSERT(m_isContinuously);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		uint64* res = (uint64*)luaL_pushcdata(*m_rppLuaState, MY_CTID_UINT64, sizeof(uint64));
		*res = key;
		ScriptValue* userdata = (ScriptValue*)lua_newuserdata(*m_rppLuaState, sizeof(ScriptValue));
		userdata->SetValue(obj, func);
		lua_getglobal(*m_rppLuaState, obj->GetTypeName());
		if (!lua_istable(*m_rppLuaState, -1))
		{
			lua_pop(*m_rppLuaState, 1);
			if (obj->isTypeOrDriverType(hc::IAgent::RTTI()))
			{
				hc::IAgent* agent = static_cast<hc::IAgent*>(obj);
				hc::ClassRegister* clr = agent->GetClassInfomation();
				m_rpVirtualMachine->ReferenceClass(clr);
				lua_getglobal(*m_rppLuaState, obj->GetTypeName());
				ASSERT(lua_istable(*m_rppLuaState, -1));
				lua_setmetatable(*m_rppLuaState, -2);
			}
		}
		else
		{
			lua_setmetatable(*m_rppLuaState, -2);
		}
		lua_settable(*m_rppLuaState, -3);
	}	
	//---------------------------------------------------------------------------------------------------------
	hc::IScriptInstance* LuaInstance::CreateChieldInstance(const char* name)
	{
		if (!_isContinuously())
		{
			BeginSetMemberBulk();
		}
		lua_getfield(*_GetState(), -1, name);
		if (lua_istable(*_GetState(), -1))
		{
			lua_pop(*_GetState(), 1);
		}
		else
		{
			lua_pop(*_GetState(), 1);
			lua_newtable(*_GetState());
			lua_setfield(*_GetState(), -2, name);
		}
		if (!_isContinuously())
		{
			EndSetMemberBulk();
		}
		hc::IScriptInstance* res = NULL;
		uint hash = Utility::HashCode(name);
		ChieldMap::iterator it = m_ChieldMap.find(hash);
		if (m_ChieldMap.end() != it)
		{
			res = it->second;
		}
		else
		{
			res = NEW LuaChieldInstance(GetMachineIndex(), -1, _GetState(), this, m_rpVirtualMachine, name);
			m_ChieldMap.insert(eastl::make_pair(hash, res));
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IScriptInstance* LuaInstance::CreateChieldInstance(int key)
	{
		if (!_isContinuously())
		{
			BeginSetMemberBulk();
		}
		lua_rawgeti(*_GetState(), -1, key);
		if (lua_istable(*_GetState(), -1))
		{
			lua_pop(*_GetState(), 1);
		}
		else
		{
			lua_pop(*_GetState(), 1);
			lua_newtable(*_GetState());
			lua_rawseti(*_GetState(), -2, key);
		}
		if (!_isContinuously())
		{
			EndSetMemberBulk();
		}
		hc::IScriptInstance* res = NULL;
		ChieldMap::iterator it = m_ChieldMap.find(key);
		if (m_ChieldMap.end() != it)
		{
			res = it->second;
		}
		else
		{
			res = NEW LuaChieldInstance(GetMachineIndex(), -1, _GetState(), this, m_rpVirtualMachine, key);
			m_ChieldMap.insert(eastl::make_pair(key, res));
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	IScriptInstance* LuaInstance::CreateChieldInstance(uint64 key)
	{
		if (!_isContinuously())
		{
			BeginSetMemberBulk();
		}
		uint64* cdata = (uint64*)luaL_pushcdata(*_GetState(), MY_CTID_UINT64, sizeof(uint64));
		*cdata = key;
		lua_gettable(*_GetState(), -2);
		if (lua_istable(*_GetState(), -1))
		{
			lua_pop(*_GetState(), 2);
		}
		else
		{
			lua_pop(*_GetState(), 1);
			lua_newtable(*_GetState());
			lua_settable(*_GetState(), -3);
		}
		if (!_isContinuously())
		{
			EndSetMemberBulk();
		}
		hc::IScriptInstance* res = NULL;
		ChieldMap::iterator it = m_ChieldMap.find(key);
		if (m_ChieldMap.end() != it)
		{
			res = it->second;
		}
		else
		{
			res = NEW LuaChieldInstance(GetMachineIndex(), -1, _GetState(), this, m_rpVirtualMachine, key);
			m_ChieldMap.insert(eastl::make_pair(key, res));
		}
		return res;
	}
}