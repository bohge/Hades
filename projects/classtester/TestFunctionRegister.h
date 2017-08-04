#pragma once
#include "Functionpool.h"



namespace hct
{
	class TestFunctionRegister
	{
	public:
		TestFunctionRegister(TestFunction func);
	};
}

#define REGISTER(func) void func(); hct::TestFunctionRegister s_##func(&func);
