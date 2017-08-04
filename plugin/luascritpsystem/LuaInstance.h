#pragma once
#include "script/IScriptInstance.h"



struct lua_State;
namespace hlss
{
	class LuaVirtualMachine;
	class LuaInstance : public hc::IScriptInstance
	{
	private:
		typedef eastl::map<uint64, hc::IScriptInstance*> ChieldMap;
	protected:
		LuaVirtualMachine*	m_rpVirtualMachine;
	private:
		ChieldMap			m_ChieldMap;
		lua_State**			m_rppLuaState;
		bool				m_isContinuously;
	public:
		LuaInstance(uint host, uint hash, lua_State** state, LuaVirtualMachine* vm);
		virtual ~LuaInstance();
	public:
		virtual void PushInstance() = 0;
		virtual void PopInstance() = 0;
		virtual void Exit() = 0;
	public:
		virtual void ContinuouslySetMember(const char* name);
		virtual void ContinuouslySetMember(const char* name, bool obj);
		virtual void ContinuouslySetMember(const char* name, short obj);
		virtual void ContinuouslySetMember(const char* name, ushort obj);
		virtual void ContinuouslySetMember(const char* name, int obj);
		virtual void ContinuouslySetMember(const char* name, uint obj);
		virtual void ContinuouslySetMember(const char* name, float obj);
		virtual void ContinuouslySetMember(const char* name, int64 obj);
		virtual void ContinuouslySetMember(const char* name, uint64 obj);
		virtual void ContinuouslySetMember(const char* name, double obj);
		virtual void ContinuouslySetMember(const char* name, const char* obj, uint len);
		virtual void ContinuouslySetMember(const char* name, hc::Object* obj, hc::ScriptValue::ClearFunction func);
		virtual void ContinuouslySetMember(int key);
		virtual void ContinuouslySetMember(int key, bool obj);
		virtual void ContinuouslySetMember(int key, short obj);
		virtual void ContinuouslySetMember(int key, ushort obj);
		virtual void ContinuouslySetMember(int key, int obj);
		virtual void ContinuouslySetMember(int key, uint obj);
		virtual void ContinuouslySetMember(int key, float obj);
		virtual void ContinuouslySetMember(int key, int64 obj);
		virtual void ContinuouslySetMember(int key, uint64 obj);
		virtual void ContinuouslySetMember(int key, double obj);
		virtual void ContinuouslySetMember(int key, const char* obj, uint len);
		virtual void ContinuouslySetMember(int key, hc::Object* obj, hc::ScriptValue::ClearFunction func);
		virtual void ContinuouslySetMember(uint64 key);
		virtual void ContinuouslySetMember(uint64 key, bool obj);
		virtual void ContinuouslySetMember(uint64 key, short obj);
		virtual void ContinuouslySetMember(uint64 key, ushort obj);
		virtual void ContinuouslySetMember(uint64 key, int obj);
		virtual void ContinuouslySetMember(uint64 key, uint obj);
		virtual void ContinuouslySetMember(uint64 key, float obj);
		virtual void ContinuouslySetMember(uint64 key, int64 obj);
		virtual void ContinuouslySetMember(uint64 key, uint64 obj);
		virtual void ContinuouslySetMember(uint64 key, double obj);
		virtual void ContinuouslySetMember(uint64 key, const char* obj, uint len);
		virtual void ContinuouslySetMember(uint64 key, hc::Object* obj, hc::ScriptValue::ClearFunction func);
		virtual hc::IScriptInstance* CreateChieldInstance(const char* name);
		virtual hc::IScriptInstance* CreateChieldInstance(int key);
		virtual hc::IScriptInstance* CreateChieldInstance(uint64 key);
	protected:
		HADES_FORCEINLINE void _SetContinuously(bool c);
		HADES_FORCEINLINE bool _isContinuously() const;
		HADES_FORCEINLINE lua_State** _GetState() const;
	public:
		HADES_FORCEINLINE void SetState(lua_State** st);
		HADES_FORCEINLINE void SetVirtualMachine(LuaVirtualMachine* vm);
	};




	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LuaInstance::SetVirtualMachine(LuaVirtualMachine* vm)
	{
		m_rpVirtualMachine = vm;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LuaInstance::_SetContinuously(bool c)
	{
		m_isContinuously = c;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool LuaInstance::_isContinuously() const
	{
		return m_isContinuously;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE lua_State** LuaInstance::_GetState() const
	{
		return m_rppLuaState;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LuaInstance::SetState(lua_State** st)
	{
		m_rppLuaState = st;
	}
}
