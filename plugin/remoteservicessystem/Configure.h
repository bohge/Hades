#pragma once
#include "network/SpecificDefined.h"



namespace hrss
{
	namespace Configure
	{
		enum RemoteMessage
		{
			RMID_READY_CALL = MC_USER_DEFINED_ID_MAX+1,
			RMID_AGENT_SEND,
			RMID_AGENT_INCR,
			RMID_AGENT_FREE,
			RMID_REMOTE_CALL,
			RMID_REMOTE_CALL_BACK,
		};
		//≈‰÷√±Ìœ‡πÿ
		static const char*	FUNCTION_RECEVICE_AGENT_NAME ("OnReceiveAgent");
		static const char*	FUNCTION_REMOTENODE_READY_NAME("OnRemotenodeReady");
		static const char*	FUNCTION_REMOTENODE_DISCONNECT_NAME("OnRemotenodeDisconnect");
	};
}