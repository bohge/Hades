#include "network/message/IMessage.h"
#include "services/Protocol.h"
#include "core/Bufferpool.h"
#include "RemoteServices.h"
#include "Configure.h"
#include "Utility.h"


using namespace hc;
namespace hrss
{
	//---------------------------------------------------------------------------------------------------------
	void SerializeRemoteCall(RemoteServices* service, hc::Protocol* ptc, hc::IMessage* msg)
	{
		uint realsize = 0;
		uint64 hostgid = service->GetHostGID();
		uint rawsize = ptc->GetUsingSize() + sizeof(hostgid);
		byte* buffer = msg->PrepareMessage(Configure::RMID_REMOTE_CALL, rawsize);
		uint len = ptc->Serialize(buffer);
		memcpy(buffer + len, &hostgid, sizeof(hostgid));
		ASSERT(len == rawsize - sizeof(hostgid));
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 ParseRemoteCall(hc::Protocol* ptc, hc::IMessage* msg)
	{
		const byte* buff = msg->GetMessageBuffer();
		uint alllen = msg->GetMessageLength();
		uint calllen = alllen - sizeof(uint64);
		ptc->Parse(buff, calllen);
		uint64 res = hc::nullid;
		memcpy(&res, buff + calllen, sizeof(res));
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void SerializeFree(uint64 gid, hc::IMessage* msg)
	{
		byte* buf = msg->PrepareMessage(Configure::RMID_AGENT_FREE, sizeof(gid));
		memcpy(buf, (byte*)&gid, sizeof(gid));
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 ParseFree(hc::IMessage* msg)
	{
		return *(uint64*)msg->GetMessageBuffer();
	}
	//---------------------------------------------------------------------------------------------------------
	void SerializeIncrement(uint64 gid, hc::IMessage* msg)
	{
		byte* buf = msg->PrepareMessage(Configure::RMID_AGENT_INCR, sizeof(gid));
		memcpy(buf, (byte*)&gid, sizeof(gid));
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 ParseIncrement(hc::IMessage* msg)
	{
		return *(uint64*)msg->GetMessageBuffer();
	}
}