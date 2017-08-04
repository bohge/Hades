#include "serialize/FactorConfigure.h"
#include "script/IScriptSystem.h"
#include "services//IResumer.h"




namespace hc
{
	RTTI_TYPE_IMPLEMENT_WITH_FACTOR(IResumer, FACTOR_RESUMER);
	//---------------------------------------------------------------------------------------------------------
	IResumer::IResumer(void)
		:m_isPasue(false)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IResumer::~IResumer(void)
	{
	}
}
