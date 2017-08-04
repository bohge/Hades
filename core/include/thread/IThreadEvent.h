#pragma once
#include "IThreadEntity.h"



namespace hc
{
	class HADES_CORE_API IThreadEvent : public IThreadEntity
	{
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IThreadEvent, IThreadEntity)
	public:
		IThreadEvent():IThreadEntity( ThreadProperty::ET_EVENT ){}
		virtual ~IThreadEvent(void){};
	public:
		virtual void Lock() = 0;
		virtual void Unlock() = 0;
		virtual void Signal() = 0;
		virtual void Wait() = 0;
		virtual void Wait(uint us) = 0;
	};
}
