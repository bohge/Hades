#include "network/socket/ISocketSystem.h"
#include "serialize/ISerializeSystem.h"
#include "network/socket/SocketProxy.h"
#include "network/message/IMessage.h"
#include "script/IScriptSystem.h"
#include "RemoteServicesSystem.h"
#include "thread/IThreadEvent.h"
#include "script/IArguments.h"
#include "core/Bufferpool.h"
#include "AdaptiveSession.h"
#include "RemoteServices.h"
#include "LocalServices.h"
#include "AllRegister.hpp"
#include "RemoteResumer.h"
#include "AdaptiveAgent.h"
#include "ClassCache.h"
#include "Configure.h"
#include "Utility.h"



using namespace hc;
using namespace eastl;
namespace hrss
{
	void ScriptServicesRecycle(void* obj)
	{
		ScriptServices* ss = static_cast<ScriptServices*>(obj);
		IServicesSystem::Instance()->RecycleScriptService(ss);
	}

	//---------------------------------------------------------------------------------------------------------
	RemoteServicesSystem::RemoteServicesSystem()
		:m_pServerProxy(NULL)
		, m_pMainAgent(NULL)
		, m_IPHash(nullhandle)
		, m_pIThreadRWLock(NULL)
		, m_pServerEvent(NULL)
		, m_FuncRecevieAgent(nullhandle)
		, m_FuncRemotenodeReady(nullhandle)
		, m_FuncDisconnect(nullhandle)
		, m_ScriptInitialized(false)
		, m_ServerType(IServerSystem::ST_COMMON_TL)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	RemoteServicesSystem::~RemoteServicesSystem()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::Initialize()
	{
		ASSERT(128 == sizeof(AgentData));

		ISerializeSystem::Instance()->RegisterSerializable<AdaptiveAgent>();
		ISerializeSystem::Instance()->RegisterSerializable<RemoteResumer>();
		IServicesSystem::Initialize();
		Register_All(hc::Configure::NATIVE_LIBRARY_SYSTEM, *IScriptSystem::Instance());
		m_pIThreadRWLock = static_cast<IThreadRWLock*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_RWLOCK));
		m_pServerEvent = static_cast<IThreadEvent*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_EVENT));
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::Update()
	{
		IServicesSystem::Update();
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::Exit()
	{
		FOR_EACH(ClusterMap, kvpair, m_ClusterMap)
		{
			kvpair->second->RemoteSocketProxy->Disconnect();
			ISocketSystem::Instance()->RecycleBin(kvpair->second->RemoteSocketProxy);
			IThreadSystem::Instance()->RecycleBin(kvpair->second->ClusterRWLock);
			SAFE_DELETE(kvpair->second);
		}
		IThreadSystem::Instance()->RecycleBin(m_pServerEvent);
		IThreadSystem::Instance()->RecycleBin(m_pIThreadRWLock);
		IServicesSystem::Exit();
		ClassCache::Destroy();
		IServerSystem::Instance()->RecycleBin(m_pServerProxy);
	}
	//---------------------------------------------------------------------------------------------------------
	hc::SmartPtr<hc::IServices> RemoteServicesSystem::_OnServiceFactory(const eastl::string& file, hc::ScriptServices* ss)
	{
		m_pServerEvent->Lock();
		if (NULL != m_pServerProxy && hc::nullhandle == m_IPHash)
		{
			HADESWARNING("may create servcie after set befor start");
			m_pServerEvent->Wait();
		}
		else
		{
			m_pServerEvent->Unlock();
		}
		//程序入口rpc没有hosthash，应此不能被传递，这种设计暂时被接受，应为目前来看没有这个需求
		SmartPtr<LocalServices> res = SmartPtr<LocalServices>(NEW LocalServices);
		SmartPtr<ScriptServices> sss = SmartPtr<ScriptServices>(ss, &ScriptServicesRecycle);
		res->SetScriptServices(sss);
		const ClassCache::InfoPair* info = ClassCache::Instance()->GetClassCached(file, ss->GetScriptInstance());
		sss->Initialize(info->second);
		sss->SetSelfHandle(sss);
		res->SetAgentRtti(info->first);
		res->SetClassInfomation(info->second);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::ISession* RemoteServicesSystem::_DoAllocSession()
	{
		return NEW AdaptiveSession;
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::_DoFreeSession(hc::ISession* ss)
	{
		SAFE_DELETE(ss);
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::_OnServicesStart()
	{
		ClassCache::Create();
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::SetServerProxy(hc::ServerProxy* ser)
	{
		if (NULL == ser)
		{
			HADESERROR("Remote services need IServer component!");
		}
		else
		{
			if (ser->isRunning())
			{
				HADESERROR("Set server proxy befor server statr!");
			}
			else
			{
				if (NULL != m_pServerProxy)
				{
					IServerSystem::Instance()->RecycleBin(m_pServerProxy);
				}
				m_pServerProxy = IServerSystem::Instance()->AllocServerProxy(ser->GetIndex());
				m_pServerProxy->SetServerCallback(MEMBER_FUNC_PTR(&RemoteServicesSystem::OnReciveMessage));
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::SetHandler(hc::IAgent* agent)
	{
		if (NULL != m_pMainAgent)
		{
			RecycleBin(m_pMainAgent);
		}
		if (NULL != agent)
		{
			SmartPtr<IServices> ser = GetSerivices(agent->GetGID());
			m_pMainAgent = ser->AllocAgent();
			m_FuncRecevieAgent = m_pMainAgent->GetFunctionIndex(Configure::FUNCTION_RECEVICE_AGENT_NAME);
			m_FuncRemotenodeReady = m_pMainAgent->GetFunctionIndex(Configure::FUNCTION_REMOTENODE_READY_NAME);
			m_FuncDisconnect = m_pMainAgent->GetFunctionIndex(Configure::FUNCTION_REMOTENODE_DISCONNECT_NAME);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::EraseRemoteServicesFlag(uint hosthash, uint64 hostgid)
	{
		m_pIThreadRWLock->ReadLock();
		ClusterMap::iterator host = m_ClusterMap.find(hosthash);
		if (m_ClusterMap.end() == host)
		{
			HADESERROR("Try to free unkown remote services or remote close by peer!");
		}
		else
		{
			uint index = Utility::GetIndexFormKey(hostgid);
			host->second->ClusterRWLock->WriteLock();
			host->second->LocalServicesIndex[index] = nullid;
			host->second->ClusterRWLock->Unlock();
		}
		m_pIThreadRWLock->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 RemoteServicesSystem::_GetCreateRemoteServices(const AgentData& data)
	{
		uint64 resgid = nullid;
		ClusterMap::iterator host = m_ClusterMap.find(data.OriginalHash);
		if (m_ClusterMap.end() == host)
		{
			HADESERROR("Try send to unkown machine!");
		}
		else
		{
			uint index = Utility::GetIndexFormKey(data.OriginalGID);
			host->second->ClusterRWLock->WriteLock();
			uint64 localgid = host->second->LocalServicesIndex[index];
			if (nullid != localgid)
			{
				resgid = localgid;
			}
			else
			{
				const ClassCache::InfoPair* info = ClassCache::Instance()->GetClassCached(data.ServiceFile);
				if (info)
				{
					SmartPtr<RemoteServices> rms = SmartPtr<RemoteServices>(NEW RemoteServices);
					rms->SetServiceFile(data.ServiceFile);
					rms->SetHostSocket(host->second->RemoteSocketProxy);
					rms->SetHostHash(data.OriginalHash);
					rms->SetHostGID(data.OriginalGID);
					rms->SetAgentRtti(info->first);
					rms->SetClassInfomation(info->second);
					uint64 gid = _PushServices(rms);
					rms->SetGID(gid);
					host->second->LocalServicesIndex[index] = gid;
					resgid = gid;
					//发送增加信息
					IMessage* msg = static_cast<IMessage*>(hc::ISerializeSystem::Instance()
						->GetSerializable(IMessage::RTTI().GetFactor()));
					SerializeIncrement(data.OriginalGID, msg);
					host->second->RemoteSocketProxy->Send(msg);
					ISerializeSystem::Instance()->FreeSerializable(msg);
				}
			}
			host->second->ClusterRWLock->Unlock();
		}
		return resgid;
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::_OnRecevieAgent(hc::IMessage* msg)
	{
		AgentData agentdata;
		IMessage* pushmsg = static_cast<IMessage*>(hc::ISerializeSystem::Instance()
			->GetSerializable(IMessage::RTTI().GetFactor()));
		const byte* buffer = msg->GetMessageBuffer();
		uint len = msg->GetMessageLength();
		uint msglen = len - sizeof(AgentData);
		pushmsg->FromBuffer(buffer, msglen);
		//agentdata = *(AgentData*)(buffer + msglen);//这句话会导致崩溃，原因不明
		memcpy(&agentdata, buffer + msglen, sizeof(AgentData));
		if (agentdata.OriginalHash == m_IPHash)
		{
			HADESERROR("Recevie agent from myself\r\n");
		}
		else
		{
			m_pIThreadRWLock->ReadLock();
			ClusterMap::iterator host = m_ClusterMap.find(agentdata.OriginalHash);
			if (m_ClusterMap.end() == host)
			{
				HADESERROR("Try send to unkown machine!\r\n");
			}
			else
			{
				uint64 gid = _GetCreateRemoteServices(agentdata);
				if (!hasSerivices(gid))
				{
					HADESERROR("Asynchronous bug!\r\n");
				}
				else
				{
					SmartPtr<RemoteServices> rms = GetSerivices(gid);
					if (rms.isValid())
					{
						AdaptiveAgent* nadp = static_cast<AdaptiveAgent*>(rms->AllocAgent());
						IArguments* argus = IScriptSystem::Instance()->AllocArguments();
						argus->PushUInt64(host->second->Gid);
						argus->PushSerializable(nadp);
						argus->PushSerializable(pushmsg);
						Protocol* ptc = IServicesSystem::Instance()->AllocProtocol();
						ptc->SetBuffer(argus->GetBuffer(), argus->GetBufferSize());
						ptc->SetFunction(m_FuncRecevieAgent);
						m_pMainAgent->RemoteInvoker(ptc);
						rms->PasueAutoFree();
						rms->FreeAgent(nadp);
						IScriptSystem::Instance()->FreeArguments(argus);
					}
				}
			}
			m_pIThreadRWLock->Unlock();
		}
		ISerializeSystem::Instance()->FreeSerializable(msg);
		ISerializeSystem::Instance()->FreeSerializable(pushmsg);
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::_OnRemoteCall(hc::IMessage* msg)
	{
		Protocol* ptc = IServicesSystem::Instance()->AllocProtocol();
		uint64 gid = ParseRemoteCall(ptc, msg);
		//HADESLOG("_OnRemoteCall func %d", ptc->GetFunction());
		if (!hasSerivices(gid))
		{
			HADESERROR("TCP recevie a unkown host call. GID %llu", gid);
			//需要处理错误回掉
			IServicesSystem::Instance()->FreeProtocol(ptc);
		}
		else
		{
			SmartPtr<IServices> spservice = GetSerivices(gid);
			if (spservice.isValid())
			{
				spservice->RemoteInvoker(ptc);
			}
		}
		ISerializeSystem::Instance()->FreeSerializable(msg);
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::_OnRemoteCallBack(hc::IMessage* msg)
	{
		Protocol* ptc = IServicesSystem::Instance()->AllocProtocol();
		ptc->Parse(msg->GetMessageBuffer(), msg->GetMessageLength());
		RemoteCallBack(ptc);
		ISerializeSystem::Instance()->FreeSerializable(msg);
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IMessage* RemoteServicesSystem::_GetCallMessage()
	{
		IMessage* msg = static_cast<IMessage*>(hc::ISerializeSystem::Instance()
			->GetSerializable(IMessage::RTTI().GetFactor()));
		uint len = m_Address.size() + 1;
		uint rawsize = sizeof(m_ServerType) + sizeof(len) + len;
		byte* buffer = msg->PrepareMessage(Configure::RMID_READY_CALL, rawsize);

		memcpy(buffer, &m_ServerType, sizeof(m_ServerType));
		memcpy(buffer + sizeof(m_ServerType), &len, sizeof(len));
		memcpy(buffer + sizeof(m_ServerType)+sizeof(len), m_Address.c_str(), len);
		return msg;
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::ScriptInitialized()
	{
		m_pIThreadRWLock->WriteLock();
		m_ScriptInitialized = true;
		IMessage* msg = _GetCallMessage();
		FOR_EACH(ClusterMap, kvpair, m_ClusterMap)
		{
			kvpair->second->ClusterRWLock->ReadLock();
			if (kvpair->second->isConnected)
			{
				kvpair->second->RemoteSocketProxy->Send(msg);
				_TryRemotenodeReadyInvoke(kvpair->second);
			}
			kvpair->second->ClusterRWLock->Unlock();
		}
		ISerializeSystem::Instance()->FreeSerializable(msg);
		m_pIThreadRWLock->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::OnConnected(ISocket* sk)
	{
		const string& ip = sk->GetSocketAddress();
		uint hash = Utility::HashCode(ip);
		m_pIThreadRWLock->ReadLock();
		ClusterMap::iterator it = m_ClusterMap.find(hash);
		if (m_ClusterMap.end() != it)
		{
			it->second->ClusterRWLock->WriteLock();
			it->second->isConnected = true;
			it->second->ClusterRWLock->Unlock();
			if (m_ScriptInitialized)
			{
				IMessage* msg = _GetCallMessage();
				it->second->RemoteSocketProxy->Send(msg);
				ISerializeSystem::Instance()->FreeSerializable(msg);
				_TryRemotenodeReadyInvoke(it->second);
			}
		}
		m_pIThreadRWLock->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	RemoteServicesSystem::ClusterStack* RemoteServicesSystem::_ConnectNode(hc::ISocketSystem::SocketType st, const eastl::string& ip)
	{
		SocketProxy* sk = ISocketSystem::Instance()->SocketFactory(st);
		uint hash = Utility::HashCode(ip);
		ClusterStack* cs = NEW ClusterStack;
		cs->ClusterRWLock = static_cast<IThreadRWLock*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_RWLOCK));
		cs->ClusterRWLock->WriteLock();
		cs->isConnected = false;
		cs->isReciveEcho = false;
		cs->isInvoked = false;
		cs->Gid = 0;
		cs->RemoteSocketProxy = sk;
		cs->LocalServicesIndex.resize(SSC_SERVICES_SIZE, nullid);
		cs->ClusterRWLock->Unlock();
		m_ClusterMap.insert(make_pair(hash, cs));
		sk->Connect(ip, MEMBER_FUNC_PTR(&RemoteServicesSystem::OnConnected));
		return cs;
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::_OnRecevieCall(uint64 id, hc::IMessage* msg)
	{
		//HADESLOG("_OnRecevieCall");
		uint len;
		ISocketSystem::SocketType st;
		const byte* data = msg->GetMessageBuffer();
		memcpy(&st, data, sizeof(st));
		data += sizeof(st);
		memcpy(&len, data, sizeof(len));
		data += sizeof(len);
		char* str = NEW char[len];
		memcpy(str, data, len);
		ISerializeSystem::Instance()->FreeSerializable(msg);
		uint hash = Utility::HashCode(str);
		uint64 specgid = hash;
		specgid = SET_FLAG(specgid, SD_SERVICE_FLAG);
		m_pServerProxy->SetSpecific(id, specgid);
		m_pIThreadRWLock->WriteLock();
		ClusterMap::iterator it = m_ClusterMap.find(hash);
		ClusterStack* cs = NULL;
		if (m_ClusterMap.end() == it)
		{
			cs = _ConnectNode(st, str);
		}
		else
		{
			cs = it->second;
		}
		m_pIThreadRWLock->Unlock();
		cs->ClusterRWLock->WriteLock();
		cs->Gid = id;
		cs->isReciveEcho = true;
		cs->ClusterRWLock->Unlock();
		_TryRemotenodeReadyInvoke(cs);
		SAFE_DELETE_ARRAY(str);
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::_TryRemotenodeReadyInvoke(ClusterStack* cs)
	{
		cs->ClusterRWLock->ReadLock();
		if (cs->isConnected
			&& cs->isReciveEcho
			&& !cs->isInvoked)
		{
			cs->isInvoked = true;
			uint64 id = cs->Gid;
			uint iphash = Utility::HashCode(cs->RemoteSocketProxy->GetSocketAddress());
			IArguments* argus = IScriptSystem::Instance()->AllocArguments();
			argus->PushUInt64(id);
			argus->PushUInt(iphash);
			Protocol* ptc = IServicesSystem::Instance()->AllocProtocol();
			ptc->SetBuffer(argus->GetBuffer(), argus->GetBufferSize());
			ptc->SetFunction(m_FuncRemotenodeReady);
			m_pMainAgent->RemoteInvoker(ptc);
			IScriptSystem::Instance()->FreeArguments(argus);
		}
		cs->ClusterRWLock->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::_OnRecevieFree(hc::IMessage* msg)
	{
		uint64 gid = ParseFree(msg);
		if (!hasSerivices(gid))
		{
			HADESERROR("TCP recevie a unkown agent free. GID %llu", gid);
		}
		else
		{
			SmartPtr<IServices> spservice = GetSerivices(gid);
			if (spservice.isValid())
			{
				spservice->DecrementAgent();
			}
		}
		ISerializeSystem::Instance()->FreeSerializable(msg);
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::_OnRecevieIncrment(uint64 id, hc::IMessage* msg)
	{
		uint64 specgid = m_pServerProxy->GetSpecific(id);
		uint hash = GET_GID(specgid);
		uint64 gid = ParseIncrement(msg);
		if (!hasSerivices(gid))
		{
			HADESERROR("TCP recevie a unkown service refrence increment. GID %llu", gid);
		}
		else
		{
			SmartPtr<IServices> spservice = GetSerivices(gid);
			if (spservice.isValid())
			{
				m_pIThreadRWLock->ReadLock();
				ClusterMap::iterator it = m_ClusterMap.find(hash);
				if (m_ClusterMap.end() != it)
				{
					it->second->ClusterRWLock->WriteLock();
					it->second->SendServicesIndex.push_back(gid);
					it->second->ClusterRWLock->Unlock();
				}
				m_pIThreadRWLock->Unlock();
				spservice->IncrementAgent();
			}
		}
		ISerializeSystem::Instance()->FreeSerializable(msg);
	}
	//---------------------------------------------------------------------------------------------------------
	bool RemoteServicesSystem::_OnDisconnect(uint64 id)
	{
		uint64 specgid = m_pServerProxy->GetSpecific(id);
		if (hc::nullid != specgid && IS_FLAG(specgid, SD_SERVICE_FLAG))
		{
			//HADESLOG("_OnDisconnect %llu", id);
			uint hash = GET_GID(specgid);
			m_pIThreadRWLock->WriteLock();
			ClusterMap::iterator it = m_ClusterMap.find(hash);
			if (m_ClusterMap.end() != it)
			{
				for (eastl::vector<uint64>::iterator sgid = it->second->SendServicesIndex.begin();
					sgid != it->second->SendServicesIndex.end();
					++sgid)
				{
					SmartPtr<IServices> spservice = GetSerivices(*sgid);
					if (spservice.isValid())
					{
						spservice->DecrementAgent();
					}
				}
				it->second->RemoteSocketProxy->Disconnect();
				ISocketSystem::Instance()->RecycleBin(it->second->RemoteSocketProxy);
				IThreadSystem::Instance()->RecycleBin(it->second->ClusterRWLock);
				m_ClusterMap.erase(it);
				if (nullhandle != m_FuncDisconnect)
				{
					IArguments* argus = IScriptSystem::Instance()->AllocArguments();
					argus->PushUInt64(id);
					Protocol* ptc = IServicesSystem::Instance()->AllocProtocol();
					ptc->SetBuffer(argus->GetBuffer(), argus->GetBufferSize());
					ptc->SetFunction(m_FuncDisconnect);
					m_pMainAgent->RemoteInvoker(ptc);
					IScriptSystem::Instance()->FreeArguments(argus);
				}
				m_pIThreadRWLock->Unlock();
				return true;
			}
			m_pIThreadRWLock->Unlock();
		}
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	bool RemoteServicesSystem::OnReciveMessage(hc::IServer* ser, uint64 id, hc::CallbackType ct, hc::IMessage* message)
	{
		//处理rpc
		switch (ct)
		{
		case SCT_SERVER_START:
		{
								 m_pServerEvent->Lock();
								 m_Address = m_pServerProxy->GetServerAddress();
								 m_ServerType = m_pServerProxy->GetServerType();
								 m_IPHash = Utility::HashCode(m_Address);
								 _SetUUID(m_IPHash);
								 m_pServerEvent->Signal();
								 return true;
		}
		case SCT_CONNECT:
		{
			break;
		}
		case SCT_REQUEST:
		{
			if (NULL != message)
			{
				switch (message->GetID())
				{
				case Configure::RMID_READY_CALL:_OnRecevieCall(id, message); return true;
				case Configure::RMID_AGENT_SEND:_OnRecevieAgent(message); return true;
				case Configure::RMID_AGENT_INCR:_OnRecevieIncrment(id, message); return true;
				case Configure::RMID_AGENT_FREE:_OnRecevieFree(message); return true;
				case Configure::RMID_REMOTE_CALL:_OnRemoteCall(message); return true;
				case Configure::RMID_REMOTE_CALL_BACK:_OnRemoteCallBack(message); return true;
				}
			}
			break;
		}
		case SCT_DISCONNECT:return _OnDisconnect(id);
		default:ASSERT(false);
		}
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::SmartPtr<hc::IServices> RemoteServicesSystem::GetRemoteService(const AgentData& data)
	{
		m_pIThreadRWLock->ReadLock();
		uint64 gid = _GetCreateRemoteServices(data);
		m_pIThreadRWLock->Unlock();
		return GetSerivices(gid);
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::SendAgent(uint index, hc::IAgent* agent, hc::IMessage* msg)
	{
		AdaptiveAgent* adpagent = static_cast<AdaptiveAgent*>(agent);
		if (index == adpagent->GetCurrentHash())
		{
			HADESERROR("Try to send a agent to myself\r\n");
		}
		else
		{
			m_pIThreadRWLock->ReadLock();
			ClusterMap::iterator it = m_ClusterMap.find(index);
			if (m_ClusterMap.end() == it)
			{
				HADESERROR("Try send to unkown machine!\r\n");
			}
			else
			{
				if (adpagent->GetCurrentHash() != m_IPHash)
				{
					HADESERROR("Can not send remote agent!\r\n");
				}
				else
				{
					uint64 gid = adpagent->GetGID();
					if (hasSerivices(gid))
					{
						SmartPtr<IServices> spservice = GetSerivices(gid);
						if (spservice.isValid())
						{//发送的时候不增加引用计数，当客户端收到后增加
							IMessage* sendmsg = static_cast<IMessage*>(hc::ISerializeSystem::Instance()
								->GetSerializable(IMessage::RTTI().GetFactor()));

							uint orilen = msg->GetLength();
							uint len = adpagent->GetLength();
							uint fullsize = orilen + len;
							byte* buffer = sendmsg->PrepareMessage(Configure::RMID_AGENT_SEND, fullsize);
							memcpy(buffer, msg->GetBuffer(), orilen);
							memcpy(buffer + orilen, adpagent->GetBuffer(), len);
							it->second->RemoteSocketProxy->Send(sendmsg);
							ISerializeSystem::Instance()->FreeSerializable(sendmsg);
						}
					}
				}
			}
			m_pIThreadRWLock->Unlock();
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void RemoteServicesSystem::SendRemoteCallBack(hc::Protocol* ptc)
	{
		uint hash = ptc->GetCallerHash();
		m_pIThreadRWLock->ReadLock();
		ClusterMap::iterator it = m_ClusterMap.find(hash);
		if (m_ClusterMap.end() == it)
		{
			HADESERROR("Try call back to unkown machine!\r\n");
		}
		else
		{
			IMessage* msg = static_cast<IMessage*>(hc::ISerializeSystem::Instance()
				->GetSerializable(IMessage::RTTI().GetFactor()));
			uint ptcsize = ptc->GetUsingSize();
			byte* buffer = msg->PrepareMessage(Configure::RMID_REMOTE_CALL_BACK, ptcsize);
			uint len = ptc->Serialize(buffer);
			ASSERT(len == ptcsize);
			it->second->RemoteSocketProxy->Send(msg);
			ISerializeSystem::Instance()->FreeSerializable(msg);
		}
		IServicesSystem::Instance()->FreeProtocol(ptc);
		m_pIThreadRWLock->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	RemoteServicesSystem::ClusterStack* RemoteServicesSystem::ConnectNode(hc::ISocketSystem::SocketType st, const eastl::string& ip)
	{
		m_pIThreadRWLock->WriteLock();
		RemoteServicesSystem::ClusterStack* res = _ConnectNode(st, ip);
		m_pIThreadRWLock->Unlock();
		return res;
	}
	typedef eastl::pair<uint64, hc::ISession*> MachineUserData;
	//---------------------------------------------------------------------------------------------------------
	IResumer* RemoteServicesSystem::AllocResumer(IVirtualMachine* vm)
	{
		RemoteResumer* rr = static_cast<RemoteResumer*>
			(ISerializeSystem::Instance()
			->GetSerializable(IResumer::RTTI().GetFactor()));
		MachineUserData* md =
			static_cast<MachineUserData*>(vm->GetUserData());
		rr->Setup(md->first, md->second->GetSessionID(), m_IPHash);
		return rr;
	}
}