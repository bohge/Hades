#pragma once
#include "serialize/ISerializable.h"
#include "log/ILogSystem.h"

namespace hc
{
	class IArguments;
	class HADES_CORE_API IResumer : public ISerializable
	{
		SERIALIZABL_TYPE_DECLEAR(IResumer);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IResumer, ISerializable);
	private:
		bool		m_isPasue;
	public:
		IResumer(void);
		virtual ~IResumer(void);
	private:
		virtual void _Resume(IArguments* arg) = 0;
	public:
		HADES_FORCEINLINE void Pasue();
		HADES_FORCEINLINE void Resume(IArguments* arg);
	};

	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IResumer::Pasue()
	{
		m_isPasue = true;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IResumer::Resume(IArguments* arg)
	{
		if (m_isPasue)
		{
			m_isPasue = false;
			_Resume(arg);
		}
		else
		{
			HADESERROR("Try to resume a unpasued context!");
		}
	}
}
