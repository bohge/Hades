#pragma once
#include "thread/ThreadProperty.h"
#include "core/Predefine.h"

#include <pthread.h>

namespace hpts
{
	class ThreadMutex;
	class Threading//线程基础类
	{
	private:
		eastl::string						m_Name;
		ThreadMutex*						m_pMutex;
		pthread_attr_t*						m_pThreadAttr;
		pthread_t*							m_pThreadID;
		hc::ThreadProperty::ThreadPriority	m_ePriority;
		bool								m_isRunning;
	public:
		Threading(void);
		virtual ~Threading(void);
	public:
		static void* ThreadProc( void* param );
	private:
		virtual void _OnStart() = 0;//线程启动时的事件
		virtual void _OnReset() = 0;//线程重新初始化线程，准备复用事件
		virtual bool _StopWork() = 0;//停止线程工作，线程准备退出,返回是否成功退出，如果失败将强制退出
	public:
		virtual void* DoWork() = 0;//工作线程
	public:
		void Reset();//重新初始化线程，准备复用
		void Start();//开启线程
		void Stop();//关闭线程
		void SetPriority( hc::ThreadProperty::ThreadPriority tp );
		void SetName(const eastl::string& name);
	public:
		HADES_FORCEINLINE bool isRunning() const;
		HADES_FORCEINLINE hc::ThreadProperty::ThreadPriority GetPriority() const;
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool Threading::isRunning() const
	{
		return m_isRunning;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::ThreadProperty::ThreadPriority Threading::GetPriority() const
	{
		return m_ePriority;
	}
}

