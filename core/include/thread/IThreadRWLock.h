#pragma once
#include "IThreadEntity.h"



namespace hc
{
	class HADES_CORE_API IThreadRWLock : public IThreadEntity
	{//读写锁
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IThreadRWLock, IThreadEntity)
	public:
		IThreadRWLock():IThreadEntity( ThreadProperty::ET_RWLOCK ){}
		virtual ~IThreadRWLock(void){};
	public:
		virtual void ReadLock() = 0;//共享方式的读取锁
		virtual void WriteLock() = 0;//独占的写入锁
		virtual void Unlock() = 0;//解锁
	};
}