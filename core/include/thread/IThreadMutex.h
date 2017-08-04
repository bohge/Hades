#pragma once
#include "IThreadEntity.h"



namespace hc
{
	class HADES_CORE_API IThreadMutex : public IThreadEntity
	{
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IThreadMutex, IThreadEntity)
	public:
		IThreadMutex():IThreadEntity( ThreadProperty::ET_MUTEX ){}
		virtual ~IThreadMutex(void){};
	public:
		virtual void Lock() = 0;//¼ÓËø
		virtual void Unlock() = 0;//½âËø
	};
}
