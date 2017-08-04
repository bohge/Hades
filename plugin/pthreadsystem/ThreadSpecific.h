#pragma once
#include "thread/IThreadSpecific.h"

#include <pthread.h>

namespace hpts
{
	class ThreadSpecific : public hc::IThreadSpecific
	{
	private:
		pthread_key_t*		m_pKey;
	public:
		ThreadSpecific(void);
		virtual ~ThreadSpecific(void);
	private:
		virtual void* _GetSpecific();
	public:
		//virtual void Initialize( );
		void Initialize();
		virtual void SetSpecific( const void* data );
	};
}