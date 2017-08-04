#pragma once
#include "core/Singleton.hpp"
#include "core/SmartPtr.hpp"
#include "core/Object.hpp"

#include <EASTL/map.h>
namespace hc
{
	class IAgent;
	class IThreadRWLock;
	struct RequestHandle;
	class RequestPool
	{
		SINGLETON_DECLARE(RequestPool);
	private:
		struct ServiceFunctionIndex
		{
			uint	connect;
			uint	request;
			uint	disconnect;
			ServiceFunctionIndex(uint c, uint r, uint d);
		};
	private:
		typedef eastl::map<uint, ServiceFunctionIndex* > RequestHandleMap;
	private:
		RequestHandleMap		m_RequestHandleMap;
		IThreadRWLock*			m_pIThreadRWLock;
	public:
		RequestPool();
		~RequestPool();
	public:
		SmartPtr<RequestHandle> CreateRequestHandle(IAgent* agent);
	};
}