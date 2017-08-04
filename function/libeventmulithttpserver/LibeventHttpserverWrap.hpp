#pragma once
#include "serialize/ISerializeSystem.h"
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/IScriptSystem.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "services/IAgent.h"
#include "core/Types.h"
#include "Httpserver.h"

namespace hlhsp
{

	//-------------------------------------------------------------------------------------------------------
	static int LibeventHttpserverFunction_StartServer(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Httpserver* self, uint* tc, uint* type, uint* port, bool* isauto)
	{
		self->StartServer(*tc, (HttpserverJob::ServerType)*type, *port, *isauto);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int LibeventHttpserverFunction_Reply(hc::IVirtualMachine* vm, hc::FunctionRegister* func, bool* res, Httpserver* self, uint64* uid, uint* code, hc::StringBuffer* buf)
	{
		*res = self->Reply(*uid, *code, static_cast<const char*>(buf->pBuffer), buf->Length);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int LibeventHttpserverFunction_GetRequsetUrl(hc::IVirtualMachine* vm, hc::FunctionRegister* func, eastl::string* res, Httpserver* self, uint64* uid)
	{
		const char* ret = self->GetRequsetUrl(*uid);
		if (NULL != ret)
		{
			*res = ret;
		}
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int LibeventHttpserverFunction_GetAddress(hc::IVirtualMachine* vm, hc::FunctionRegister* func, eastl::string* res, Httpserver* self, uint64* uid)
	{
		const char* ret = self->GetAddress(*uid);
		if (NULL != ret)
		{
			*res = ret;
		}
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int LibeventHttpserverFunction_GetValue(hc::IVirtualMachine* vm, hc::FunctionRegister* func, eastl::string* res, Httpserver* self, uint64* uid, hc::StringBuffer* name)
	{
		*res = self->GetValue(*uid, static_cast<const char*>(name->pBuffer), name->Length);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int LibeventHttpserverFunction_GetType(hc::IVirtualMachine* vm, hc::FunctionRegister* func, eastl::string* res, Httpserver* self, uint64* uid, hc::StringBuffer* name)
	{
		*res = self->GetType(*uid, static_cast<const char*>(name->pBuffer), name->Length);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int LibeventHttpserverFunction_GetCommand(hc::IVirtualMachine* vm, hc::FunctionRegister* func, uint* res, Httpserver* self, uint64* uid)
	{
		*res = self->GetCommand(*uid);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int LibeventHttpserverFunction_GetRoot(hc::IVirtualMachine* vm, hc::FunctionRegister* func, eastl::string* res, Httpserver* self)
	{
		*res = self->GetRoot();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int LibeventHttpserverFunction_PushHandle(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Httpserver* self, hc::IAgent* agent)
	{
		self->PushHandle(agent);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int LibeventHttpserverFunction_Header(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Httpserver* self, uint64* uid, hc::StringBuffer* key, hc::StringBuffer* value)
	{
		self->Header(*uid, (const char*)key->pBuffer, (const char*)value->pBuffer);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_LibeventHttpserverFunction(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;		
		sc.ClassType(&Httpserver::RTTI());

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::UInt::RTTI(), &hc::UInt::RTTI(), &hc::UInt::RTTI(), &hc::Bool::RTTI());
			sf.Function(&hlhsp::LibeventHttpserverFunction_StartServer, "StartMultiServer");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::Bool::RTTI());
			sf.Arguments(&hc::UInt64::RTTI(), &hc::UInt::RTTI(), &hc::String::RTTI());
			sf.Function(&hlhsp::LibeventHttpserverFunction_Reply, "Reply");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::String::RTTI());
			sf.Arguments(&hc::UInt64::RTTI());
			sf.Function(&hlhsp::LibeventHttpserverFunction_GetRequsetUrl, "GetRequsetUrl");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::String::RTTI());
			sf.Arguments(&hc::UInt64::RTTI());
			sf.Function(&hlhsp::LibeventHttpserverFunction_GetAddress, "GetAddress");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::String::RTTI());
			sf.Arguments(&hc::UInt64::RTTI(), &hc::String::RTTI());
			sf.Function(&hlhsp::LibeventHttpserverFunction_GetValue, "GetValue");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::String::RTTI());
			sf.Arguments(&hc::UInt64::RTTI(), &hc::String::RTTI());
			sf.Function(&hlhsp::LibeventHttpserverFunction_GetType, "GetType");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::UInt::RTTI());
			sf.Arguments(&hc::UInt64::RTTI());
			sf.Function(&hlhsp::LibeventHttpserverFunction_GetCommand, "GetCommand");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::String::RTTI());
			sf.Function(&hlhsp::LibeventHttpserverFunction_GetRoot, "GetRoot");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::IAgent::RTTI());
			sf.Function(&hlhsp::LibeventHttpserverFunction_PushHandle, "PushHandle");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::UInt64::RTTI(), &hc::String::RTTI(), &hc::String::RTTI());
			sf.Function(&hlhsp::LibeventHttpserverFunction_Header, "Header");
			sc.ClassFunction(sf);
		}

		sc.ClassConstant(HttpserverJob::RC_REQ_GET, "GET");
		sc.ClassConstant(HttpserverJob::RC_REQ_POST, "POST");
		sc.ClassConstant(HttpserverJob::ST_HTTP, "HTTP");
		sc.ClassConstant(HttpserverJob::ST_HTTPS, "HTTPS");

		sys.SetNativeLibrary(name, sc);
		sys.SetNativeGlobal(name, Httpserver::RTTI().GetTypeName(), Httpserver::Instance());
	}
}