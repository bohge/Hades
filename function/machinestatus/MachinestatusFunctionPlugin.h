#pragma once
#include "core/IFunction.h"
#include "core/Singleton.hpp"
#include "core/VariableType.h"

namespace hmsfp
{
	class MachinestatusFunctionPlugin : public hc::IFunction
	{
		RTTI_DRIVER_TYPE_DECLEAR(MachinestatusFunctionPlugin, hc::IFunction);
		SYSTEM_SINGLETON_DECLEAR(MachinestatusFunctionPlugin);

	public:
		double lastcputime;
		int64 preidleTime;
		int64 prekernelTime;
		int64 preuserTime;

		double lastnetcardrectime;
		int lasttotalrec;
		double lastnetcardsendtime;
		int lasttotalsend;

		int GetFreePhysicalmemory();
		int GetTotalPhysicalmemory();
		int GetFreeVirtualmemory();
		int GetTotalVirtualmemory();

		float GetCpuBusypercentage();
		float GetNetcardReceiveSpeed();
		float GetNetcardSendSpeed();

	public:
		MachinestatusFunctionPlugin();
		~MachinestatusFunctionPlugin();
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	};

}