#pragma once
#include "core/Singleton.hpp"
#include "core/IPlugin.h"

#include "concurrentqueue.h"

#include <EASTL/string.h>

namespace hc
{
	class ISerializer;
	class ISerializable;
	class HADES_CORE_API ISerializeSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(ISerializeSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(ISerializeSystem, IPlugin);
	private:
		typedef moodycamel::ConcurrentQueue< ISerializable* > BufferQueue;
		typedef eastl::vector< eastl::pair<ISerializable*, BufferQueue*>* > PiarVector;
	private:
		PiarVector		m_PiarVector;
	public:
		ISerializeSystem(void);
		virtual ~ISerializeSystem(void);
	public:
		static void SerializableCleaner(void* rec, Object* obj);//回收除器，必须是线程安全的
	public:
		virtual ISerializer* SerializerFactory() = 0;
		virtual void RecycleBin(ISerializer* ser) = 0;
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	public:
		void PrepareSerializable(uint factor, uint count);
		ISerializable* GetSerializable(uint factor);
		void FreeSerializable(ISerializable* obj);
	public:
		template<typename T> void RegisterSerializable();
	};


	//---------------------------------------------------------------------------------------------------------
	template<typename T>
	void hc::ISerializeSystem::RegisterSerializable()
	{
		T* root = NEW T();
		uint index = root->GetFactor();
		if (NULL != m_PiarVector[index])
		{
			SAFE_DELETE(root);
		}
		else
		{
			BufferQueue* queue = NEW BufferQueue();
			m_PiarVector[index] = NEW eastl::pair<ISerializable*, BufferQueue*>(root, queue);
		}
	}

}
