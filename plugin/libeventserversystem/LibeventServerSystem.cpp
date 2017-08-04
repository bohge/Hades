#include "memory/IMemorySystem.h"
#include "LibeventServerSystem.h"
#include "LibeventServer.h"

#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <event2/listener.h>
#include <event2/util.h>
#include <event2/event.h>
#include <event2/thread.h>

using namespace hc;

namespace hles
{
	//---------------------------------------------------------------------------------------------------------
	LibeventServerSystem::LibeventServerSystem(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	LibeventServerSystem::~LibeventServerSystem(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IServer* LibeventServerSystem::_DoServerFactory(IServerSystem::ServerType st) const
	{
		LibeventServer* ls = NEW LibeventServer();
		return ls;
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServerSystem::_DoRecycleBin( hc::IServer* is ) const
	{
		SAFE_DELETE(is);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServerSystem::Initialize()
	{
		IServerSystem::Initialize();
		evthread_use_depend_threads();
		event_set_mem_functions(
			&IMemorySystem::AllocMemory,
			&IMemorySystem::ReallocMemory,
			&IMemorySystem::ReleaseMemory
			);
#ifdef WIN32
		WSADATA wsa_data;
		WSAStartup(0x0201, &wsa_data);
#endif
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServerSystem::Update()
	{

	}
}