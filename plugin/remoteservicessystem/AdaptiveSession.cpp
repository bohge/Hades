#include "services/IServicesSystem.h"
#include "RemoteServicesSystem.h"
#include "services/IServices.h"
#include "script/IArguments.h"
#include "Serviceshelper.hpp"
#include "AdaptiveSession.h"


using namespace hc;
namespace hrss
{
	//---------------------------------------------------------------------------------------------------------
	AdaptiveSession::AdaptiveSession()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	AdaptiveSession::~AdaptiveSession()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void AdaptiveSession::_OnSessionDone(hc::Protocol* input, hc::IArguments* res)
	{
		HandleCallback(input->GetCallerGID(),
			input->GetCallerSession(),
			input->GetCallerHash(),
			res);
	}
}