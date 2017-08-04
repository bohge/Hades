#include "script/ScriptProperty.h"
#include "script/ScriptValue.h"
#include "LuaMemberIterator.h"
#include "LuaChiledIterator.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

struct lua_State;
using namespace hc;
namespace hlss
{
	//---------------------------------------------------------------------------------------------------------
	LuaMemberIterator::LuaMemberIterator(int index, lua_State** l)
		:m_rppLuaState(l)
		, m_InstanceHandle(index)
		, m_ChiledIndex(0)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LuaMemberIterator::~LuaMemberIterator()
	{
		FOR_EACH(ChiledArray, it, m_ChiledArray)
		{
			SAFE_DELETE(*it);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaMemberIterator::Being() const
	{
		lua_rawgeti(*m_rppLuaState, LUA_REGISTRYINDEX, m_InstanceHandle);
		ASSERT(lua_istable(*m_rppLuaState, -1));
		lua_pushnil(*m_rppLuaState);
	}
	//---------------------------------------------------------------------------------------------------------
	bool LuaMemberIterator::Next() const
	{
		bool isnext;
		bool iscontinue;
		do 
		{
			iscontinue = false;
			isnext = lua_next(*m_rppLuaState, -2) != 0;
			if (isnext)
			{
				int keytype = lua_type(*m_rppLuaState, -2);
				int valuetype = lua_type(*m_rppLuaState, -1);
				if (LUA_TNONE == valuetype
					|| LUA_TFUNCTION == valuetype
					|| LUA_TTHREAD == valuetype
					)
				{
					iscontinue = true;
					lua_pop(*m_rppLuaState, 1);
				}
				else
				{
					if (LUA_TSTRING == keytype)
					{
						const char* name = lua_tostring(*m_rppLuaState, -2);
						if (name[0] == ScriptProperty::SCRIPT_INERL_PREFIX)
						{
							iscontinue = true;
							lua_pop(*m_rppLuaState, 1);
						}
					}
				}
				if (!iscontinue)
				{
					if (LUA_TTABLE == valuetype)
					{
						if (1 == lua_getmetatable(*m_rppLuaState, -1))
						{
							lua_getfield(*m_rppLuaState, -1, "__mode");
							if (LUA_TNIL != lua_type(*m_rppLuaState, -1))
							{
								iscontinue = true;
								lua_pop(*m_rppLuaState, 1);
							}
							lua_pop(*m_rppLuaState, 2);
						}						
					}
				}
			}
		} while (iscontinue && isnext);		
		return isnext;
	}
	//---------------------------------------------------------------------------------------------------------
	const char* LuaMemberIterator::GetStringField(uint* len) const
	{
		size_t slen = 0;
		const char* pstr = lua_tolstring(*m_rppLuaState, -2, &slen);
		*len = slen;
		return pstr;
	}
	//---------------------------------------------------------------------------------------------------------
	int LuaMemberIterator::GetIntField() const
	{
		return lua_tointeger(*m_rppLuaState, -2);
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 LuaMemberIterator::GetUInt64Field() const
	{
		return*(uint64*)luaL_tocdata(*m_rppLuaState, -2);
	}
	//---------------------------------------------------------------------------------------------------------
	hc::DataType LuaMemberIterator::GetKeyType() const
	{
		DataType res = DT_NULL;
		switch (lua_type(*m_rppLuaState, -2))
		{
		case LUA_TNUMBER: res = DT_INT; break;
		case LUA_TSTRING: res = DT_STRING; break;
		case LUA_TCDATA:
		{
			switch (luaL_cdatatype(*m_rppLuaState, -2))
			{
			case MY_CTID_INT64: res = DT_INT64; break;
			case MY_CTID_UINT64: res = DT_UINT64; break;
			default:ASSERT(false);
			}
			break;
		}
		default:ASSERT(false);
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::DataType LuaMemberIterator::GetValueType() const
	{
		DataType res = DT_NULL;
		switch (lua_type(*m_rppLuaState, -1))
		{
		case LUA_TNIL: res = DT_NULL; break;
		case LUA_TBOOLEAN: res = DT_BOOL; break;
		case LUA_TNUMBER: res = DT_DOUBLE; break;
		case LUA_TSTRING: res = DT_STRING; break;
		case LUA_TUSERDATA: res = DT_NATIVE; break;
		case LUA_TTABLE: res = DT_CLASS; break;
		case LUA_TCDATA:
		{
						   switch (luaL_cdatatype(*m_rppLuaState, -1))
						   {
						   case MY_CTID_INT64: res = DT_INT64; break;
						   case MY_CTID_UINT64: res = DT_UINT64; break;
						   default:ASSERT(false);
						   }
						   break;
		}
		default:ASSERT(false);
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	bool LuaMemberIterator::AsBool() const
	{
		ASSERT(lua_isboolean(*m_rppLuaState, -1));
		bool res = lua_toboolean(*m_rppLuaState, -1);
		//removes 'value'; keeps 'key' for next iteration
		lua_pop(*m_rppLuaState, 1);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	short LuaMemberIterator::AsShort() const
	{
		ASSERT(lua_isnumber(*m_rppLuaState, -1));
		short res = lua_tointeger(*m_rppLuaState, -1);
		lua_pop(*m_rppLuaState, 1);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	ushort LuaMemberIterator::AsUShort() const
	{
		ASSERT(lua_isnumber(*m_rppLuaState, -1));
		ushort res = lua_tointeger(*m_rppLuaState, -1);
		lua_pop(*m_rppLuaState, 1);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	int LuaMemberIterator::AsInt() const
	{
		ASSERT(lua_isnumber(*m_rppLuaState, -1));
		int res = lua_tointeger(*m_rppLuaState, -1);
		lua_pop(*m_rppLuaState, 1);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	uint LuaMemberIterator::AsUInt() const
	{
		ASSERT(lua_isnumber(*m_rppLuaState, -1));
		uint res = lua_tointeger(*m_rppLuaState, -1);
		lua_pop(*m_rppLuaState, 1);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	double LuaMemberIterator::AsDouble() const
	{
		ASSERT(lua_isnumber(*m_rppLuaState, -1));
		double res = lua_tonumber(*m_rppLuaState, -1);
		lua_pop(*m_rppLuaState, 1);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	float LuaMemberIterator::AsFloat() const
	{
		ASSERT(lua_isnumber(*m_rppLuaState, -1));
		float res = lua_tonumber(*m_rppLuaState, -1);
		//removes 'value'; keeps 'key' for next iteration
		lua_pop(*m_rppLuaState, 1);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	int64 LuaMemberIterator::AsInt64() const
	{
		ASSERT(LUA_TCDATA == lua_type(*m_rppLuaState, -1));
		ASSERT(MY_CTID_INT64 == luaL_cdatatype(*m_rppLuaState, -1));
		int64 res = *(int64*)luaL_tocdata(*m_rppLuaState, -1);
		lua_pop(*m_rppLuaState, 1);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 LuaMemberIterator::AsUInt64() const
	{
		ASSERT(LUA_TCDATA == lua_type(*m_rppLuaState, -1));
		ASSERT(MY_CTID_UINT64 == luaL_cdatatype(*m_rppLuaState, -1));
		uint64 res = *(uint64*)luaL_tocdata(*m_rppLuaState, -1);
		lua_pop(*m_rppLuaState, 1);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	const char* LuaMemberIterator::AsString(uint* len) const
	{
		size_t slen = 0;
		ASSERT(lua_isstring(*m_rppLuaState, -1));
		const char* res = lua_tolstring(*m_rppLuaState, -1, &slen);
		*len = slen;
		//removes 'value'; keeps 'key' for next iteration
		lua_pop(*m_rppLuaState, 1);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	const Object* LuaMemberIterator::AsNative() const
	{
		ASSERT(lua_isuserdata(*m_rppLuaState, -1));
		const Object* res = ((hc::ScriptValue*)lua_touserdata(*m_rppLuaState, -1))->GetObject();
		//removes 'value'; keeps 'key' for next iteration
		lua_pop(*m_rppLuaState, 1);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	const hc::IMemberIterator& LuaMemberIterator::AsClass() const
	{
		ASSERT(lua_istable(*m_rppLuaState, -1));
		int index = m_ChiledIndex++;
		if (m_ChiledArray.size()<m_ChiledIndex)
		{
			LuaChiledIterator* cit = NEW LuaChiledIterator(m_InstanceHandle, m_rppLuaState);
			m_ChiledArray.push_back(cit);
		}
		return *m_ChiledArray[index];
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaMemberIterator::End() const
	{
		m_ChiledIndex = 0;
		lua_pop(*m_rppLuaState, 1);
	}
}
