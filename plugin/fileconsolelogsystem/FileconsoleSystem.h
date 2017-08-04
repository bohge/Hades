#pragma once
#include "log/ILogSystem.h"

#include "concurrentqueue.h"

#include <map>

namespace hc
{
	class IFile;
	class IThreadMutex;
}
namespace hfcl
{
	class FileconsoleSystem : public hc::ILogSystem
	{
	private:
		struct LogData
		{
			double						uTime;
			hc::ILogSystem::LogType		eLogType;
			char*						rpText;
			LogData();
			LogData(hc::ILogSystem::LogType t, uint64 time, char* str);
		};
	private:
		typedef moodycamel::ConcurrentQueue<LogData> LogQueue;
		typedef eastl::multimap<double, LogData> LogDataMap;
	private:
		eastl::string		m_Filename;
		eastl::string		m_Fileext;
		LogQueue			m_LogQueue;
		hc::IFile*			m_pLogFile;
		hc::IThreadMutex*	m_pIThreadMutex;
		uint				m_MaxSize;
		uint				m_Filesize;
		uint				m_Index;
#ifdef _PLATFORM_WINDOWS
		HANDLE			m_hConsole;
#endif
	public:
		FileconsoleSystem();
		virtual ~FileconsoleSystem();
	private:
		virtual void _DoLog(hc::ILogSystem::LogType lt, double time, char* str);
		virtual void _OnSetFile(uint size, const eastl::string& file);
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	private:
		void _CreateFile();
	};
}
