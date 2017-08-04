#include "LuaChiledIterator.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


namespace hlss
{
	//---------------------------------------------------------------------------------------------------------
	LuaChiledIterator::LuaChiledIterator(int index, lua_State** l)
		:LuaMemberIterator(index, l)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LuaChiledIterator::~LuaChiledIterator()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaChiledIterator::Being() const
	{
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushnil(*m_rppLuaState);
	}
}