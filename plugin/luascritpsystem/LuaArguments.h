#pragma once
#include "script/ScriptProperty.h"
#include "script/ScriptValue.h"
#include "script/IArguments.h"


#include <EASTL/vector.h>

struct lua_State;
namespace hlss
{
	class LuaVirtualMachine;
	class LuaArgumentInstance;
	class LuaArgumentTableIterator;
	class LuaArguments : public hc::IArguments
	{
	private:
		int							m_ArgumentsCount;
		int							m_ArgumentOffset;
		lua_State*					m_rpLuaState;
		LuaArgumentInstance*		m_pTable;
		LuaArgumentTableIterator*	m_pIterate;
		hc::IDecoder*				m_pDecoder;
		hc::IEncoder*				m_pRootIEncoder;
	public:
		LuaArguments(void);
		virtual ~LuaArguments(void);
	private:
		virtual void _Reset();
		virtual void _Deserializer(hc::IDecoder* de);
	public:
		virtual bool Empty();
		virtual void PushUInt(uint data);
		virtual void PushUInt64(uint64 data);
		virtual void PushString(const char* v, uint len);
		virtual void PushSerializable(hc::ISerializable* obj);
	private:
		hc::IEncoder* _GetRootEncoder();
	public:
		int RealDeserialize(LuaVirtualMachine* vm);
		void SetLuaState(lua_State* state);
		void Serialize();
	public:
		HADES_FORCEINLINE void SetArgumentCount(int count);
		HADES_FORCEINLINE void SetArgumentOffset(int offset);
	};




	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LuaArguments::SetArgumentCount(int count)
	{
		m_ArgumentsCount = count > 0 ? count : 0;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LuaArguments::SetArgumentOffset(int offset)
	{
		m_ArgumentOffset = offset;
	}
}
