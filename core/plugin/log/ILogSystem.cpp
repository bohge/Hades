#include "timer/ITimerSystem.h"
#include "log/ILogSystem.h"
#include "core/Configure.h"


#include "concurrentqueue.h"

typedef moodycamel::ConcurrentQueue<char*> BufferQueue;
#define BUFFERQUEUE (static_cast<BufferQueue*>(m_pBufferQueue))
namespace hc
{
	RTTI_TYPE_IMPLEMENT(ILogSystem);
	SYSTEM_SINGLETON_IMPLEMENT(ILogSystem);
	//---------------------------------------------------------------------------------------------------------
	ILogSystem::ILogSystem(void)
		: m_pBufferQueue(NULL)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	ILogSystem::~ILogSystem(void)
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//---------------------------------------------------------------------------------------------------------
	void ILogSystem::Initialize()
	{
		m_pBufferQueue = NEW BufferQueue;
	}
	//---------------------------------------------------------------------------------------------------------
	void ILogSystem::Exit()
	{
		char* buffer;
		while (BUFFERQUEUE->try_dequeue(buffer))
		{
			SAFE_DELETE_ARRAY(buffer);
		}
		BufferQueue* p = BUFFERQUEUE;
		m_pBufferQueue = NULL;
		SAFE_DELETE(p);
	}
	//---------------------------------------------------------------------------------------------------------
	char* ILogSystem::_Sprintf(double& time, const char* str, uint len, va_list args)
	{
		char* buffer;
		if (len > (MAX_BUFFER_SIZE - LC_HEAD_SIZE))
		{
			uint toalloc = len + LC_HEAD_SIZE;
			buffer = NEW char[toalloc];
		}
		else
		{
			if (!BUFFERQUEUE->try_dequeue(buffer))
			{
				buffer = NEW char[MAX_BUFFER_SIZE];
			}
		}
		if (ITimerSystem::Instance())
		{
			time = ITimerSystem::Instance()->GetTimevalue();
		}
		else
		{
			time = 0.0;
		}
		int index = sprintf(buffer, "%10.5f\t", time);
		if (NULL == args)
		{
			memcpy(buffer + index, str, len);
			buffer[index + len] = 0;
		}
		else
		{
			vsprintf(buffer + index, str, args);
		}
		return buffer;
	}
	//---------------------------------------------------------------------------------------------------------
	void ILogSystem::_Recycle(char* buffer)
	{
		BUFFERQUEUE->enqueue(buffer);
	}
	//---------------------------------------------------------------------------------------------------------
	void ILogSystem::Log(const char* str, ...)
	{
		double time;
		va_list args;
		va_start(args, str);
		char* buffer = _Sprintf(time, str, strlen(str), args);
		va_end(args);
		_DoLog(LT_LOG, time, buffer);
	}
	//---------------------------------------------------------------------------------------------------------
	void ILogSystem::Warning(const char* str, ...)
	{
		double time;
		va_list args;
		va_start(args, str);
		char* buffer = _Sprintf(time, str, strlen(str), args);
		va_end(args);
		_DoLog(LT_WARNING, time, buffer);
	}
	//---------------------------------------------------------------------------------------------------------
	void ILogSystem::Error(const char* str, ...)
	{
		double time;
		va_list args;
		va_start(args, str);
		char* buffer = _Sprintf(time, str, strlen(str), args);
		va_end(args);
		_DoLog(LT_ERROR, time, buffer);
	}
	//---------------------------------------------------------------------------------------------------------
	void ILogSystem::ScriptLog(const char* str, uint len)
	{
		double time;
		char* buffer = _Sprintf(time, str, len, NULL);
		_DoLog(LT_LOG, time, buffer);
	}
	//---------------------------------------------------------------------------------------------------------
	void ILogSystem::ScriptWarning(const char* str, uint len)
	{
		double time;
		char* buffer = _Sprintf(time, str, len, NULL);
		_DoLog(LT_WARNING, time, buffer);
	}
	//---------------------------------------------------------------------------------------------------------
	void ILogSystem::ScriptError(const char* str, uint len)
	{
		double time;
		char* buffer = _Sprintf(time, str, len, NULL);
		_DoLog(LT_ERROR, time, buffer);
	}
}