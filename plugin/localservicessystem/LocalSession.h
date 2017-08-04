#pragma once
#include "services/ISession.h"


namespace hlss
{
	class LocalSession : public hc::ISession
	{
	public:
		LocalSession();
		~LocalSession();
	private:
		virtual void _OnSessionDone(hc::Protocol* input, hc::IArguments* res);
	};
}