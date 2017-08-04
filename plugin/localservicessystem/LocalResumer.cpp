#include "LocalServicesSystem.h"
#include "Serviceshelper.hpp"
#include "LocalResumer.h"



namespace hlss
{
	RTTI_TYPE_IMPLEMENT(LocalResumer);
	//---------------------------------------------------------------------------------------------------------
	LocalResumer::LocalResumer()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LocalResumer::~LocalResumer()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	const byte* LocalResumer::GetBuffer() const
	{
		return (byte*)&m_Data;
	}
	//---------------------------------------------------------------------------------------------------------
	uint LocalResumer::GetLength() const
	{
		return sizeof(m_Data);
	}
	//---------------------------------------------------------------------------------------------------------
	bool LocalResumer::FromBuffer(const byte* buff, uint len)
	{
		m_Data = *(Data*)buff;
		return true;
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalResumer::_Resume(hc::IArguments* arg)
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
				arg);
		}
	}
}