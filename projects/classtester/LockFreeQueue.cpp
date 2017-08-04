#include "TestFunctionRegister.h"
#include "BufferUtility.hpp"

#include "concurrentqueue.h"


using namespace hc;
namespace hct
{
	REGISTER(LockFreeQueue);

	//---------------------------------------------------------------------------------------------------------
	void LockFreeQueue()
	{
		bool res = true;
		const int MAX = 100;
		moodycamel::ConcurrentQueue< int > messagequeue;
		for (int i = 0; i < MAX; ++i)
		{
			messagequeue.enqueue(i);
		}
		for (int i = 0; i < MAX; ++i)
		{
			int local = 0;
			messagequeue.try_dequeue(local);
			if (i != local)
			{
				res = false;
				break;
			}
		}
		if (res)
		{
			printf("SUCCESS:moodycamel::ConcurrentQueue\r\n");
		}
		else
		{
			printf("FAIL:moodycamel::ConcurrentQueue\r\n");
		}
	}
}