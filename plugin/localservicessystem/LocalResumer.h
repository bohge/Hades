#pragma once
#include "services/IResumer.h"


namespace hlss
{
	class LocalResumer : public hc::IResumer
	{
		RTTI_DRIVER_TYPE_DECLEAR(LocalResumer, hc::IResumer);
	private:
		struct Data
		{
			uint64		Caller;
			uint64		Session;
			bool		isResumed;
		};
	private:
		Data	m_Data;
	public:
		LocalResumer();
		virtual ~LocalResumer();
	public:
		virtual void _Resume(hc::IArguments* arg);
		virtual const byte* GetBuffer() const;
		virtual uint GetLength() const;
		virtual bool FromBuffer(const byte* buff, uint len);
	public:
		HADES_FORCEINLINE void Setup(uint64 c, uint64 s);
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void LocalResumer::Setup(uint64 c, uint64 s)
	{
		m_Data.isResumed = false;
		m_Data.Caller = c;
		m_Data.Session = s;
	}
}