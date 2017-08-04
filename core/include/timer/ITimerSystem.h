#pragma once
#include "core/Singleton.hpp"
#include "core/Delegate.hpp"
#include "core/IPlugin.h"



namespace hc
{
	class IAgent;
	class Timevalue;
	class IThreadMutex;
	typedef Unidelegate1<void, uint64> TimerCallback;
	class HADES_CORE_API ITimerSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(ITimerSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(ITimerSystem, IPlugin);
	private:
		typedef eastl::pair<uint64, uint> ServiceFunctionPair;
		typedef eastl::list<ServiceFunctionPair> ServiceFunctionList;
		typedef eastl::map<uint, ServiceFunctionList*> UpdateMap;
	private:
		UpdateMap				m_UpdateMap;
		IThreadMutex*			m_pTimerMutex;
	public:
		ITimerSystem();
		virtual ~ITimerSystem();	
	public:
		virtual CallbackHandle RegisterTimer(float sec, uint64 userdata, TimerCallback::Callback func) = 0;
		virtual void UnregisterTimer(CallbackHandle h) = 0;
		virtual double GetTimevalue() = 0;
	public:
		virtual void Initialize();
		virtual void Exit();
	public:
		void OnTimerCallback(uint64 gid);
	public:
		void RegisterTimer(hc::IAgent* agent, float sec, const eastl::string& function);
	};
}