#pragma once
#include "core/Object.hpp"


#include <EASTL/vector.h>

namespace hc
{
	class IScriptInstance;
	class IFunctionInvoker;
}
namespace hbtf
{
	class TreeTemplate : public hc::Object
	{
		RTTI_TYPE_DECLEAR(TreeTemplate);
	public:
		enum Constance
		{
			BTC_CONDITION_BEGIN = 0,
			BTC_CONDITION_END = 30,
			BTC_CONDITION_COUNT,
			BTC_ACTION_BEGIN = BTC_CONDITION_END + 1,
			BTC_ACTION_END = BTC_ACTION_BEGIN + 30,
			BTC_ACTION_COUNT = BTC_ACTION_END - BTC_CONDITION_END,
			BTC_METHOD_BEGIN = BTC_ACTION_END + 1,
			BTC_METHOD_END = BTC_METHOD_BEGIN + 30,
			BTC_METHOD_COUNT = BTC_METHOD_END - BTC_ACTION_END,
			BTC_ALL_COUNT = BTC_METHOD_END + 1,
		};
	private:
		typedef eastl::vector< hc::IFunctionInvoker* > InvokerArray;
	private:
		InvokerArray	m_InvokerArray;
	public:
		TreeTemplate();
		virtual ~TreeTemplate();
	public:
		void Initialize(hc::IScriptInstance* response);
	public:
		HADES_FORCEINLINE hc::IFunctionInvoker* GetFunction(uint i);
	};



	HADES_FORCEINLINE hc::IFunctionInvoker* TreeTemplate::GetFunction(uint i)
	{
		return m_InvokerArray[i];
	}
}