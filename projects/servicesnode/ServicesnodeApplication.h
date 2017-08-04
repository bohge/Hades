#pragma once
#include "application/IApplication.h"
#include "core/SmartPtr.hpp"

#include <EASTL/string.h>

namespace hc
{
	class IAgent;
	class IJobDispatcher;
}
namespace hsna
{
	class Commandline;
	class ServicesnodeApplication : public hc::IApplication
	{
	private:
		enum Constant
		{
			SNAC_MESSAGE_CACHE_SIZE = 1000,
		};
	private:
		eastl::string				m_ConfigureName;
		hc::IAgent*					m_pIAgent;
		hc::IJobDispatcher*			m_pCommandlineThread;
		hc::SmartPtr<Commandline>	m_spCommandlineJob;
		uint						m_FuncInitIndex;
		uint						m_FuncCommandlineIndex;
	public:
		ServicesnodeApplication(void);
		virtual ~ServicesnodeApplication(void);
	private:
		virtual eastl::string _Configurepath();
		virtual void _OnInitialize(const eastl::string& args);
		virtual bool _OnDowork();
		virtual void _OnExit();
	private:
		void _InitializeCommandline();
	public:
		void SetExecutableName(const eastl::string& exename);
	};
}

