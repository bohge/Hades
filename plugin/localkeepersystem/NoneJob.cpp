#include "keeper/Accessor.h"
#include "Accessorpool.h"
#include "NoneJob.h"
#include "log/ILogSystem.h"

using namespace hc;
namespace hlks
{
	//---------------------------------------------------------------------------------------------------------
	NoneJob::NoneJob()
		:m_pUserCallback(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	NoneJob::~NoneJob()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void NoneJob::_DoYieldJob()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void NoneJob::_DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata)
	{
		m_pUserCallback->Invoker(NULL);
	}

}