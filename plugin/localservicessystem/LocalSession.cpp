#include "services/IServicesSystem.h"
#include "services/IServices.h"
#include "script/IArguments.h"
#include "Serviceshelper.hpp"
#include "LocalSession.h"



using namespace hc;
namespace hlss
{
	//---------------------------------------------------------------------------------------------------------
	LocalSession::LocalSession()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LocalSession::~LocalSession()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalSession::_OnSessionDone(hc::Protocol* input, hc::IArguments* res)
	{
		HandleCallback(input->GetCallerGID(), input->GetCallerSession(), res);
	}
}