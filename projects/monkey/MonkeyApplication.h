#pragma once
#include "application/IApplication.h"
#include "core/SmartPtr.hpp"
#include <EASTL/string.h>


namespace hc
{
	class IJobDispatcher;
}
namespace hmka
{
	class MonkeyHandle;
	class MonkeyApplication : public hc::IApplication
	{
	private:
		typedef eastl::vector< hc::SmartPtr<MonkeyHandle> > MonkeyArray;
	private:
		MonkeyArray					m_MonkeyArray;
		hc::IJobDispatcher*			m_pMonkeyThread;
	public:
		MonkeyApplication(void);
		virtual ~MonkeyApplication(void);
	private:
		virtual eastl::string _Configurepath();
		virtual void _OnInitialize( );
		virtual bool _OnDowork();
		virtual void _OnExit();
	public:
		void OnTimerCallback(uint64 gid);
	};

}

