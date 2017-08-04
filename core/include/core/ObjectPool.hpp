#pragma once
#include "concurrentqueue.h"
#include "Object.hpp"


namespace hc
{
	template <typename ObjectType>
	class ObjectPool
	{
	private:
		typedef moodycamel::ConcurrentQueue< ObjectType* > ObjectlQueue;
	private:
		ObjectlQueue		m_ObjectlQueue;
	public:
		ObjectPool(){}
		~ObjectPool(){}
	public:
		HADES_FORCEINLINE ObjectType* AllocObjcet();
		HADES_FORCEINLINE void FreeObject(ObjectType* obj);
	};

	//------------------------------------------------------------------------------------------------------
	template <typename ObjectType>
	HADES_FORCEINLINE void hc::ObjectPool<ObjectType>::FreeObject(ObjectType* obj)
	{
		if (NULL != obj)
		{
			m_ObjectlQueue.enqueue(obj);
		}
	}
	//------------------------------------------------------------------------------------------------------
	template <typename ObjectType>
	HADES_FORCEINLINE ObjectType* hc::ObjectPool<ObjectType>::AllocObjcet()
	{
		ObjectType* sp;
		if (!m_ObjectlQueue.try_dequeue(sp))
		{
			sp = NEW ObjectType;
		}
		return sp;
	}
}