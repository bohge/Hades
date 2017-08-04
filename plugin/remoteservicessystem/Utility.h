#pragma once



namespace hc
{
	class IMessage;
	class Protocol;
}
namespace hrss
{
	class AdaptiveAgent;


	void SerializeRemoteCall(RemoteServices* service, hc::Protocol* ptc, hc::IMessage* msg);
	uint64 ParseRemoteCall(hc::Protocol* ptc, hc::IMessage* msg);

	void SerializeFree(uint64 gid, hc::IMessage* msg);
	uint64 ParseFree(hc::IMessage* msg);

	void SerializeIncrement(uint64 gid, hc::IMessage* msg);
	uint64 ParseIncrement(hc::IMessage* msg);
}
