#pragma once

#include <EASTL/list.h>
namespace hct
{
	typedef void(*TestFunction)(void);
	class Functionpool
	{
	private:
		typedef eastl::list<TestFunction>	TestFunctionList;
	private:
		static Functionpool		m_Instance;
	private:
		TestFunctionList		m_TestFunctionList;
	private:
		Functionpool();
		~Functionpool();
	public:
		static Functionpool& Instance();
	public:
		void PushFunction(TestFunction f);
		void InvokeFunctions();
	};
}
