#pragma once
#include "script/IFunctionIterator.h"
#include "services/IServicesSystem.h"
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "LocalServicesSystem.h"
#include "services/IServices.h"
#include "services/Protocol.h"
#include "script/IArguments.h"
#include "services/ISession.h"
#include "services/IAgent.h"




namespace hlss
{
	class IVirtualMachine;
	typedef eastl::pair<uint64, hc::ISession*> MachineUserData;
	//---------------------------------------------------------------------------------------------------------
	static int ServiceInvokeHanlder_WithResult(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::IAgent* self, hc::IArguments* argus)
	{
		//规定接受Protocol的使用者负责释放
		hc::Protocol* ptc = hc::IServicesSystem::Instance()->AllocProtocol();
		MachineUserData* md =
			static_cast<MachineUserData*>(vm->GetUserData());
		ptc->SetCallerGID(md->first);
		ptc->SetCallerSession(md->second->GetSessionID());
		ptc->SetFunction(func->GetIndex());
		if (!argus->Empty())
		{
			ptc->SetBuffer(argus->GetBuffer(), argus->GetBufferSize());
			argus->Reset();
		}
		self->RemoteInvoker(ptc);
		return md->second->PasueCoroutine();
	}
	//---------------------------------------------------------------------------------------------------------
	static int ServiceInvokeHanlder_NoResult(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::IAgent* self, hc::IArguments* argus)
	{
		//规定接受Protocol的使用者负责释放
		hc::Protocol* ptc = hc::IServicesSystem::Instance()->AllocProtocol();
		MachineUserData* md =
			static_cast<MachineUserData*>(vm->GetUserData());
		ptc->SetCallerGID(hc::nullid);
		ptc->SetCallerSession(hc::nullid);
		ptc->SetFunction(func->GetIndex());
		if (!argus->Empty())
		{
			ptc->SetBuffer(argus->GetBuffer(), argus->GetBufferSize());
			argus->Reset();
		}
		self->RemoteInvoker(ptc);
		return 0;
	}
	//---------------------------------------------------------------------------------------------------------
	static int Service_Equal(hc::IVirtualMachine* vm, hc::FunctionRegister* func, bool* res, hc::IAgent* lhs, hc::IAgent* rhs)
	{
		*res = lhs->isEqual(*rhs);
		return 1;
	}
	//---------------------------------------------------------------------------------------------------------
	static void IteratorFunction(hc::ClassRegister* clr, const hc::IFunctionIterator* fit, bool isoptimization)
	{
		//先迭代自己在迭代基类，保证一定的函数重载关系
		fit->Being();
		while (fit->Next())
		{
			switch (fit->GetFieldType())
			{
			case hc::DT_STRING:
			{
								  uint len;
								  const char* pstr = fit->GetStringField(len);
								  eastl::string name(pstr, len);
								  hc::FunctionRegister sf;
								  sf.Arguments(&hc::IArguments::RTTI());
								  if (fit->HaveRenturnValue())
								  {
									  sf.Function(&ServiceInvokeHanlder_WithResult, name);
								  }
								  else
								  {
									  if (isoptimization)
									  {
										  sf.Function(&ServiceInvokeHanlder_NoResult, name);
									  }
									  else
									  {
										  sf.Function(&ServiceInvokeHanlder_WithResult, name);
										  //hc::FunctionRegister sf2;
										  //sf2.Arguments(&hc::IArguments::RTTI());
										  //sf2.Function(&ServiceInvokeHanlder_NoResult, name + "_opt");
										  //clr->ClassFunction(sf2);
									  }
								  }
								  clr->ClassFunction(sf);
								  break;
			}
			default:ASSERT(false); break;
			}
		}
		fit->End();
		const hc::IFunctionIterator* basefit = fit->GetBaseFunctionIterator();
		if (basefit)
		{
			IteratorFunction(clr, basefit, isoptimization);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	static void IndexFunction(hc::ClassRegister& clr)
	{
		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::IArguments::RTTI());
			sf.Function(&ServiceInvokeHanlder_WithResult, hc::ScriptProperty::__GET);
			clr.ClassFunction(sf);
		}
		{
		hc::FunctionRegister sf;
		sf.Arguments(&hc::IArguments::RTTI());
		sf.Function(&ServiceInvokeHanlder_NoResult, hc::ScriptProperty::__SET);
		clr.ClassFunction(sf);
	}
	}
	//---------------------------------------------------------------------------------------------------------
	static void EquivalentFunction(hc::ClassRegister* clr)
	{
		{//增加一个对比函数
			hc::FunctionRegister sf;
			sf.Result(&hc::Bool::RTTI());
			sf.Arguments(&hc::IAgent::RTTI());
			sf.Function(&Service_Equal, hc::ScriptProperty::__EQU);
			clr->ClassFunction(sf);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	static void HandleCallback(uint64 gid, uint64 sid, hc::IArguments* res)
	{
		if (hc::nullid != gid)
		{
			hc::Protocol* ptc = LocalServicesSystem::Instance()->AllocProtocol();
			ptc->SetCallerGID(gid);
			ptc->SetCallerSession(sid);
			ptc->SetFunction(hc::nullhandle);
			if (!res->Empty())
			{
				ptc->SetBuffer(res->GetBuffer(), res->GetBufferSize());
				res->Reset();
			}
			LocalServicesSystem::Instance()->RemoteCallBack(ptc);
		}
	}
}