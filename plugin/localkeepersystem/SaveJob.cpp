#include "keeper/Accessor.h"
#include "Accessorpool.h"
#include "SaveJob.h"


using namespace hc;
namespace hlks
{
	//---------------------------------------------------------------------------------------------------------
	SaveJob::SaveJob()
		:m_rpSource(NULL)
		, m_rpDestination(NULL)
		, m_pUserCallback(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	SaveJob::~SaveJob()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void SaveJob::_DoYieldJob()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void SaveJob::_DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata)
	{
		hc::Accessor::AccessMode am = m_rpSource->GetAccessorMode();
		switch (am)
		{
		case hc::Accessor::AM_UNRIGOROUS:
		case hc::Accessor::AM_READ_ONLY:
		{
			break;
		}
		case hc::Accessor::AM_READ_WRITE:
		{
			m_rpDestination->SetupAccessor(*m_rpSource, true);
			break;
		}
		default:ASSERT(false); break;
		}
		Accessorpool::Instance()->Free(m_rpSource);
		m_pUserCallback->Invoker(am);
		m_pSaveJobCallback->Invoker(self);
	}

}