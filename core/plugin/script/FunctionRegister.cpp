#include "script/FunctionRegister.h"
#include "script/ScriptObjectpool.h"
#include "script/IScriptSystem.h"


using namespace eastl;
namespace hc
{
	//RTTI_TYPE_IMPLEMENT( ScriptFunction );
	//-------------------------------------------------------------------------------------------------------
	FunctionRegister::FunctionRegister(void)
		:m_pResult(NULL),
		m_pResultGetter(NULL),
		m_pResultPusher(NULL),
		m_pFunction(NULL),
		m_eFunctionType(ScriptProperty::FT_MEMBER),
		m_ID(0)
		, m_Index(0)
		, m_pCompareFunction(IScriptSystem::Instance()->GetTypeCompare())
	{

	}
	//-------------------------------------------------------------------------------------------------------
	FunctionRegister::~FunctionRegister(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void FunctionRegister::_CaculateID()
	{
		//和c++一样返回值不能做为重载的区分符号
		//这里可以用简单的循环去计算id，但是为了保证计算方法的一致（与脚本实现处，这里将其抽出来）
		switch (m_ArgumentRttis.size())
		{
		case 0: m_ID = CaculateFunctionID(); break;
		case 1: m_ID = CaculateFunctionID(
			m_ArgumentRttis[0]->GetTypeID()
			); break;
		case 2: m_ID = CaculateFunctionID(
			m_ArgumentRttis[0]->GetTypeID(),
			m_ArgumentRttis[1]->GetTypeID()
			); break;
		case 3: m_ID = CaculateFunctionID(
			m_ArgumentRttis[0]->GetTypeID(),
			m_ArgumentRttis[1]->GetTypeID(),
			m_ArgumentRttis[2]->GetTypeID()
			); break;
		case 4: m_ID = CaculateFunctionID(
			m_ArgumentRttis[0]->GetTypeID(),
			m_ArgumentRttis[1]->GetTypeID(),
			m_ArgumentRttis[2]->GetTypeID(),
			m_ArgumentRttis[3]->GetTypeID()
			); break;
		case 5: m_ID = CaculateFunctionID(
			m_ArgumentRttis[0]->GetTypeID(),
			m_ArgumentRttis[1]->GetTypeID(),
			m_ArgumentRttis[2]->GetTypeID(),
			m_ArgumentRttis[3]->GetTypeID(),
			m_ArgumentRttis[4]->GetTypeID()
			); break;
		case 6: m_ID = CaculateFunctionID(
			m_ArgumentRttis[0]->GetTypeID(),
			m_ArgumentRttis[1]->GetTypeID(),
			m_ArgumentRttis[2]->GetTypeID(),
			m_ArgumentRttis[3]->GetTypeID(),
			m_ArgumentRttis[4]->GetTypeID(),
			m_ArgumentRttis[5]->GetTypeID()
			); break;
		case 7: m_ID = CaculateFunctionID(
			m_ArgumentRttis[0]->GetTypeID(),
			m_ArgumentRttis[1]->GetTypeID(),
			m_ArgumentRttis[2]->GetTypeID(),
			m_ArgumentRttis[3]->GetTypeID(),
			m_ArgumentRttis[4]->GetTypeID(),
			m_ArgumentRttis[5]->GetTypeID(),
			m_ArgumentRttis[6]->GetTypeID()
			); break;
		case 8: m_ID = CaculateFunctionID(
			m_ArgumentRttis[0]->GetTypeID(),
			m_ArgumentRttis[1]->GetTypeID(),
			m_ArgumentRttis[2]->GetTypeID(),
			m_ArgumentRttis[3]->GetTypeID(),
			m_ArgumentRttis[4]->GetTypeID(),
			m_ArgumentRttis[5]->GetTypeID(),
			m_ArgumentRttis[6]->GetTypeID(),
			m_ArgumentRttis[7]->GetTypeID()
			); break;
		case 9: m_ID = CaculateFunctionID(
			m_ArgumentRttis[0]->GetTypeID(),
			m_ArgumentRttis[1]->GetTypeID(),
			m_ArgumentRttis[2]->GetTypeID(),
			m_ArgumentRttis[3]->GetTypeID(),
			m_ArgumentRttis[4]->GetTypeID(),
			m_ArgumentRttis[5]->GetTypeID(),
			m_ArgumentRttis[6]->GetTypeID(),
			m_ArgumentRttis[7]->GetTypeID(),
			m_ArgumentRttis[8]->GetTypeID()
			); break;
		case 10: m_ID = CaculateFunctionID(
			m_ArgumentRttis[0]->GetTypeID(),
			m_ArgumentRttis[1]->GetTypeID(),
			m_ArgumentRttis[2]->GetTypeID(),
			m_ArgumentRttis[3]->GetTypeID(),
			m_ArgumentRttis[4]->GetTypeID(),
			m_ArgumentRttis[5]->GetTypeID(),
			m_ArgumentRttis[6]->GetTypeID(),
			m_ArgumentRttis[7]->GetTypeID(),
			m_ArgumentRttis[8]->GetTypeID(),
			m_ArgumentRttis[9]->GetTypeID()
			); break;
		default:ASSERT(false);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void FunctionRegister::_SetupArgument()
	{
		for (int i = 0; i < m_ArgumentRttis.size(); ++i)
		{
			void* first = NULL;
			const Rtti* rtti = m_ArgumentRttis[i];
			m_ArgumentGetter.push_back(_GetArgumentPair(rtti));
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void FunctionRegister::Result(const Rtti* rtti)
	{
		ASSERT(NULL == m_pResult);
		m_pResult = rtti;
		m_pResultGetter = IScriptSystem::Instance()->GetNativeResultValueFunction(rtti);
		m_pResultPusher = IScriptSystem::Instance()->GetNativeResultPushFunction(rtti);
		_CaculateID();
	}
	//-------------------------------------------------------------------------------------------------------
	FunctionRegister::ArgIndexFuncPair FunctionRegister::_GetArgumentPair(const Rtti* rtti)
	{
		int first = 0;
		switch (rtti->GetTypeID())
		{
		case BOOL_RTTI_ID: first = ScriptObjectpool::POOL_INDEX_BOOL; break;
		case SHORT_RTTI_ID: first = ScriptObjectpool::POOL_INDEX_SHORT; break;
		case USHORT_RTTI_ID: first = ScriptObjectpool::POOL_INDEX_USHORT; break;
		case INT_RTTI_ID: first = ScriptObjectpool::POOL_INDEX_INT; break;
		case UINT_RTTI_ID: first = ScriptObjectpool::POOL_INDEX_UINT; break;
		case FLOAT_RTTI_ID: first = ScriptObjectpool::POOL_INDEX_FLOAT; break;
		case DOUBLE_RTTI_ID: first = ScriptObjectpool::POOL_INDEX_DOUBLE; break;
		case STRING_RTTI_ID: first = ScriptObjectpool::POOL_INDEX_STRING_READ; break;
		case FUNCTION_RTTI_ID:first = ScriptObjectpool::POOL_INDEX_FUNCTION; break;
		case INSTANCE_RTTI_ID:first = ScriptObjectpool::POOL_INDEX_INSTANCE; break;
		case INT64_RTTI_ID:first = ScriptObjectpool::POOL_INDEX_INT64; break;
		case UINT64_RTTI_ID:first = ScriptObjectpool::POOL_INDEX_UINT64; break;
		case ARGUMENTS_RTTI_ID:first = ScriptObjectpool::POOL_INDEX_ARGUMENTS; break;
		default:first = ScriptObjectpool::POOL_INDEX_EMPTY; break;
		}
		IScriptSystem::ArgumentFunction second = IScriptSystem::Instance()->GetScriptArgumentFunction(rtti);
		return eastl::make_pair(first, second);
	}
	//-------------------------------------------------------------------------------------------------------
	void* FunctionRegister::GetScriptParameter(int index, void* vm, void* l, void* stack)
	{
		void* res = NULL;
		ArgIndexFuncPair& funcpair = m_ArgumentGetter[index];
		res = ScriptObjectpool::Instance()->GetObject(funcpair.first);
		funcpair.second(res, vm, l, stack);
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	void FunctionRegister::Clearup(int count, void* args[])
	{
		ASSERT(count == m_ArgumentGetter.size());
		if (count == m_ArgumentGetter.size())
		{
			for (int i = 0; i < m_ArgumentGetter.size(); ++i)
			{
				void* arg = args[i];
				if (NULL != arg)
				{
					ArgIndexFuncPair& res = m_ArgumentGetter[i];
					ScriptObjectpool::Instance()->FreeObject(res.first, arg);
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void FunctionRegister::Arguments(const Rtti* rtti)
	{
		ASSERT(0 == m_ArgumentRttis.size());
		m_ArgumentRttis.push_back(rtti);
		_SetupArgument();
		_CaculateID();
	}
	//-------------------------------------------------------------------------------------------------------
	void FunctionRegister::Arguments(const Rtti* rtti1, const Rtti* rtti2)
	{
		ASSERT(0 == m_ArgumentRttis.size());
		m_ArgumentRttis.push_back(rtti1);
		m_ArgumentRttis.push_back(rtti2);
		_SetupArgument();
		_CaculateID();
	}
	//-------------------------------------------------------------------------------------------------------
	void FunctionRegister::Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3)
	{
		ASSERT(0 == m_ArgumentRttis.size());
		m_ArgumentRttis.push_back(rtti1);
		m_ArgumentRttis.push_back(rtti2);
		m_ArgumentRttis.push_back(rtti3);
		_SetupArgument();
		_CaculateID();
	}
	//-------------------------------------------------------------------------------------------------------
	void FunctionRegister::Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4)
	{
		ASSERT(0 == m_ArgumentRttis.size());
		m_ArgumentRttis.push_back(rtti1);
		m_ArgumentRttis.push_back(rtti2);
		m_ArgumentRttis.push_back(rtti3);
		m_ArgumentRttis.push_back(rtti4);
		_SetupArgument();
		_CaculateID();
	}
	//-------------------------------------------------------------------------------------------------------
	void FunctionRegister::Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5)
	{
		ASSERT(0 == m_ArgumentRttis.size());
		m_ArgumentRttis.push_back(rtti1);
		m_ArgumentRttis.push_back(rtti2);
		m_ArgumentRttis.push_back(rtti3);
		m_ArgumentRttis.push_back(rtti4);
		m_ArgumentRttis.push_back(rtti5);
		_SetupArgument();
		_CaculateID();
	}
	//-------------------------------------------------------------------------------------------------------
	void FunctionRegister::Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6)
	{
		ASSERT(0 == m_ArgumentRttis.size());
		m_ArgumentRttis.push_back(rtti1);
		m_ArgumentRttis.push_back(rtti2);
		m_ArgumentRttis.push_back(rtti3);
		m_ArgumentRttis.push_back(rtti4);
		m_ArgumentRttis.push_back(rtti5);
		m_ArgumentRttis.push_back(rtti6);
		_SetupArgument();
		_CaculateID();
	}
	//-------------------------------------------------------------------------------------------------------
	void FunctionRegister::Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7)
	{
		ASSERT(0 == m_ArgumentRttis.size());
		m_ArgumentRttis.push_back(rtti1);
		m_ArgumentRttis.push_back(rtti2);
		m_ArgumentRttis.push_back(rtti3);
		m_ArgumentRttis.push_back(rtti4);
		m_ArgumentRttis.push_back(rtti5);
		m_ArgumentRttis.push_back(rtti6);
		m_ArgumentRttis.push_back(rtti7);
		_SetupArgument();
		_CaculateID();
	}
	//-------------------------------------------------------------------------------------------------------
	void FunctionRegister::Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7, const Rtti* rtti8)
	{
		ASSERT(0 == m_ArgumentRttis.size());
		m_ArgumentRttis.push_back(rtti1);
		m_ArgumentRttis.push_back(rtti2);
		m_ArgumentRttis.push_back(rtti3);
		m_ArgumentRttis.push_back(rtti4);
		m_ArgumentRttis.push_back(rtti5);
		m_ArgumentRttis.push_back(rtti6);
		m_ArgumentRttis.push_back(rtti7);
		m_ArgumentRttis.push_back(rtti8);
		_SetupArgument();
		_CaculateID();
	}
	//-------------------------------------------------------------------------------------------------------
	void FunctionRegister::Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7, const Rtti* rtti8, const Rtti* rtti9)
	{
		ASSERT(0 == m_ArgumentRttis.size());
		m_ArgumentRttis.push_back(rtti1);
		m_ArgumentRttis.push_back(rtti2);
		m_ArgumentRttis.push_back(rtti3);
		m_ArgumentRttis.push_back(rtti4);
		m_ArgumentRttis.push_back(rtti5);
		m_ArgumentRttis.push_back(rtti6);
		m_ArgumentRttis.push_back(rtti7);
		m_ArgumentRttis.push_back(rtti8);
		m_ArgumentRttis.push_back(rtti9);
		_SetupArgument();
		_CaculateID();
	}
	//-------------------------------------------------------------------------------------------------------
	void FunctionRegister::Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7, const Rtti* rtti8, const Rtti* rtti9, const Rtti* rtti10)
	{
		ASSERT(0 == m_ArgumentRttis.size());
		m_ArgumentRttis.push_back(rtti1);
		m_ArgumentRttis.push_back(rtti2);
		m_ArgumentRttis.push_back(rtti3);
		m_ArgumentRttis.push_back(rtti4);
		m_ArgumentRttis.push_back(rtti5);
		m_ArgumentRttis.push_back(rtti6);
		m_ArgumentRttis.push_back(rtti7);
		m_ArgumentRttis.push_back(rtti8);
		m_ArgumentRttis.push_back(rtti9);
		m_ArgumentRttis.push_back(rtti10);
		_SetupArgument();
		_CaculateID();
	}
	//-------------------------------------------------------------------------------------------------------
	void FunctionRegister::Arguments(const Rtti* rtti1, const Rtti* rtti2, const Rtti* rtti3, const Rtti* rtti4, const Rtti* rtti5, const Rtti* rtti6, const Rtti* rtti7, const Rtti* rtti8, const Rtti* rtti9, const Rtti* rtti10, const Rtti* rtti11)
	{
		ASSERT(0 == m_ArgumentRttis.size());
		m_ArgumentRttis.push_back(rtti1);
		m_ArgumentRttis.push_back(rtti2);
		m_ArgumentRttis.push_back(rtti3);
		m_ArgumentRttis.push_back(rtti4);
		m_ArgumentRttis.push_back(rtti5);
		m_ArgumentRttis.push_back(rtti6);
		m_ArgumentRttis.push_back(rtti7);
		m_ArgumentRttis.push_back(rtti8);
		m_ArgumentRttis.push_back(rtti9);
		m_ArgumentRttis.push_back(rtti10);
		m_ArgumentRttis.push_back(rtti11);
		_SetupArgument();
		_CaculateID();
	}
}
