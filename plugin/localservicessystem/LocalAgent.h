#pragma once
#include "services/IAgent.h"


namespace hlss
{
	class LocalAgent : public hc::IAgent
	{
	private:
		uint64				m_ServiceGID;
	public:
		LocalAgent();
		~LocalAgent();
	public:
		virtual bool isEqual(const IAgent& lhs);
		virtual uint64 GetGID() const;
		virtual hc::Object* Duplicate() const;
		virtual const byte* GetBuffer() const;
		virtual uint GetLength() const;
		virtual bool FromBuffer(const byte* buff, uint len);
	public:
		HADES_FORCEINLINE void SetGID(uint64 id);
	};



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LocalAgent::SetGID(uint64 id)
	{
		m_ServiceGID = id;
	}
}