#pragma once
#include "serialize/ISerializable.h"
#include "services/IServices.h"
#include "core/SmartPtr.hpp"


namespace hc
{
	class HADES_CORE_API IAgent : public ISerializable
	{
		SERIALIZABL_TYPE_DECLEAR(IAgent);
	private:
		static hc::Rtti			s_Rtti;
	public:
		static hc::Rtti _MakeRtti();
		static hc::Rtti _MakeRttiWithFacotr(uint f);
		static const hc::Rtti& RTTI();
		static uint SID();
	private:
		const Rtti*			m_pAgentRtti;		
	public:
		IAgent(void);
		virtual ~IAgent(void);
	public:
		virtual bool isEqual(const IAgent& lhs) = 0;
		virtual uint64 GetGID() const = 0;
	public:
		virtual uint GetTypeID() const;
		virtual bool isType(const hc::Rtti& rtti) const;
		virtual const char* GetTypeName() const;
		virtual uint GetTypeSize() const;
		virtual bool isTypeOrDriverType(const hc::Rtti& rtti) const;
		virtual const hc::Rtti& GetRtti() const;
		virtual const hc::Rtti* GetBase() const;
		virtual ScriptValue::ClearFunction GetClearFunction();
	public:
		ClassRegister* GetClassInfomation();
		bool RemoteInvoker(Protocol* ptc);
		uint GetFunctionIndex(const eastl::string& funcname);
	public:
		HADES_FORCEINLINE void SetAgentRtti(const Rtti* rtti);
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IAgent::SetAgentRtti(const Rtti* rtti)
	{
		m_pAgentRtti = rtti;
	}
}
