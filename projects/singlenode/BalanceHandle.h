#pragma once
#include "thread/IJob.h"



namespace hc
{
	enum CallbackType;
	class Message;
}
namespace hsna
{
	class BalanceHandle : public hc::IJob
	{
		RTTI_DRIVER_TYPE_DECLEAR(BalanceHandle, hc::IJob);
	private:
		uint64						m_ID;
		hc::Message*				m_rpMessage;
		hc::CallbackType			m_CallbackType;
	public:
		BalanceHandle();
		virtual ~BalanceHandle();
	private:
		virtual void _DoYieldJob();
		virtual void _DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata);
	public:
		HADES_FORCEINLINE void SetMessage(hc::Message* req);
		HADES_FORCEINLINE void SetCallbackType(hc::CallbackType type);
		HADES_FORCEINLINE void SetClient(uint64 id);
	};



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void BalanceHandle::SetMessage(hc::Message* req)
	{
		m_rpMessage = req;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void BalanceHandle::SetCallbackType(hc::CallbackType type)
	{
		m_CallbackType = type;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void BalanceHandle::SetClient(uint64 id)
	{
		m_ID = id;
	}
}