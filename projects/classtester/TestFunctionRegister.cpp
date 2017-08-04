#include "TestFunctionRegister.h"



namespace hct
{
	//---------------------------------------------------------------------------------------------------------
	TestFunctionRegister::TestFunctionRegister(TestFunction func)
	{
		Functionpool::Instance().PushFunction(func);
	}
}
