#pragma once
#include "thread/IJob.h"


namespace hc{ class IAgent; }
namespace hsna
{
	class Commandline : public hc::IJob
	{
	private:
		hc::IAgent*		m_pIAgent;
		uint			m_FuncCommandlineIndex;
		bool			m_isRunning;
	public:
		Commandline();
		~Commandline();
	private:
		virtual void _DoYieldJob();
		virtual void _DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* data);
	public:
		void Initialize(hc::IAgent* agent, uint index);
	};
}
