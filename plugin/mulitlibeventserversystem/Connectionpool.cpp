#include "LibeventConnection.h"
#include "Connectionpool.h"



namespace hles
{
	SINGLETON_IMPLEMENT(Connectionpool)
	//---------------------------------------------------------------------------------------------------------
	Connectionpool::Connectionpool()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	Connectionpool::~Connectionpool()
	{
		LibeventConnection* res = NULL;
		if (!m_LibeventConnectionQueue.try_dequeue(res))
		{
			SAFE_DELETE(res);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	LibeventConnection* Connectionpool::GetLibeventConnection()
	{
		LibeventConnection* res = NULL;
		if (!m_LibeventConnectionQueue.try_dequeue(res))
		{
			res = NEW LibeventConnection;
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void Connectionpool::FreeLibeventConnection(LibeventConnection* obj)
	{
		ASSERT(NULL != obj);
		m_LibeventConnectionQueue.enqueue(obj);
	}
}
