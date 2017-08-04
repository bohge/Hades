#pragma once
#include "core/Object.hpp"



namespace hc
{
	class IVirtualMachine;
	class IScriptInstance;
	class IFunctionInvoker;
}
namespace hsna
{
	class SinglenodeApplication;
	class BalanceUserData : public hc::Object
	{
		RTTI_TYPE_DECLEAR(BalanceUserData);
	public:
		hc::IVirtualMachine*	pMachine;
		hc::IScriptInstance*	pInstance;
		hc::IFunctionInvoker*	pFunction;
		SinglenodeApplication*	rpApplication;
	};
}
