#pragma once
#include "core/Singleton.hpp"
#include "core/IPlugin.h"




#include <EASTL/string.h>

namespace hc
{
	class HADES_CORE_API ILogSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(ILogSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(ILogSystem, IPlugin)
	public:
		enum LogType
		{
			LT_LOG = 0,
			LT_WARNING,
			LT_ERROR,
		};
		enum Constant
		{
			LC_HEAD_SIZE = 100,//Í·³¤¶È
		};
	private:
		void*			m_pBufferQueue;
	public:
		ILogSystem(void);
		virtual ~ILogSystem(void);
	private:
		virtual void _DoLog(LogType lt, double time, char* str) = 0;
		virtual void _OnSetFile(uint size, const eastl::string& file) = 0;
	public:
		virtual void Initialize();
		virtual void Exit();
	private:
		char* _Sprintf(double& time, const char* str, uint len, va_list args);
	protected:
		void _Recycle(char* buffer);
	public:
		void Log(const char* str, ...);
		void Warning(const char* str, ...);
		void Error(const char* str, ...);
		void ScriptLog(const char* str, uint len);
		void ScriptWarning(const char* str, uint len);
		void ScriptError(const char* str, uint len);
	public:
		HADES_FORCEINLINE void SetLogFile(uint size, const eastl::string& name);
	};

	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void ILogSystem::SetLogFile(uint size, const eastl::string& name)
	{
		_OnSetFile(size, name);
	}
}

#define HADESLOG(x, ...)		hc::ILogSystem::Instance()->Log(x, ##__VA_ARGS__)
#define HADESWARNING(x, ...)	hc::ILogSystem::Instance()->Warning(x, ##__VA_ARGS__)
#define HADESERROR(x, ...)	hc::ILogSystem::Instance()->Error(x, ##__VA_ARGS__)