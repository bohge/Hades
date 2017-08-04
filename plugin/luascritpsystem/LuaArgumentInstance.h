#pragma once
#include "LuaChieldInstance.h"



struct lua_State;
namespace hlss
{
	class LuaMemberIterator;
	class LuaVirtualMachine;
	class LuaArgumentInstance : public LuaChieldInstance
	{
	private:
		LuaArgumentInstance*	m_ChileAgent;
		bool					m_isChield;
	public:
		LuaArgumentInstance(void);
		LuaArgumentInstance(bool ischield);
		virtual ~LuaArgumentInstance(void);
	public:
		virtual void BeginSetMemberBulk();
		virtual void EndSetMemberBulk();
		virtual hc::IScriptInstance* CreateChieldInstance(const char* name);
		virtual hc::IScriptInstance* CreateChieldInstance(int key);
		virtual hc::IScriptInstance* CreateChieldInstance(uint64 key);
	};
}
