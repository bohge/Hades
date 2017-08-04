#pragma once
#include "thread/IThreadRWLock.h"



namespace hpts
{
	class ThreadRWLock : public hc::IThreadRWLock
	{
	private:
		void*	m_pRWLock;
	public:
		ThreadRWLock(void);
		virtual ~ThreadRWLock(void);
	public:
		virtual void ReadLock();//共享方式的读取锁
		virtual void WriteLock();//独占的写入锁
		virtual void Unlock();//解锁
	};
}
