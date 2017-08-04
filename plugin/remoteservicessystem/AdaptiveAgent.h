#pragma once
#include "services/IAgent.h"
#include "AgentData.h"



namespace hrss
{
	class AdaptiveAgent : public hc::IAgent
	{
	private:
		AgentData	m_AgentData;
	public:
		AdaptiveAgent();
		~AdaptiveAgent();
	public:
		virtual uint64 GetGID() const;
		virtual bool isEqual(const IAgent& lhs);
		virtual hc::Object* Duplicate() const;
		virtual const byte* GetBuffer() const;
		virtual uint GetLength() const;
		virtual bool FromBuffer(const byte* buff, uint len);
	public:
		HADES_FORCEINLINE void SetCurrentGID(uint64 id);
		HADES_FORCEINLINE void SetOriginalGID(uint64 id);
		HADES_FORCEINLINE uint GetCurrentHash();
		HADES_FORCEINLINE void SetCurrentHash(uint hash);
		HADES_FORCEINLINE void SetOriginalHash(uint hash);
		HADES_FORCEINLINE void SetServiceFile(const eastl::string& file);
	};




	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void AdaptiveAgent::SetOriginalGID(uint64 id)
	{
		m_AgentData.OriginalGID = id;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void AdaptiveAgent::SetOriginalHash(uint hash)
	{
		m_AgentData.OriginalHash = hash;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void AdaptiveAgent::SetCurrentGID(uint64 id)
	{
		m_AgentData.CurrentGID = id;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint AdaptiveAgent::GetCurrentHash()
	{
		return m_AgentData.CurrentHash;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void AdaptiveAgent::SetCurrentHash(uint hash)
	{
		m_AgentData.CurrentHash = hash;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void AdaptiveAgent::SetServiceFile(const eastl::string& file)
	{
		memcpy(m_AgentData.ServiceFile, file.c_str(), file.size() + 1);
	}
}