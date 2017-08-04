#include "thread/IThreadSystem.h"
#include "thread/IThreadMutex.h"
#include "FileconsoleSystem.h"
#include "file/IFileSystem.h"
#include "file/IFile.h"


#include <iostream>
#include <sstream>
#ifdef _PLATFORM_WINDOWS
#include <windows.h>
#endif // _PLATFORM_WINDOWS

using namespace hc;
using namespace std;
namespace hfcl
{
	//---------------------------------------------------------------------------------------------------------
	FileconsoleSystem::LogData::LogData(hc::ILogSystem::LogType t, uint64 time, char* str)
		:eLogType(t)
		, rpText(str)
		, uTime(time)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	FileconsoleSystem::LogData::LogData()
	{

	}




	//---------------------------------------------------------------------------------------------------------
	FileconsoleSystem::FileconsoleSystem()
		:m_pLogFile(NULL)
		, m_Filesize(0)
		, m_Index(0)
		, m_MaxSize(0)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	FileconsoleSystem::~FileconsoleSystem()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void FileconsoleSystem::_DoLog(hc::ILogSystem::LogType lt, double time, char* str)
	{
		//m_pIThreadMutex->Lock();
		//std::cout << str << std::endl;
		//m_pIThreadMutex->Unlock();
		m_LogQueue.enqueue(LogData(lt, time, str));
	}
	//---------------------------------------------------------------------------------------------------------
	void FileconsoleSystem::_OnSetFile(uint size, const eastl::string& file)
	{
		m_MaxSize = size;
		m_Fileext = file.substr(file.find_last_of("."));
		m_Filename = file.substr(0, file.find_last_of("."));
		_CreateFile();
		m_pIThreadMutex = static_cast<IThreadMutex*>(
			IThreadSystem::Instance()->
			ThreadEntityFactory(ThreadProperty::ET_MUTEX));
	}
	//---------------------------------------------------------------------------------------------------------
	void FileconsoleSystem::_CreateFile()
	{
		m_Filesize = 0;
		std::stringstream ss;
		ss << m_Filename.c_str() << "_" << m_Index++ << m_Fileext.c_str();
		if (m_pLogFile)
		{
			m_pLogFile->CloseFile();
			IFileSystem::Instance()->RecycleBin(m_pLogFile);
		}
		m_pLogFile = IFileSystem::Instance()->FileFactory(ss.str().c_str());
		if (m_pLogFile)
		{
			if (!m_pLogFile->OpenFile(IFile::AT_WRITE))
			{
				IFileSystem::Instance()->RecycleBin(m_pLogFile);
				m_pLogFile = NULL;
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void FileconsoleSystem::Initialize()
	{
		ILogSystem::Initialize();
#ifdef _PLATFORM_WINDOWS
		m_hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
#endif
	}
	//---------------------------------------------------------------------------------------------------------
	void FileconsoleSystem::Update()
	{
		LogData data;
		LogDataMap logmap;
		while (m_LogQueue.try_dequeue(data))
		{
			logmap.insert(eastl::make_pair(data.uTime, data));
		}

		//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		//// you can loop k higher to see more color choices
		//for (int k = 1; k < 255; k++)
		//{
		//	// pick the colorattribute k you want
		//	SetConsoleTextAttribute(hConsole, k);
		//	cout << k << " I want to be nice today!" << endl;
		//}

		hc::ILogSystem::LogType last = (hc::ILogSystem::LogType) - 1;
		FOR_EACH(LogDataMap, log, logmap)
		{
#ifdef _PLATFORM_WINDOWS
			if (last != log->second.eLogType)
			{
				last = log->second.eLogType;
				switch (log->second.eLogType)
				{
				case ILogSystem::LT_LOG:
					SetConsoleTextAttribute(m_hConsole, 2);
					break;
				case ILogSystem::LT_WARNING:
					SetConsoleTextAttribute(m_hConsole, 14);
					break;
				case ILogSystem::LT_ERROR:
					SetConsoleTextAttribute(m_hConsole, 12);
					break;
				default:
					SetConsoleTextAttribute(m_hConsole, 7);
					break;
				}
			}
#endif
			std::cout << log->second.rpText << std::endl;
			if (m_pLogFile)
			{
				m_Filesize += m_pLogFile->WriteFile(log->second.rpText, strlen(log->second.rpText));
				m_Filesize += m_pLogFile->WriteFile("\r\n", 2);
			}
			_Recycle(log->second.rpText);
		}
#ifdef _PLATFORM_WINDOWS
		SetConsoleTextAttribute(m_hConsole, 7);
#endif
		if (m_pLogFile)
		{
			m_pLogFile->Flush();
			if (m_Filesize > m_MaxSize)
			{
				_CreateFile();
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void FileconsoleSystem::Exit()
	{
		if (NULL != m_pLogFile)
		{
			m_pLogFile->CloseFile();
			IFileSystem::Instance()->RecycleBin(m_pLogFile);
		}
		if (NULL != m_pIThreadMutex)
		{
			IThreadSystem::Instance()->RecycleBin(m_pIThreadMutex);
		}
		ILogSystem::Exit();
	}
}