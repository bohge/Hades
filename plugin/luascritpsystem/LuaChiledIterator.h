#pragma once
#include "LuaMemberIterator.h"

namespace hlss
{
	class LuaChiledIterator : public LuaMemberIterator
	{
	public:
		LuaChiledIterator(int index, lua_State** l);
		virtual ~LuaChiledIterator();
	public:
		virtual void Being() const;
	};
}