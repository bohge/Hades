#pragma once
#include "application/IApplication.h"
#include "core/SmartPtr.hpp"



namespace hct
{
	class TesterApplication : public hc::IApplication
	{
	private:
	public:
		TesterApplication(void);
		virtual ~TesterApplication(void);
	private:
		virtual eastl::string _Configurepath();
		virtual void _OnInitialize(const eastl::string& args);
		virtual bool _OnDowork();
		virtual void _OnExit();
	};

}

