#include "serialize/ISerializeSystem.h"
#include "network/message/IMessage.h"
#include "services/IServicesSystem.h"
#include "script/ICoroutineInvoker.h"
#include "serialize/ISerializer.h"
#include "services/IServices.h"
#include "services/ExecuteJob.h"
#include "serialize/IDecoder.h"
#include "script/IArguments.h"
#include "services/ISession.h"

namespace hc
{
	RTTI_TYPE_IMPLEMENT(ISession);
	//---------------------------------------------------------------------------------------------------------
	ISession::ISession(void)
		:m_rpResult(NULL)
		, m_rpArgument(NULL)
		, m_SessionID(nullid)
		, m_rpInvoker(NULL)
		, m_State(SS_ENTRY)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	ISession::~ISession(void)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void ISession::_OnEntry(IScriptInstance* slefinstance, IArguments* result, IArguments* input)
	{
		if (!m_rpArgument->EmptyBuffer())
		{
			input->FromBuffer(m_rpArgument->GetBuffer());
		}
		IMessage* msg = m_rpArgument->GetMessage();
		m_rpInvoker->Invoke(&result, &slefinstance, &input, &msg);
	}
	//---------------------------------------------------------------------------------------------------------
	void ISession::_OnResume(IArguments* result, IArguments* input)
	{
		if (!m_rpResult->EmptyBuffer())
		{
			input->FromBuffer(m_rpResult->GetBuffer());
		}
		m_rpInvoker->Resume(&result, &input);
	}
	//---------------------------------------------------------------------------------------------------------
	void ISession::Invoke(IScriptInstance* slefinstance, IArguments* result, IArguments* input)
	{
		ICoroutineInvoker::CoroutineStatus cs = ICoroutineInvoker::CS_ERROR;
		switch (m_State)
		{
		case hc::ISession::SS_ENTRY:
		{
									   _OnEntry(slefinstance, result, input);
									   cs = m_rpInvoker->GetStatus();
									   break;
		}
		case hc::ISession::SS_PASUE:
		{
									   _OnResume(result, input);
									   cs = m_rpInvoker->GetStatus();
									   break;
		}
		case hc::ISession::SS_ERROR: break;//Null invoker will set state is error
		case hc::ISession::SS_DONE:
		default:ASSERT(false); break;
		}
		switch (cs)
		{
		case ICoroutineInvoker::CS_OK:
		case ICoroutineInvoker::CS_ERROR:
		{
											m_State = ICoroutineInvoker::CS_OK == cs
												? SS_DONE
												: SS_ERROR;
											_OnSessionDone(m_rpArgument, result);
											break;
		}
		case ICoroutineInvoker::CS_YEILD:
		{
											m_State = SS_PASUE;
											break;
		}
		default:ASSERT(false); break;
		}
		result->Reset();
		input->Reset();
	}
	//---------------------------------------------------------------------------------------------------------
	int ISession::PasueCoroutine()
	{
		return m_rpInvoker->Pasue();
	}
	//---------------------------------------------------------------------------------------------------------
	void ISession::Reset()
	{
		IServicesSystem::Instance()->FreeProtocol(m_rpArgument);
		IServicesSystem::Instance()->FreeProtocol(m_rpResult);
		m_State = SS_ENTRY;
		m_SessionID = nullid;
		m_rpArgument = NULL;
		m_rpInvoker = NULL;
		m_rpResult = NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	void ISession::SetResult(Protocol* ptc)
	{
		IServicesSystem::Instance()->FreeProtocol(m_rpResult);
		m_rpResult = ptc;
	}
	//---------------------------------------------------------------------------------------------------------
	void ISession::GotError(const eastl::string& error)
	{
		m_rpInvoker->GotError(error);
	}
}