#include "core/Predefine.h"
#include "Functionpool.h"



namespace hct
{

	hct::Functionpool Functionpool::m_Instance;
	//---------------------------------------------------------------------------------------------------------
	Functionpool::Functionpool()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	Functionpool::~Functionpool()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	Functionpool& Functionpool::Instance()
	{
		return m_Instance;
	}
	//---------------------------------------------------------------------------------------------------------
	void Functionpool::PushFunction(TestFunction f)
	{
		m_TestFunctionList.push_back(f);
	}
	//---------------------------------------------------------------------------------------------------------
	void Functionpool::InvokeFunctions()
	{
		FOR_EACH(TestFunctionList, func, m_TestFunctionList)
		{
			TestFunction ptr = *func;
			(*ptr)();
		}
	}

}