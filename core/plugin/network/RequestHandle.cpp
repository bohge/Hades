#include "services/IServicesSystem.h"
#include "network/RequestHandle.h"


namespace hc
{
	//---------------------------------------------------------------------------------------------------------
	RequestHandle::RequestHandle(SmartPtr<IServices>& services, uint connect, uint requestfunc, uint disconnectfunc)
		:pAgent(NULL)
	{
		pAgent = services->AllocAgent();
		Functions[CONNECTINDX] = connect;
		Functions[REQUESTINDX] = requestfunc;
		Functions[DISCONNECTINDX] = disconnectfunc;
	}
	//---------------------------------------------------------------------------------------------------------
	RequestHandle::~RequestHandle()
	{
		hc::IServicesSystem::Instance()->RecycleBin(pAgent);
	}
}