#include "script/IFunctionInvoker.h"


using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT( IFunctionInvoker );
	//-------------------------------------------------------------------------------------------------------
	IFunctionInvoker::IFunctionInvoker( void )
		:m_ID(nullhandle)
	{

	}
	//-------------------------------------------------------------------------------------------------------
	IFunctionInvoker::~IFunctionInvoker( void )
	{

	}
}