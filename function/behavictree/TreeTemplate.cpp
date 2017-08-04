#include "script/IFunctionInvoker.h"
#include "script/IScriptInstance.h"
#include "TreeTemplate.h"
#include "core/Types.h"


#include <sstream>


using namespace hc;
namespace hbtf
{
	RTTI_TYPE_IMPLEMENT(TreeTemplate);
	//---------------------------------------------------------------------------------------------------------
	TreeTemplate::TreeTemplate()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	TreeTemplate::~TreeTemplate()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void TreeTemplate::Initialize(hc::IScriptInstance* response)
	{
		m_InvokerArray.reserve(BTC_ALL_COUNT);
		for (int i = 0; i < BTC_CONDITION_COUNT; ++i)
		{
			std::stringstream ss;
			ss << "Condition" << i;
			std::string name = ss.str();
			IFunctionInvoker* res = response->SignFuncitonWithReturn(Bool::SID(), name.c_str(), Instance::SID());
			m_InvokerArray.push_back(res);
		}
		for (int i = 0; i < BTC_ACTION_COUNT; ++i)
		{
			std::stringstream ss;
			ss << "Action" << i;
			std::string name = ss.str();
			IFunctionInvoker* res = response->SignFuncitonWithReturn(Int::SID(), name.c_str(), Instance::SID());
			m_InvokerArray.push_back(res);
		}
		for (int i = 0; i < BTC_METHOD_COUNT; ++i)
		{
			std::stringstream ss;
			ss << "Method" << i;
			std::string name = ss.str();
			IFunctionInvoker* res = response->SignFunciton(name.c_str(), Instance::SID());
			m_InvokerArray.push_back(res);
		}
	}

}