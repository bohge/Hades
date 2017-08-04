#pragma once
#include "thread/IThreadMutex.h"


namespace hpts
{
	class ThreadMutex : public hc::IThreadMutex
	{
	private:
		void*		m_pMutex;
	public:
		ThreadMutex(void);
		virtual ~ThreadMutex(void);
	public:
		virtual void Lock();//¼ÓËø
		virtual void Unlock();//½âËø
	};
}
