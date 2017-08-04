#pragma once
#include "core/Object.hpp"


namespace hc
{
	class ScriptValue;
	class HADES_CORE_API IFunctionInvoker : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IFunctionInvoker);
	private:
		uint		m_ID;
	protected:
		IFunctionInvoker(void);
		virtual ~IFunctionInvoker(void);
	private:
		virtual void _DoInvoke(void** p1) = 0;
		virtual void _DoInvoke(void** p1, void** p2) = 0;
		virtual void _DoInvoke(void** p1, void** p2, void** p3) = 0;
		virtual void _DoInvoke(void** p1, void** p2, void** p3, void** p4) = 0;
	public:
		virtual void Invoke() = 0;
	public:
		HADES_FORCEINLINE void SetFunctionID(uint id);
		HADES_FORCEINLINE uint GetFunctionID();
	public:
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
	};


	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IFunctionInvoker::SetFunctionID(uint id)
	{
		m_ID = id;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint IFunctionInvoker::GetFunctionID()
	{
		return m_ID;
	}
}
