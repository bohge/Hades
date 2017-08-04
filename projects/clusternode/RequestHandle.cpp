#include "services/IServicesSystem.h"
#include "RequestHandle.h"



namespace hsna
{
	//---------------------------------------------------------------------------------------------------------
	RequestHandle::RequestHandle()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	RequestHandle::~RequestHandle()
	{
		hc::IServicesSystem::Instance()->RecycleBin(pAgent);
	}
}