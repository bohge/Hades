#include "RemoteServicesSystem.h"
#include "Serviceshelper.hpp"
#include "RemoteResumer.h"



namespace hrss
{
	RTTI_TYPE_IMPLEMENT(RemoteResumer);
	//---------------------------------------------------------------------------------------------------------
	RemoteResumer::RemoteResumer()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	RemoteResumer::~RemoteResumer()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	const byte* RemoteResumer::GetBuffer() const
	{
		return (byte*)&m_Data;
	}
	//---------------------------------------------------------------------------------------------------------
	uint RemoteResumer::GetLength() const
	{
		return sizeof(m_Data);
	}
	//---------------------------------------------------------------------------------------------------------
	bool RemoteResumer::FromBuffer(const byte* buff, uint len)
	{
		m_Data = *(Data*)buff;
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteResumer::_Resume(hc::IArguments* arg)
	{
		if (m_Data.isResumed)
		{
			HADESERROR("Service context is one shot deal! Scrapped after first invoke! plz set null after invoke");
		}
		else
		{
			m_Data.isResumed = true;
			HandleCallback(m_Data.Caller,
				m_Data.Session,
				m_Data.Hash,
				arg);
		}
	}
}