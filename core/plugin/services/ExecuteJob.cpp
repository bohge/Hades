#include "script/ICoroutineInvoker.h"
#include "network/message/IMessage.h"
#include "script/FunctionRegister.h"
#include "services/ScriptServices.h"
#include "script/IVirtualMachine.h"
#include "serialize/ISerializer.h"
#include "script/IScriptSystem.h"
#include "script/ClassRegister.h"
#include "services/ExecuteJob.h"
#include "serialize/IDecoder.h"
#include "services/Protocol.h"
#include "script/IArguments.h"
#include "services/ISession.h"
#include "log/ILogSystem.h"
#include "ThreadData.h"

#include "core/Log.h"

namespace hc
{
	//---------------------------------------------------------------------------------------------------------
	ExecuteJob::ExecuteJob()
		: m_rpInstance(NULL)
		, m_rpIVirtualMachine(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	ExecuteJob::~ExecuteJob()
	{
		FOR_EACH(CoroutineAssetCache, it, m_CoroutineAssetCache)
		{
			SAFE_DELETE(it->second);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ExecuteJob::_DoYieldJob()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void ExecuteJob::_DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata)
	{
		ThreadData* arguments = static_cast<ThreadData*>(userdata);
		switch (m_rpSession->GetState())
		{
		case ISession::SS_ENTRY:
		{
								  //m_rpIVirtualMachine->Update();暂时不更新
								  uint id = m_rpSession->GetFunction();
								  ICoroutineInvoker* invoker = _GetCoroutine(id);
								  m_rpSession->SetInvoker(invoker);
								  break;
		}
		case ISession::SS_PASUE: break;
		case ISession::SS_DONE:
		case ISession::SS_ERROR:
		default:ASSERT(false); break;
		}
		
		m_spScriptServices->SetActiveSession(m_rpSession);
		m_rpSession->Invoke(m_rpInstance, arguments->ResultArgument, arguments->InputArgument);

		switch (m_rpSession->GetState())
		{
		case ISession::SS_PASUE: break;
		case ISession::SS_DONE:
		{
								 uint id = m_rpSession->GetFunction();
								 ICoroutineInvoker* invoke = m_rpSession->GetInvoker();
								 ASSERT(NULL != invoke);
								 CoroutineList* que = m_CoroutineAssetCache[id].second;
								 que->push_back(invoke);
								 break;
		}
		case ISession::SS_ERROR:
		{
								  ICoroutineInvoker* invoke = m_rpSession->GetInvoker();
								  m_rpInstance->UnsignInvoker(invoke);
								  break;
		}
		case ISession::SS_ENTRY:
		default:ASSERT(false); break;
		}
		m_spScriptServices->OnExecuteDone(m_rpSession);
	}
	//---------------------------------------------------------------------------------------------------------
	void ExecuteJob::Initialize(const ClassRegister* clr)
	{
		if (m_CoroutineAssetCache.empty())
		{
			uint count = clr->GetFunctionListCount();
			for (int i = 0; i < count; ++i)
			{
				FunctionRegister* func = clr->GetFunction(i);
				CoroutineList* que = NEW CoroutineList;
				if (m_rpInstance->isFunctionExist(func->GetName()))
				{
					for (int j = 0; j < EC_PREPARE_COROUTINE_SIZE; ++j)
					{
						ICoroutineInvoker* invoke = m_rpInstance->SignCoroutine(IArguments::SID(), func->GetName(), Instance::SID(), IArguments::SID(), IMessage::SID());
						invoke->SetupResume(IArguments::SID());
						que->push_back(invoke);
					}
				}
				m_CoroutineAssetCache.push_back(eastl::make_pair(func, que));
			}
			if (m_CoroutineAssetCache.empty())
			{
				HADESWARNING("empty services, name is %s\r\n", m_spScriptServices->GetTypeName());
				ASSERT(false);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	ICoroutineInvoker* ExecuteJob::_GetCoroutine(uint index)
	{
		ICoroutineInvoker* invoke = NULL;
		if (index < m_CoroutineAssetCache.size())
		{
			CoroutineList* que = m_CoroutineAssetCache[index].second;
			if (que->empty())
			{
				FunctionRegister* func = m_CoroutineAssetCache[index].first;
				invoke = m_rpInstance->SignCoroutine(IArguments::SID(), func->GetName(), Instance::SID(), IArguments::SID(), IMessage::SID());
				invoke->SetupResume(IArguments::SID());
			}
			else
			{
				invoke = que->back();
				que->pop_back();
			}
		}
		return invoke;
	}
}