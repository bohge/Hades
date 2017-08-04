#pragma once
#include "core/Singleton.hpp"
#include "core/Delegate.hpp"
#include "core/IPlugin.h"



namespace hc
{
	class IRawbuffer;
	class HADES_CORE_API IMessageSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(IMessageSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IMessageSystem, IPlugin);
	public:
		IMessageSystem();
		virtual ~IMessageSystem();
	private:
		virtual IRawbuffer* _DoRawbufferFactory() const = 0;
		virtual void _DoRecycleBin(IRawbuffer* rb) const = 0;
	public:
		HADES_FORCEINLINE IRawbuffer* RawbufferFactory();
		HADES_FORCEINLINE void RecycleBin(IRawbuffer* rb);
	};

	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE IRawbuffer* IMessageSystem::RawbufferFactory()
	{
		return _DoRawbufferFactory();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IMessageSystem::RecycleBin(IRawbuffer* rb)
	{
		_DoRecycleBin(rb);
	}
}