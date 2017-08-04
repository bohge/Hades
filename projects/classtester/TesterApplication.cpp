#include "TesterApplication.h"
#include "Functionpool.h"
#include "Configure.h"




namespace hct
{
	//---------------------------------------------------------------------------------------------------------
	TesterApplication::TesterApplication(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	TesterApplication::~TesterApplication(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	eastl::string TesterApplication::_Configurepath()
	{
		return Configure::CONFIG_FILE_PATH;
	}
	//---------------------------------------------------------------------------------------------------------
	void TesterApplication::_OnInitialize(const eastl::string& args)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	bool TesterApplication::_OnDowork()
	{
		Functionpool::Instance().InvokeFunctions();
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	void TesterApplication::_OnExit()
	{
	}
}