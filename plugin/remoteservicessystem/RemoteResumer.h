#pragma once
#include "services/IResumer.h"


namespace hrss
{
	class RemoteResumer : public hc::IResumer
	{
		RTTI_DRIVER_TYPE_DECLEAR(RemoteResumer, hc::IResumer);
	private:
		struct Data
		{
			uint64		Caller;
			uint64		Session;
			uint		Hash;
			bool		isResumed;
		};
	private:
		Data	m_Data;
	public:
		RemoteResumer();
		virtual ~RemoteResumer();
	public:
		virtual void _Resume(hc::IArguments* arg);
		virtual const byte* GetBuffer() const;
		virtual uint GetLength() const;
		virtual bool FromBuffer(const byte* buff, uint len);
	public:
		HADES_FORCEINLINE void Setup(uint64 c, uint64 s, uint h);
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RemoteResumer::Setup(uint64 c, uint64 s, uint h)
	{
		m_Data.isResumed = false;
		m_Data.Caller = c;
		m_Data.Session = s;
		m_Data.Hash = h;
	}
}