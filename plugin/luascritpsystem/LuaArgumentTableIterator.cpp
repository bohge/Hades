#include "LuaArgumentTableIterator.h"
extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}


namespace hlss
{
	//---------------------------------------------------------------------------------------------------------
	LuaArgumentTableIterator::LuaArgumentTableIterator(void)
		:LuaMemberIterator(-1, NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LuaArgumentTableIterator::~LuaArgumentTableIterator(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaArgumentTableIterator::Being() const
	{
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushnil(*m_rppLuaState);
	}
}