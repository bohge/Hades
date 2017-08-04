#pragma once
#include "core/Recycleablevector.hpp"
#include "services/ScriptServices.h"
#include "script/IScriptInstance.h"
#include "core/SmartPtr.hpp"


namespace hc
{
	class IAgent;
	class ClassRegister;
	class HADES_CORE_API IServices : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IServices);
	private:
		eastl::string		m_ServiceFile;
		uint64				m_ServiceGID;
		Rtti*				m_rpAgentRtti;
		ClassRegister*		m_rpClassRegister;
		volatile int		m_AgentCount;
		bool				m_isPasueAutoFree;
	public:
		IServices(void);
		virtual ~IServices(void);
	private:
		virtual void _OnAllocAgent(IAgent* agent) = 0;
		virtual void _OnFreeAgent(IAgent* agent) = 0;
		virtual void _OnServicesFree() = 0;
	public:
		virtual void SetGID(uint64 id) = 0;
		virtual	bool RemoteInvoker(Protocol* ptc) = 0;
		virtual	bool RemoteCallBack(Protocol* ptc) = 0;
	public:
		IAgent* AllocAgent();
		void FreeAgent(IAgent* agent);
		void IncrementAgent();
		void DecrementAgent();
		uint GetFunctionIndex(const eastl::string& funcname);
	protected:
		HADES_FORCEINLINE void _SetGID(uint64 id);
	public:
		HADES_FORCEINLINE void SetClassInfomation(ClassRegister* cr);
		HADES_FORCEINLINE ClassRegister* GetClassInfomation();
		HADES_FORCEINLINE void SetAgentRtti(Rtti* rtti);
		HADES_FORCEINLINE Rtti* GetAgentRtti();
		HADES_FORCEINLINE uint64 GetGID();
		HADES_FORCEINLINE const eastl::string& GetServiceFile();
		HADES_FORCEINLINE void SetServiceFile(const eastl::string& f);
		HADES_FORCEINLINE void PasueAutoFree();//这个有点特殊了，rpc调用需要收到对方send来的agent之后转化为流，所以需要暂时释放handle，这个需要修改结构来修正这个问题
	};



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IServices::_SetGID(uint64 id)
	{
		m_ServiceGID = id;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint64 IServices::GetGID()
	{
		return m_ServiceGID;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IServices::SetClassInfomation(ClassRegister* cr)
	{
		ASSERT(NULL == m_rpClassRegister);
		m_rpClassRegister = cr;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IServices::SetAgentRtti(Rtti* rtti)
	{
		ASSERT(NULL == m_rpAgentRtti);
		m_rpAgentRtti = rtti;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE Rtti* IServices::GetAgentRtti()
	{
		return m_rpAgentRtti;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE ClassRegister* IServices::GetClassInfomation()
	{
		return m_rpClassRegister;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const eastl::string& IServices::GetServiceFile()
	{
		return m_ServiceFile;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IServices::SetServiceFile(const eastl::string& f)
	{
		m_ServiceFile = f;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IServices::PasueAutoFree()
	{
		m_isPasueAutoFree = true;
	}
}
