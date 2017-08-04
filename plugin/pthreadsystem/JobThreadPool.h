//#pragma once
//#include "core/Singleton.hpp"
//#include "core/Object.hpp"
//
//
//#include <EASTL/list.h>
//namespace hpts
//{
//	class JobThread;
//	class JobThreadPool
//	{
//		SINGLETON_DECLARE_ONCREATE_ONDESTROY( JobThreadPool );
//	private:
//		typedef eastl::list< JobThread* > ThreadList;
//	private:
//		ThreadList			m_ThreadList;
//	public:
//		JobThreadPool(void);
//		~JobThreadPool(void);
//	private:
//		void _OnCreate();
//		void _OnDestroy();
//	public:
//		JobThread* GetJobThread();
//		void ReleaseJobThread( JobThread* t );
//	};
//}
//
