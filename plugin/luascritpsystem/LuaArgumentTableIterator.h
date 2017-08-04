#pragma once
#include "LuaMemberIterator.h"

namespace hlss
{
	class LuaArgumentTableIterator : public LuaMemberIterator
	{
	public:
		LuaArgumentTableIterator(void);
		virtual ~LuaArgumentTableIterator(void);
	public:
		virtual void Being() const;
	public:
		HADES_FORCEINLINE void SetLuaState(lua_State** l);
	};



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LuaArgumentTableIterator::SetLuaState(lua_State** l)
	{
		m_rppLuaState = l;
	}
}