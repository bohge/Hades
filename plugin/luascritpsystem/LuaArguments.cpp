#include "serialize/ISerializeSystem.h"
#include "LuaArgumentTableIterator.h"
#include "serialize/ISerializable.h"
#include "serialize/ISerializer.h"
#include "LuaArgumentInstance.h"
#include "serialize/IEncoder.h"
#include "NativePushHelper.hpp"
#include "LuaVirtualMachine.h"
#include "services/IAgent.h"
#include "log/ILogSystem.h"
#include "LuaArguments.h"
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
	//-------------------------------------------------------------------------------------------------------
	LuaArguments::LuaArguments(void)
		: m_rpLuaState(NULL)
		, m_ArgumentsCount(0)
		, m_ArgumentOffset(0)
		, m_pIterate(NEW LuaArgumentTableIterator)
		, m_pTable(NEW LuaArgumentInstance)
		, m_pDecoder(NULL)
		, m_pRootIEncoder(NULL)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	LuaArguments::~LuaArguments(void)
	{
		SAFE_DELETE(m_pTable);
		SAFE_DELETE(m_pIterate);
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaArguments::SetLuaState(lua_State* state)
	{
		m_rpLuaState = state;
		m_pTable->SetState(&m_rpLuaState);
		m_pIterate->SetLuaState(&m_rpLuaState);
	}
	//-------------------------------------------------------------------------------------------------------
	bool LuaArguments::Empty()
	{
		return 0 == m_ArgumentsCount;
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaArguments::PushUInt(uint data)
	{
		IEncoder* en = _GetRootEncoder();
		en->SetField(m_ArgumentsCount, data);
		++m_ArgumentsCount;
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaArguments::PushUInt64(uint64 data)
	{
		IEncoder* en = _GetRootEncoder();
		en->SetField(m_ArgumentsCount, data);
		++m_ArgumentsCount;
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaArguments::PushString(const char* v, uint len)
	{
		IEncoder* en = _GetRootEncoder();
		en->SetField(m_ArgumentsCount, v, len);
		++m_ArgumentsCount;
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaArguments::PushSerializable(hc::ISerializable* obj)
	{
		IEncoder* en = _GetRootEncoder();
		en->SetField(m_ArgumentsCount, obj);
		++m_ArgumentsCount;
	}
	//-------------------------------------------------------------------------------------------------------
	hc::IEncoder* LuaArguments::_GetRootEncoder()
	{
		if (NULL == m_pRootIEncoder)
		{
			ISerializer* ser = _GetSerializer();
			m_pRootIEncoder = ser->GetEncoder();
		}
		return m_pRootIEncoder;
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaArguments::Serialize()
	{
		if (!Empty())
		{
			ISerializer* ser = _GetSerializer();
			IEncoder* en = ser->GetEncoder();
			for (int i = 0; i < m_ArgumentsCount; ++i)
			{
				int argindex = i + m_ArgumentOffset;
				switch (lua_type(m_rpLuaState, argindex))
				{
				case LUA_TNIL:
				{
								 en->SetField(i);
								 break;
				}
				case LUA_TBOOLEAN:
				{
									 ASSERT(lua_isboolean(m_rpLuaState, argindex));
									 bool res = lua_toboolean(m_rpLuaState, argindex);
									 en->SetField(i, res);
									 break;
				}
				case LUA_TNUMBER:
				{
									ASSERT(lua_isnumber(m_rpLuaState, argindex));
									double res = lua_tonumber(m_rpLuaState, argindex);
									en->SetField(i, res);
									break;
				}
				case LUA_TCDATA:
				{
								   switch (luaL_cdatatype(m_rpLuaState, argindex))
								   {
								   case MY_CTID_INT64:
								   {
														 ASSERT(LUA_TCDATA == lua_type(m_rpLuaState, argindex));
														 ASSERT(MY_CTID_INT64 == luaL_cdatatype(m_rpLuaState, argindex));
														 int64 res = *(int64*)luaL_tocdata(m_rpLuaState, argindex);
														 en->SetField(i, res);
														 break;
								   }
								   case MY_CTID_UINT64:
								   {
														  ASSERT(LUA_TCDATA == lua_type(m_rpLuaState, argindex));
														  ASSERT(MY_CTID_UINT64 == luaL_cdatatype(m_rpLuaState, argindex));
														  uint64 res = *(uint64*)luaL_tocdata(m_rpLuaState, argindex);
														  en->SetField(i, res);
														  break;
								   }
								   default:ASSERT(false);
								   }
								   break;
				}
				case LUA_TSTRING:
				{
									ASSERT(lua_isstring(m_rpLuaState, argindex));
									size_t len = 0;
									const char* res = lua_tolstring(m_rpLuaState, argindex, &len);
									en->SetField(i, res, len);
									break;
				}
				case LUA_TUSERDATA:
				{
									  ASSERT(lua_isuserdata(m_rpLuaState, argindex));
									  const Object* value = ((hc::ScriptValue*)lua_touserdata(m_rpLuaState, argindex))->GetObject();
									  ASSERT(value->isTypeOrDriverType(ISerializable::RTTI()));
									  en->SetField(i, static_cast<const ISerializable*>(value));
									  break;
				}
				case LUA_TTABLE:
				{
								   DEBUGCODE(int t = lua_gettop(m_rpLuaState));
								   lua_pushvalue(m_rpLuaState, argindex);
								   IEncoder* sonen = en->GetChild();
								   hc::RecursiveSaveInstance(*m_pIterate, sonen);
								   en->SetField(i, sonen);
								   ASSERT(lua_gettop(m_rpLuaState) == t);
								   break;
				}
				default:ASSERT(false); break;
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaArguments::_Deserializer(hc::IDecoder* de)
	{
		m_pDecoder = de;
	}
	//-------------------------------------------------------------------------------------------------------
	void LuaArguments::_Reset()
	{
		m_ArgumentsCount = 0;
		m_ArgumentOffset = 0;
		m_pDecoder = NULL;
		m_pRootIEncoder = NULL;
	}
	//-------------------------------------------------------------------------------------------------------
	int LuaArguments::RealDeserialize(LuaVirtualMachine* vm)
	{
		int size = 0;
		if (m_pDecoder)
		{
			m_pTable->SetVirtualMachine(vm);
			size = m_pDecoder->GetLength();
			for (int i = 0; i < size; ++i)
			{
				switch (m_pDecoder->GetValueType(i))
				{
				case DT_NULL:
				{
								lua_pushnil(m_rpLuaState);
								break;
				}
				case DT_BOOL:
				{
								bool value = m_pDecoder->AsBool(i);
								lua_pushboolean(m_rpLuaState, value);
								break;
				}
				case DT_INT:
				{
							   int value = m_pDecoder->AsInt(i);
							   lua_pushinteger(m_rpLuaState, value);
							   break;
				}
				case DT_UINT:
				{
								uint value = m_pDecoder->AsUInt(i);
								lua_pushinteger(m_rpLuaState, value);
								break;
				}
				case DT_DOUBLE:
				{
								  double value = m_pDecoder->AsDouble(i);
								  lua_pushnumber(m_rpLuaState, value);
								  break;
				}
				case DT_INT64:
				{
								 int64* ud = (int64*)luaL_pushcdata(m_rpLuaState, MY_CTID_INT64, sizeof(int64));
								 *ud = m_pDecoder->AsInt64(i);
								 break;
				}
				case DT_UINT64:
				{
								  uint64 value = m_pDecoder->AsUInt64(i);
								  if (hc::nullid == value)
								  {
									  lua_pushnil(m_rpLuaState);
								  }
								  else
								  {
									  uint64* ud = (uint64*)luaL_pushcdata(m_rpLuaState, MY_CTID_UINT64, sizeof(uint64));
									  *ud = value;
								  }
								  break;
				}
				case DT_STRING:
				{
								  uint len = 0;
								  const char* value = m_pDecoder->AsString(i, &len);
								  lua_pushlstring(m_rpLuaState, value, len);
								  break;
				}
				case DT_NATIVE:
				{
								  ISerializable* value = m_pDecoder->AsNative(i);
								  if (NULL == value)
								  {
									  lua_pushnil(m_rpLuaState);
								  }
								  else
								  {
									  //uint size = value->GetLength();
									  //size = size > sizeof(ScriptValue) ? size : sizeof(ScriptValue);
									  //ScriptValue* userdata = (ScriptValue*)lua_newuserdata(m_rpLuaState, size);
									  ScriptValue* userdata = (ScriptValue*)lua_newuserdata(m_rpLuaState, sizeof(ScriptValue));
									  userdata->SetValue(value, value->GetClearFunction());
									  PushNative(m_rpLuaState, vm, value);
								  }
								  break;
				}
				case DT_CLASS:
				{
								 DEBUGCODE(int t = lua_gettop(m_rpLuaState));
								 const IDecoder* tde = m_pDecoder->AsClass(i);
								 lua_newtable(m_rpLuaState);
								 RecursiveSetInstance(m_pTable, tde);
								 ASSERT(lua_gettop(m_rpLuaState) == t + 1);
								 break;
				}
				default:ASSERT(false && "unkown data type");
				}
			}
		}
		return size;
	}
}