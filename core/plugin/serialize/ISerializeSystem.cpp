#include "serialize/ISerializeSystem.h"
#include "serialize/ISerializable.h"

using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT(ISerializeSystem);
	SYSTEM_SINGLETON_IMPLEMENT(ISerializeSystem);
	//---------------------------------------------------------------------------------------------------------
	void ISerializeSystem::SerializableCleaner(void* rec, Object* obj)
	{
		ISerializeSystem::Instance()->FreeSerializable(static_cast<ISerializable*>(obj));
	}
	//---------------------------------------------------------------------------------------------------------
	ISerializeSystem::ISerializeSystem(void)
	{
		SYSTEM_SINGLETON_INITIALIZE;		
	}
	//---------------------------------------------------------------------------------------------------------
	ISerializeSystem::~ISerializeSystem(void)
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//---------------------------------------------------------------------------------------------------------
	void ISerializeSystem::Initialize()
	{
		m_PiarVector.resize(Rtti::MaxFactor + 1, NULL);
	}
	//---------------------------------------------------------------------------------------------------------
	void ISerializeSystem::Update()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void ISerializeSystem::Exit()
	{
		FOR_EACH(PiarVector, it, m_PiarVector)
		{
			ISerializable* res;
			if (NULL != *it)
			{
				while ((*it)->second->try_dequeue(res))
				{
					SAFE_DELETE(res);
				}
				SAFE_DELETE((*it)->first);
				SAFE_DELETE((*it)->second);
				SAFE_DELETE((*it));
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ISerializeSystem::PrepareSerializable(uint factor, uint count)
	{
		if (factor < m_PiarVector.size())
		{
			pair<ISerializable*, BufferQueue*>* host = m_PiarVector[factor];
			ASSERT(NULL != host);
			if (NULL != host)
			{
				for (int i = 0; i < count; ++i)
				{
					ISerializable* res = static_cast<ISerializable*>(host->first->Duplicate());
					host->second->enqueue(res);
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	ISerializable* ISerializeSystem::GetSerializable(uint factor)
	{
		ISerializable* res = NULL;
		if (factor < m_PiarVector.size())
		{
			pair<ISerializable*, BufferQueue*>* host = m_PiarVector[factor];
			ASSERT(NULL != host);
			if (NULL != host
				&& !host->second->try_dequeue(res))
			{
				res = static_cast<ISerializable*>(host->first->Duplicate());
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void ISerializeSystem::FreeSerializable(ISerializable* obj)
	{
		if (NULL != obj)
		{
			uint factor = obj->GetFactor();
			if (factor < m_PiarVector.size())
			{
				pair<ISerializable*, BufferQueue*>* host = m_PiarVector[factor];
				if (NULL != host)
				{
					obj->OnRecycle();
					host->second->enqueue(obj);
				}
			}
		}
	}

}