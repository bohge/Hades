#pragma once
#include "services/ISession.h"


namespace hrss
{
	class AdaptiveSession : public hc::ISession
	{
	public:
		AdaptiveSession();
		~AdaptiveSession();
	private:
		virtual void _OnSessionDone(hc::Protocol* input, hc::IArguments* res);
	};
}