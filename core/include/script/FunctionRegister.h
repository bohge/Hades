#pragma once
#include "ScriptProperty.h"
#include "IScriptSystem.h"
#include "ScriptUtility.h"


#include <EASTL/string.h>
#include <EASTL/vector.h>

namespace hc
{
	class Rtti;
	class HADES_CORE_API FunctionRegister// : public Object
	{
		//RTTI_TYPE_DECLEAR( ScriptFunction )
	public:
		typedef bool(*TypeCompare)(const Rtti*, const Rtti*);
	private:
		typedef eastl::pair<int, IScriptSystem::ArgumentFunction> ArgIndexFuncPair;
		typedef eastl::vector<const Rtti*> ArgumentArray;
		typedef eastl::vector< ArgIndexFuncPair > GetArgumentFunctionArray;
	private:
		eastl::string					m_FuncName;
		ArgumentArray					m_ArgumentRttis;
		GetArgumentFunctionArray		m_ArgumentGetter;
		IScriptSystem::ResultFunction	m_pResultGetter;
		IScriptSystem::ResultFunction	m_pResultPusher;
		FunctionRegister::TypeCompare	m_pCompareFunction;
		ScriptProperty::FunctionType	m_eFunctionType;
		const Rtti*						m_pResult;
		void*							m_pFunction;
		uint							m_ID;
		uint							m_Index;
	public:
		FunctionRegister(void);
		~FunctionRegister(void);
	private:
		void _CaculateID();//计算函数标识符
		void _SetupArgument();
		ArgIndexFuncPair _GetArgumentPair(const Rtti* rtti);
	public:
		void Clearup(int count, void* args[]);
		void Result(const Rtti* rtti);
		void Arguments(const Rtti* rtti1);
		void Arguments(const Rtti* rtti1, const Rtti* rtti2);
		void Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3);
		void Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4);
		void Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5);
		void Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6);
		void Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7);
		void Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7, const Rtti* rtti8);
		void Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7, const Rtti* rtti8, const Rtti* rtti9);
		void Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7, const Rtti* rtti8, const Rtti* rtti9, const Rtti* rtti10);
		void Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7, const Rtti* rtti8, const Rtti* rtti9, const Rtti* rtti10, const Rtti* rtti11);
		void* GetScriptParameter(int index, void* vm, void* l, void* stack);
	public:
		template<typename FUNC>
		void Function(FUNC f, const eastl::string& name)
		{
			m_pFunction = (void*)f;
			m_FuncName = name;
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE void SetIndex(uint index)
		{
			m_Index = index;
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE uint GetIndex()
		{
			return m_Index;
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE bool isSameFunction(const FunctionRegister* f)
		{
			bool res = true;
			for (int i = 0; i < m_ArgumentRttis.size(); ++i)
			{
				const Rtti* lhs = m_ArgumentRttis[i];
				const Rtti* rhs = f->m_ArgumentRttis[i];
				if (!lhs->isTypeOrDriverType(*rhs)
					&& !rhs->isTypeOrDriverType(*lhs)
					&& !m_pCompareFunction(lhs, rhs)
					)
				{
					res = false;
					break;
				}
			}
			return res;
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE bool isMatchArguments(const Rtti* o1)
		{
			ASSERT(1 == GetArgumentCount());
			return
				CaculateFunctionID(o1->GetTypeID()) == m_ID
				|| o1->isTypeOrDriverType(*GetArguments(0))
				|| m_pCompareFunction(o1, GetArguments(0));
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE bool isMatchArguments(const Rtti* o1, const Rtti* o2)
		{
			ASSERT(2 == GetArgumentCount());
			return
				CaculateFunctionID(
				o1->GetTypeID(),
				o2->GetTypeID()
				) == m_ID
				|| (
				o1->isTypeOrDriverType(*GetArguments(0))
				&& o2->isTypeOrDriverType(*GetArguments(1))
				)
				|| (
				m_pCompareFunction(o1, GetArguments(0))
				&& m_pCompareFunction(o2, GetArguments(1))
				);
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE bool isMatchArguments(const Rtti* o1, const Rtti* o2, const Rtti* o3)
		{
			ASSERT(3 == GetArgumentCount());
			return
				CaculateFunctionID(
				o1->GetTypeID(),
				o2->GetTypeID(),
				o3->GetTypeID()
				) == m_ID
				|| (
				o1->isTypeOrDriverType(*GetArguments(0))
				&& o2->isTypeOrDriverType(*GetArguments(1))
				&& o3->isTypeOrDriverType(*GetArguments(2))
				)
				|| (
				m_pCompareFunction(o1, GetArguments(0))
				&& m_pCompareFunction(o2, GetArguments(1))
				&& m_pCompareFunction(o3, GetArguments(2))
				);
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE bool isMatchArguments(const Rtti* o1, const Rtti* o2, const Rtti* o3, const Rtti* o4)
		{
			ASSERT(4 == GetArgumentCount());
			return
				CaculateFunctionID(
				o1->GetTypeID(),
				o2->GetTypeID(),
				o3->GetTypeID(),
				o4->GetTypeID()
				) == m_ID
				|| (
				o1->isTypeOrDriverType(*GetArguments(0))
				&& o2->isTypeOrDriverType(*GetArguments(1))
				&& o3->isTypeOrDriverType(*GetArguments(2))
				&& o4->isTypeOrDriverType(*GetArguments(3))
				)
				|| (
				m_pCompareFunction(o1, GetArguments(0))
				&& m_pCompareFunction(o2, GetArguments(1))
				&& m_pCompareFunction(o3, GetArguments(2))
				&& m_pCompareFunction(o4, GetArguments(3))
				);
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE bool isMatchArguments(const Rtti* o1, const Rtti* o2, const Rtti* o3, const Rtti* o4, const Rtti* o5)
		{
			ASSERT(5 == GetArgumentCount());
			return
				CaculateFunctionID(
				o1->GetTypeID(),
				o2->GetTypeID(),
				o3->GetTypeID(),
				o4->GetTypeID(),
				o5->GetTypeID()
				) == m_ID
				|| (
				o1->isTypeOrDriverType(*GetArguments(0))
				&& o2->isTypeOrDriverType(*GetArguments(1))
				&& o3->isTypeOrDriverType(*GetArguments(2))
				&& o4->isTypeOrDriverType(*GetArguments(3))
				&& o5->isTypeOrDriverType(*GetArguments(4))
				)
				|| (
				m_pCompareFunction(o1, GetArguments(0))
				&& m_pCompareFunction(o2, GetArguments(1))
				&& m_pCompareFunction(o3, GetArguments(2))
				&& m_pCompareFunction(o4, GetArguments(3))
				&& m_pCompareFunction(o5, GetArguments(4))
				);
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE bool isMatchArguments(const Rtti* o1, const Rtti* o2, const Rtti* o3, const Rtti* o4, const Rtti* o5, const Rtti* o6)
		{
			ASSERT(6 == GetArgumentCount());
			return
				CaculateFunctionID(
				o1->GetTypeID(),
				o2->GetTypeID(),
				o3->GetTypeID(),
				o4->GetTypeID(),
				o5->GetTypeID(),
				o6->GetTypeID()
				) == m_ID
				|| (
				o1->isTypeOrDriverType(*GetArguments(0))
				&& o2->isTypeOrDriverType(*GetArguments(1))
				&& o3->isTypeOrDriverType(*GetArguments(2))
				&& o4->isTypeOrDriverType(*GetArguments(3))
				&& o5->isTypeOrDriverType(*GetArguments(4))
				&& o6->isTypeOrDriverType(*GetArguments(5))
				)
				|| (
				m_pCompareFunction(o1, GetArguments(0))
				&& m_pCompareFunction(o2, GetArguments(1))
				&& m_pCompareFunction(o3, GetArguments(2))
				&& m_pCompareFunction(o4, GetArguments(3))
				&& m_pCompareFunction(o5, GetArguments(4))
				&& m_pCompareFunction(o6, GetArguments(5))
				);
		}
	public:
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE void* GetScriptResultValue(void* vm, void* p1, void* p2)
		{
			return m_pResultGetter(vm, p1, p2);
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE void* PushScriptReslut(void* vm, void* p1, void* p2)
		{
			return m_pResultPusher(vm, p1, p2);
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE const Rtti* GetArguments(int index)
		{
			return m_ArgumentRttis[index];
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE const Rtti* GetReturnType()
		{
			return m_pResult;
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE const eastl::string& GetName() const
		{
			return m_FuncName;
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE bool hasReturn() const//是否有返回值
		{
			return NULL != m_pResult;
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE uint GetArgumentCount() const
		{
			return m_ArgumentRttis.size();
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE ScriptProperty::FunctionType GetFunctionType() const
		{
			return m_eFunctionType;
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE void SetFunctionType(ScriptProperty::FunctionType ft)
		{
			m_eFunctionType = ft;
		}
		//-------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE int Invoke()
		{
			return ((int(*)(void))m_pFunction)();
		}
		HADES_FORCEINLINE int Invoke(void* p1)
		{
			return ((int(*)(void*))m_pFunction)(p1);
		}
		HADES_FORCEINLINE int Invoke(void* p1, void* p2)
		{
			return ((int(*)(void*, void*))m_pFunction)(p1, p2);
		}
		HADES_FORCEINLINE int Invoke(void* p1, void* p2, void* p3)
		{
			return ((int(*)(void*, void*, void*))m_pFunction)(p1, p2, p3);
		}
		HADES_FORCEINLINE int Invoke(void* p1, void* p2, void* p3, void* p4)
		{
			return ((int(*)(void*, void*, void*, void*))m_pFunction)(p1, p2, p3, p4);
		}
		HADES_FORCEINLINE int Invoke(void* p1, void* p2, void* p3, void* p4, void* p5)
		{
			return ((int(*)(void*, void*, void*, void*, void*))m_pFunction)(p1, p2, p3, p4, p5);
		}
		HADES_FORCEINLINE int Invoke(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6)
		{
			return ((int(*)(void*, void*, void*, void*, void*, void*))m_pFunction)(p1, p2, p3, p4, p5, p6);
		}
		HADES_FORCEINLINE int Invoke(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7)
		{
			return ((int(*)(void*, void*, void*, void*, void*, void*, void*))m_pFunction)(p1, p2, p3, p4, p5, p6, p7);
		}
		HADES_FORCEINLINE int Invoke(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8)
		{
			return ((int(*)(void*, void*, void*, void*, void*, void*, void*, void*))m_pFunction)(p1, p2, p3, p4, p5, p6, p7, p8);
		}
		HADES_FORCEINLINE int Invoke(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9)
		{
			return ((int(*)(void*, void*, void*, void*, void*, void*, void*, void*, void*))m_pFunction)(p1, p2, p3, p4, p5, p6, p7, p8, p9);
		}
		HADES_FORCEINLINE int Invoke(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9, void* p10)
		{
			return ((int(*)(void*, void*, void*, void*, void*, void*, void*, void*, void*, void*))m_pFunction)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10);
		}
		HADES_FORCEINLINE int Invoke(void* p1, void* p2, void* p3, void* p4, void* p5, void* p6, void* p7, void* p8, void* p9, void* p10, void* p11)
		{
			return ((int(*)(void*, void*, void*, void*, void*, void*, void*, void*, void*, void*, void*))m_pFunction)(p1, p2, p3, p4, p5, p6, p7, p8, p9, p10, p11);
		}
	};
}