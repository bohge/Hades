#include "Accessorpool.h"



using namespace hc;
namespace hlks
{
	SINGLETON_IMPLEMENT(Accessorpool);
	//---------------------------------------------------------------------------------------------------------
	Accessorpool::Accessorpool()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	Accessorpool::~Accessorpool()
	{
		Accessor* acc;
		while (m_AccessorQueue.try_dequeue(acc))
		{
			SAFE_DELETE(acc);
		}
	}
}