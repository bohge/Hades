#include "script/IFunctionInvoker.h"
#include "SinglenodeApplication.h"
#include "BalanceUserData.h"
#include "BalanceHandle.h"



using namespace hc;
namespace hsna
{
	RTTI_TYPE_IMPLEMENT(BalanceHandle);
	RTTI_TYPE_IMPLEMENT(BalanceUserData);
	//---------------------------------------------------------------------------------------------------------
	BalanceHandle::BalanceHandle()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	BalanceHandle::~BalanceHandle()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void BalanceHandle::_DoYieldJob()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void BalanceHandle::_DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata)
	{
		BalanceUserData* data = static_cast<BalanceUserData*>(userdata);
		uint* res = (uint*)data->pFunction->Invoke(&m_ID, &m_CallbackType, m_rpMessage);
		data->rpApplication->DispatchRequest(*res, m_ID, m_CallbackType, m_rpMessage);
		SmartPtr<BalanceHandle> handle = SmartPtr_Static_Cast<BalanceHandle>(self);
		data->rpApplication->RecycleBalanceHandle(handle);
	}

}