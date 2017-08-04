#pragma once
#include "IThreadEntity.h"



namespace hc
{
	class HADES_CORE_API IThreadSpecific : public IThreadEntity
	{
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IThreadSpecific, IThreadEntity)
	public:
		IThreadSpecific(void):IThreadEntity( ThreadProperty::ET_THREAD_SPECIFIC ){}
		virtual ~IThreadSpecific(void){}
	private:
		virtual void* _GetSpecific() = 0;
	public:
		//virtual void Initialize( ) = 0;
		virtual void SetSpecific( const void* data ) = 0;
	public:
		template<typename T>T* GetSpecific()
		{
			return static_cast<T*>(_GetSpecific());
		}
	};
}