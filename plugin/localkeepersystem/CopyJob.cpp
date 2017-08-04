#include "keeper/Accessor.h"
#include "Accessorpool.h"
#include "CopyJob.h"
#include "log/ILogSystem.h"

using namespace hc;
namespace hlks
{
	//---------------------------------------------------------------------------------------------------------
	CopyJob::CopyJob()
		:m_rpSource(NULL)
		, m_pUserCallback(NULL)
		, m_eAccessMode(hc::Accessor::AM_CREATE_ACCESS)
		, m_isCopy(false)
		, m_Timestamp(0)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	CopyJob::~CopyJob()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void CopyJob::_DoYieldJob()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void CopyJob::_DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata)
	{
		Accessor* des = Accessorpool::Instance()->Get();
		des->Reset();
		des->SetupAccessor(*m_rpSource, m_isCopy);
		des->SetTimestamp(m_Timestamp);
		des->SetTimestampValid(!m_isCopy);
		des->SetAccessorMode(m_eAccessMode);
		m_pUserCallback->Invoker(des);
		m_pCopyJobCallback->Invoker(self);
	}
}