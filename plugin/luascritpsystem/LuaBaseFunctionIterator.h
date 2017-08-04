#pragma once
#include "LuaFunctionIterator.h"


#include <EASTL/vector.h>

struct lua_State;
namespace hlss
{
	class LuaBaseFunctionIterator : public LuaFunctionIterator
	{
	public:
		LuaBaseFunctionIterator(int index, lua_State** l);
		virtual ~LuaBaseFunctionIterator();
	};
}
