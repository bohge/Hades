#pragma once
#include "script/IVirtualMachine.h"
#include "services/IAgent.h"

extern "C"
{
#include "lua.h"
#include "lualib.h"
#include "lauxlib.h"
}

namespace hlss
{
	static bool _RecursivelySetClass(lua_State* pls, hc::IVirtualMachine* vm, hc::Object* obj, const hc::Rtti* rtti)
	{
		if (NULL != rtti)
		{
			const char* name = rtti->GetTypeName();
			if (name)
			{
				lua_getglobal(pls, name);
				if (!lua_istable(pls, -1))
				{
					lua_pop(pls, 1);
					return _RecursivelySetClass(pls, vm, obj, rtti->GetBase());
				}
				else
				{
					lua_setmetatable(pls, -2);
					return true;
				}
			}
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	static bool PushNative(lua_State* pls, hc::IVirtualMachine* vm, hc::Object* obj)
	{
		if (obj->isTypeOrDriverType(hc::IAgent::RTTI()))
		{
			hc::IAgent* agent = static_cast<hc::IAgent*>(obj);
			hc::ClassRegister* clr = agent->GetClassInfomation();
			if (NULL != clr)
			{
				const char* name = obj->GetTypeName();
				vm->ReferenceClass(clr);
				lua_getglobal(pls, name);
				ASSERT(lua_istable(pls, -1));
				lua_setmetatable(pls, -2);
				return true;
			}
		}
		else
		{
			const hc::Rtti* rtti = &obj->GetRtti();
			return _RecursivelySetClass(pls, vm, obj, rtti);
		}
		return false;
	}
}