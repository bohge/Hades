#pragma once
#include "script/ScriptProperty.h"
#include "core/Object.hpp"


namespace hc
{
	class ScriptValue;
	class HADES_CORE_API ICoroutineInvoker : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(ICoroutineInvoker);
	public:
		enum CoroutineStatus
		{
			CS_OK = 0,
			CS_YEILD,
			CS_ERROR,
		};
	private:
		CoroutineStatus		m_Status;
	protected:
		ICoroutineInvoker(void);
		virtual ~ICoroutineInvoker(void);
	private:
		virtual void _SetupResume(const ScriptProperty::Arguments& args) = 0;
	private:
		virtual void _DoResume(void** p1) = 0;
		virtual void _DoResume(void** p1, void** p2) = 0;
		virtual void _DoResume(void** p1, void** p2, void** p3) = 0;
		virtual void _DoResume(void** p1, void** p2, void** p3, void** p4) = 0;
		virtual void _DoInvoke(void** p1) = 0;
		virtual void _DoInvoke(void** p1, void** p2) = 0;
		virtual void _DoInvoke(void** p1, void** p2, void** p3) = 0;
		virtual void _DoInvoke(void** p1, void** p2, void** p3, void** p4) = 0;
		virtual void _DoInvoke(void** p1, void** p2, void** p3, void** p4, void** p5) = 0;
	public:		
		virtual int Pasue() = 0;
		virtual void Resume() = 0;
		virtual void Invoke() = 0;
		virtual void GotError(const eastl::string& error) = 0;
	public:
		void SetupResume();
		void SetupResume(uint arg1);
		void SetupResume(uint arg1, uint arg2);
		void SetupResume(uint arg1, uint arg2, uint arg3);
		void SetupResume(uint arg1, uint arg2, uint arg3, uint arg4);
	protected:
		HADES_FORCEINLINE void _SetStatus(CoroutineStatus s);
	public:
		HADES_FORCEINLINE CoroutineStatus GetStatus();
	public:
		//-------------------------------------------------------------------------------------------------------
		template<typename T1>
		HADES_FORCEINLINE void Resume(T1** p1)
		{
			_DoResume((void**)p1);
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T1, typename T2>
		HADES_FORCEINLINE void Resume(T1** p1, T2** p2)
		{
			_DoResume((void**)p1, (void**)p2);
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T1, typename T2, typename T3>
		HADES_FORCEINLINE void Resume(T1** p1, T2** p2, T3** p3)
		{
			_DoResume((void**)p1, (void**)p2, (void**)p3);
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T1, typename T2, typename T3, typename T4>
		HADES_FORCEINLINE void Resume(T1** p1, T2** p2, T3** p3, T4** p4)
		{
			_DoResume((void**)p1, (void**)p2, (void**)p3, (void**)p4);
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T1>
		HADES_FORCEINLINE void Invoke(T1** p1)
		{
			_DoInvoke((void**)p1);
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T1, typename T2>
		HADES_FORCEINLINE void Invoke(T1** p1, T2** p2)
		{
			_DoInvoke((void**)p1, (void**)p2);
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T1, typename T2, typename T3>
		HADES_FORCEINLINE void Invoke(T1** p1, T2** p2, T3** p3)
		{
			_DoInvoke((void**)p1, (void**)p2, (void**)p3);
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T1, typename T2, typename T3, typename T4>
		HADES_FORCEINLINE void Invoke(T1** p1, T2** p2, T3** p3, T4** p4)
		{
			_DoInvoke((void**)p1, (void**)p2, (void**)p3, (void**)p4);
		}
		//-------------------------------------------------------------------------------------------------------
		template<typename T1, typename T2, typename T3, typename T4, typename T5>
		HADES_FORCEINLINE void Invoke(T1** p1, T2** p2, T3** p3, T4** p4, T5** p5)
		{
			_DoInvoke((void**)p1, (void**)p2, (void**)p3, (void**)p4, (void**)p5);
		}
	};



	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ICoroutineInvoker::_SetStatus(CoroutineStatus s)
	{
		m_Status = s;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE ICoroutineInvoker::CoroutineStatus ICoroutineInvoker::GetStatus()
	{
		return m_Status;
	}
}
