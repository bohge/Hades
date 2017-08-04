#include "network/server/IServerSystem.h"
#include "script/ICoroutineInvoker.h"
#include "serialize/ISerializable.h"
#include "network/server/IServer.h"
#include "script/IMemberIterator.h"
#include "script/IScriptInstance.h"
#include "script/IVirtualMachine.h"
#include "core/ThreadSafeOprator.h"
#include "SinglenodeApplication.h"
#include "thread/IJobDispatcher.h"
#include "serialize/ISerializer.h"
#include "thread/IThreadSystem.h"
#include "keeper/IKeeperSystem.h"
#include "thread/IThreadMutex.h"
#include "network/Messagepool.h"
#include "core/SmartPtr.hpp"
#include "keeper/Accessor.h"
#include "RequestUserData.h"
#include "network/Message.h"
#include "RequestHandle.h"
#include "core/Types.h"
#include "Configure.h"
#include "core/Log.h"


#include "timer/ITimerSystem.h"

using namespace hc;
using namespace eastl;
namespace hsna
{
	RTTI_TYPE_IMPLEMENT(RequestHandle);
	//---------------------------------------------------------------------------------------------------------
	RequestHandle::AccessPackage::AccessPackage(void)
		:Acessor(NULL)
		, Instance(NULL)
		, ReferenceCount(1)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	RequestHandle::AccessPackage::AccessPackage(IScriptInstance* ins)
		:Acessor(NULL)
		, Instance(ins)
		, ReferenceCount(1)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	RequestHandle::RequestHandle()
		: m_pIAccessPackage(NULL)
		, m_pCurrentInvoke(NULL)
		, m_rpMessage(NULL)
		, m_ID(nullid)
		, m_rpImmediatelyInstance(NULL)
	{
		m_pIAccessPackage = IKeeperSystem::Instance()->AccessPackageFactory();
		m_pIAccessPackage->SetAccessorCallback(MEMBER_FUNC_PTR(&RequestHandle::OnAccessorCallback));
	}
	//---------------------------------------------------------------------------------------------------------
	RequestHandle::~RequestHandle()
	{
		IKeeperSystem::Instance()->RecycleBin(m_pIAccessPackage);
	}
	//---------------------------------------------------------------------------------------------------------
	void RequestHandle::_DoYieldJob()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void RequestHandle::_DoJob(SmartPtr<IJob>& self, Object* userdata)
	{
		bool issucces = true;
		m_rpCurrentRequestUserData = static_cast<RequestUserData*>(userdata);
		if (NULL != m_rpCurrentRequestUserData)
		{
			//m_BegineTime = ITimerSystem::Instance()->GetTimevalue();
			IVirtualMachine* vm = m_rpCurrentRequestUserData->GetVirtualMachine();
			vm->SetGlobal(Configure::GLOBAL_CLIENTID, m_ID);
			vm->SetGlobal(Configure::GLOBAL_REQUEST, this, NULL, ScriptValue::NullCleaner);
			vm->SetGlobal(Configure::GLOBAL_MESSAGE, m_rpMessage, NULL, ScriptValue::NullCleaner);
			if (NULL == m_pCurrentInvoke)
			{
				switch (m_CallbackType)
				{
				case SCT_CONNECT:m_pCurrentInvoke = m_rpCurrentRequestUserData->GetConnectCoroutines();  break;
				case SCT_REQUEST:m_pCurrentInvoke = m_rpCurrentRequestUserData->GetRequestCoroutines(); break;
				case SCT_DISCONNECT:m_pCurrentInvoke = m_rpCurrentRequestUserData->GetDisconnectCoroutines(); break;
				default:ASSERT(false);
				}
				m_rpCurrentRequestUserData->UpdateVirtualMachine();
				m_pCurrentInvoke->Invoke();
			}
			else
			{
				uint64 errorid;
				issucces = _DoSetInstanceValue(errorid);
				if (!issucces)
				{
					DEBUGLOG("Request accessor failed! Message id %u, Accessor ID is %lld\r\n", m_rpMessage->GetID(), errorid);
				}
				else
				{
					if (NULL != m_rpImmediatelyInstance)
					{
						hc::IScriptInstance* local = m_rpImmediatelyInstance;
						m_rpImmediatelyInstance = NULL;
						m_pCurrentInvoke->Resume(RESUME_ACESSOR, local);
					}
					else
					{
						m_pCurrentInvoke->Resume(RESUME_BOOL, &issucces);
					}
				}
			}
			if (!issucces || ICoroutineInvoker::CS_YEILD != m_pCurrentInvoke->GetStatus())
			{
				m_rpCurrentRequestUserData->DsctoryCoroutine(m_pCurrentInvoke);
				m_pCurrentInvoke = NULL;
				m_pIAccessPackage->ReleaseAccessores();
				m_AccessPackageArrary.clear();
				Messagepool::Instance()->FreeMessage(m_rpMessage);
				SmartPtr<RequestHandle> handle = SmartPtr_Static_Cast<RequestHandle>(self);
				m_rpCurrentRequestUserData->GetApplication()->RecycleRequestHandle(handle);
			}
			else
			{
				if (!isPending())
				{
					//m_BegineTime = ITimerSystem::Instance()->GetTimevalue() - m_BegineTime;
					//DEBUGLOG("process message id %d time %lf\r\n",m_rpMessage->GetID(), m_BegineTime);
					ReleaseAccessores();
					Messagepool::Instance()->FreeMessage(m_rpMessage);
					SmartPtr<RequestHandle> handle = SmartPtr_Static_Cast<RequestHandle>(self);
					m_rpCurrentRequestUserData->GetApplication()->RecycleRequestHandle(handle);

					switch (m_CallbackType)
					{
					case SCT_CONNECT:m_rpCurrentRequestUserData->RecycleConnectCoroutine(m_pCurrentInvoke); break;
					case SCT_REQUEST:m_rpCurrentRequestUserData->RecycleRequestCoroutine(m_pCurrentInvoke); break;
					case SCT_DISCONNECT:m_rpCurrentRequestUserData->RecycleDsionnectCoroutine(m_pCurrentInvoke); break;
					default:ASSERT(false);
					}
					m_pCurrentInvoke = NULL;
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	bool RequestHandle::_DoSetInstanceValue(uint64& errorid)
	{
		bool res = true;
		ISerializer* ser = m_rpCurrentRequestUserData->GetSerializer();
		FOR_EACH(AccessPackageArrary, it, m_AccessPackageArrary)
		{
			IScriptInstance* instance = it->second.Instance;
			//instance->ClearMember();
			if (NULL == it->second.Acessor)//如果有空内存，说明访问的对象已经失去，禁止在脚本中对其操作
			{
				errorid = it->first;
				res = false;
				break;
			}
			else
			{
				ASSERT(it->second.Acessor->GetID() == it->second.Instance->GetGID()
					|| nullid == it->second.Instance->GetGID());
				if (!it->second.Acessor->isTimestampValid())
				{
					instance->ClearMember();
					instance->SetTimestamp(it->second.Acessor->GetID(), it->second.Acessor->GetTimestamp());
					instance->SetMember(ScriptProperty::SCRIPT_ACCESSOR_GETER, it->second.Acessor->GetID());
					instance->SetMember(ScriptProperty::SCRIPT_SELF_HANDLE, it->second.Instance, &ScriptValue::NullCleaner);
					const byte* buff = it->second.Acessor->GetBuffer();
					if (NULL != buff)
					{
						IDecoder* de = ser->GetDecoderFromBuffer(buff);
						instance->Deserialize(de);
						ser->ClearDecoder();
					}
				}
			}
		}
		return res;
	}
	void RequestHandle::_DoSaveInstanceValue(hc::Accessor* acc, hc::IScriptInstance* instance)
	{
		if ((NULL != acc && Accessor::AM_READ_WRITE == acc->GetAccessorMode()))
		{
			ISerializer* ser = m_rpCurrentRequestUserData->GetSerializer();
			int stamp = instance->GetTimestamp(acc->GetID());
			stamp = IKeeperSystem::IncreaseTimestamp(stamp);
			m_pIAccessPackage->UpdateTimestamp(acc->GetID(), stamp);
			instance->SetTimestamp(acc->GetID(), stamp);
			IEncoder* en = ser->GetEncoder();
			instance->Serialize(en);
			acc->CopyFrom(ser->GetBuffer(), ser->GetBufferSize());
			ser->ClearEncoder();
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void RequestHandle::OnAccessorCallback(const AccessinfoArray& accs)
	{
		FOR_EACH_CONST(AccessinfoArray, it, accs)
		{
			AccessPackage& ap = m_AccessPackageArrary[it->first];
			ap.Acessor = it->second.pAccessor;
		}
		_Resume();
	}
	//---------------------------------------------------------------------------------------------------------
	void RequestHandle::_DoCreateAccessor(IScriptInstance* instance)
	{
		uint64 id = m_pIAccessPackage->CreateAccessor();
		_DoReadwriteAccessor(id, instance);
	}
	//---------------------------------------------------------------------------------------------------------
	void RequestHandle::_DoUnrigorousAccessor(uint64 id, IScriptInstance* instance)
	{
		_CheckGID(id);
		m_pIAccessPackage->UnrigorousAccessor(id, instance->GetTimestamp(id));
		m_AccessPackageArrary.insert(make_pair(id, AccessPackage(instance)));
	}
	//---------------------------------------------------------------------------------------------------------
	void RequestHandle::_DoReadonlyAccessor(uint64 id, IScriptInstance* instance)
	{
		_CheckGID(id);
		m_pIAccessPackage->ReadonlyAccessor(id, instance->GetTimestamp(id));
		m_AccessPackageArrary.insert(make_pair(id, AccessPackage(instance)));
	}
	//---------------------------------------------------------------------------------------------------------
	void RequestHandle::_DoReadwriteAccessor(uint64 id, IScriptInstance* instance)
	{
		_CheckGID(id);
		m_pIAccessPackage->ReadwriteAccessor(id, instance->GetTimestamp(id));
		m_AccessPackageArrary.insert(make_pair(id, AccessPackage(instance)));
	}
	//---------------------------------------------------------------------------------------------------------
	bool RequestHandle::BeginRequest(int& magic)
	{
		if (m_pIAccessPackage->HasRequest())
		{
			FOR_EACH(AccessPackageArrary, it, m_AccessPackageArrary)
			{
				_DoSaveInstanceValue(it->second.Acessor, it->second.Instance);
			}
			_Pending();
			m_pIAccessPackage->BeginRequest();
			magic = m_pCurrentInvoke->Pasue();
			return true;
		}
		else
		{
			magic = 0;
			return false;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void RequestHandle::ReleaseAccessores()
	{
		FOR_EACH(AccessPackageArrary, it, m_AccessPackageArrary)
		{
			_DoSaveInstanceValue(it->second.Acessor, it->second.Instance);
		}
		m_pIAccessPackage->ReleaseAccessores();
		m_AccessPackageArrary.clear();
	}
	//---------------------------------------------------------------------------------------------------------
	void RequestHandle::ReleaseAccessor(uint64 gid)
	{
		AccessPackageArrary::iterator it = m_AccessPackageArrary.find(gid);
		if (m_AccessPackageArrary.end() == it)
		{
			m_pCurrentInvoke->GotError("\r\nPassed a invalid gid to function FreeAccessors");
		}
		else
		{
			ASSERT(0 != it->second.ReferenceCount);
			if (0 == --it->second.ReferenceCount)
			{
				_DoSaveInstanceValue(it->second.Acessor, it->second.Instance);
				m_AccessPackageArrary.erase(it);
				m_pIAccessPackage->ReleaseAccessor(gid);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IScriptInstance* RequestHandle::_TryReferenceAccessor(uint64 id, hc::Accessor::AccessMode am)
	{
		IScriptInstance* res = NULL;
		AccessPackageArrary::iterator it = m_AccessPackageArrary.find(id);
		if (m_AccessPackageArrary.end() != it)
		{
			++it->second.ReferenceCount;
			res = it->second.Instance;
			m_pIAccessPackage->MayChangeAccesorMode(id, am);
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	bool RequestHandle::hasRequest()
	{
		return !m_pIAccessPackage->isReleased();
	}

}