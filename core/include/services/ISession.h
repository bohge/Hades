#pragma once
#include "Protocol.h"


namespace hc
{
	class IArguments;
	class IScriptInstance;
	class ICoroutineInvoker;
	class HADES_CORE_API ISession : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(ISession);
	public:
		enum SessionState
		{
			SS_ENTRY = 0,
			SS_PASUE,
			SS_DONE,
			SS_ERROR,
		};
	private:
		SessionState			m_State;
		uint64					m_SessionID;
		Protocol*				m_rpArgument;
		Protocol*				m_rpResult;
		ICoroutineInvoker*		m_rpInvoker;
	public:
		ISession(void);
		virtual ~ISession(void);
	private:
		virtual void _OnSessionDone(Protocol* input, IArguments* res) = 0;
	private:
		void _OnEntry(IScriptInstance* slefinstance, IArguments* result, IArguments* input);
		void _OnResume(IArguments* result, IArguments* input);
	public:
		void Invoke(IScriptInstance* slefinstance, IArguments* result, IArguments* input);
		int PasueCoroutine();
		void GotError(const eastl::string& error);
		void SetResult(Protocol* ptc);
	public:
		void Reset();
		HADES_FORCEINLINE void SetSessionID(uint64 id);
		HADES_FORCEINLINE uint64 GetSessionID();
		HADES_FORCEINLINE void SetArgument(Protocol* ptc);
		HADES_FORCEINLINE uint GetFunction();
		HADES_FORCEINLINE void SetInvoker(ICoroutineInvoker* invoker);
		HADES_FORCEINLINE ICoroutineInvoker* GetInvoker();
		HADES_FORCEINLINE SessionState GetState();
		HADES_FORCEINLINE Protocol* GetCallerProtocl();
	};





	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE Protocol* ISession::GetCallerProtocl()
	{
		return m_rpArgument;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint ISession::GetFunction()
	{
		return m_rpArgument->GetFunction();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE ISession::SessionState ISession::GetState()
	{
		return m_State;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ISession::SetArgument(Protocol* ptc)
	{
		ASSERT(NULL == m_rpArgument);//只能设置一次
		m_rpArgument = ptc;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ISession::SetSessionID(uint64 id)
	{
		m_SessionID = id;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint64 ISession::GetSessionID()
	{
		return m_SessionID;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ISession::SetInvoker(ICoroutineInvoker* invoker)
	{
		m_rpInvoker = invoker;
		m_State = NULL == m_rpInvoker ? SS_ERROR : m_State;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE ICoroutineInvoker* ISession::GetInvoker()
	{
		return m_rpInvoker;
	}
}
