#include "LuaArgumentInstance.h"



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
	LuaArgumentInstance::LuaArgumentInstance(void)
		:LuaChieldInstance(nullhandle, nullhandle, NULL, this, NULL, nullid)
		, m_ChileAgent(NULL)
		, m_isChield(false)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	LuaArgumentInstance::LuaArgumentInstance(bool ischield)
		:LuaChieldInstance(nullhandle, nullhandle, NULL, this, NULL, nullid)
		, m_ChileAgent(NULL)
		, m_isChield(ischield)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	LuaArgumentInstance::~LuaArgumentInstance()
	{
		SAFE_DELETE(m_ChileAgent);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaArgumentInstance::BeginSetMemberBulk()
	{
		_SetContinuously(true);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaArgumentInstance::EndSetMemberBulk()
	{
		if (m_isChield)
		{
			lua_pop(*_GetState(), 1);
		}
		_SetContinuously(false);
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IScriptInstance* LuaArgumentInstance::CreateChieldInstance(const char* name)
	{
		ASSERT(lua_istable(*_GetState(), -1));
		lua_newtable(*_GetState());
		lua_setfield(*_GetState(), -2, name);
		lua_getfield(*_GetState(), -1, name);
		if (NULL == m_ChileAgent)
		{
			m_ChileAgent = NEW LuaArgumentInstance(true);
		}
		m_ChileAgent->SetState(_GetState());
		return m_ChileAgent;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IScriptInstance* LuaArgumentInstance::CreateChieldInstance(int key)
	{
		ASSERT(lua_istable(*_GetState(), -1));
		lua_newtable(*_GetState());
		lua_rawseti(*_GetState(), -2, key);
		lua_rawgeti(*_GetState(), -1, key);
		if (NULL == m_ChileAgent)
		{
			m_ChileAgent = NEW LuaArgumentInstance(true);
		}
		m_ChileAgent->SetState(_GetState());
		return m_ChileAgent;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IScriptInstance* LuaArgumentInstance::CreateChieldInstance(uint64 key)
	{
		ASSERT(lua_istable(*_GetState(), -1));
		uint64* cdata = (uint64*)luaL_pushcdata(*_GetState(), MY_CTID_UINT64, sizeof(uint64));
		*cdata = key;
		lua_newtable(*_GetState());
		lua_settable(*_GetState(), -3);
		cdata = (uint64*)luaL_pushcdata(*_GetState(), MY_CTID_UINT64, sizeof(uint64));
		*cdata = key;
		lua_gettable(*_GetState(), -2);
		if (NULL == m_ChileAgent)
		{
			m_ChileAgent = NEW LuaArgumentInstance(true);
		}
		m_ChileAgent->SetState(_GetState());
		return m_ChileAgent;
	}
}