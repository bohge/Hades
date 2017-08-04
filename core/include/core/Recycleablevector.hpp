#pragma once
#include "thread/IThreadSystem.h"
#include "thread/IThreadRWLock.h"
#include "concurrentqueue.h"
#include "core/Utility.hpp"
#include "log/ILogSystem.h"


#include <EASTL/vector.h>
#include <atomic>

namespace hc
{
	template<typename T>
	class Recycleablevector
	{
	private:
		typedef eastl::pair< ushort, T > DataPair;
		typedef eastl::vector< DataPair > ClientVector;
		typedef moodycamel::ConcurrentQueue< uint > LockfreeQueue;
	private:
		uint				m_Size;
		uint				m_Grow;
		//std::atomic<ushort>	m_UUID;
		ushort				m_UUID;
		IThreadRWLock*		m_pIThreadRWLock;
		ClientVector		m_ClientVector;
		LockfreeQueue		m_LockfreeQueue;
	public:
		Recycleablevector()
			:m_Size(0)
			, m_Grow(0)
			, m_UUID(1)
			//, m_UUID(ushort(nullhandle))
		{
			m_pIThreadRWLock = static_cast<IThreadRWLock*>(
				IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_RWLOCK));
		}
		~Recycleablevector()
		{
			IThreadSystem::Instance()->RecycleBin(m_pIThreadRWLock);
		}
	public:
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE void SetBeginUUID(ushort uuid)
		{
			m_UUID = uuid;
			m_UUID = 0 == m_UUID ? 1 : m_UUID;
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE uint Size()
		{
			return m_ClientVector.size();
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE void Initialize(uint size)
		{
			m_pIThreadRWLock->WriteLock();
			ASSERT(m_ClientVector.empty());
			m_Size = size;
			m_Grow = size;
			m_ClientVector.resize(size);
			m_pIThreadRWLock->Unlock();
			for (int i = 0; i < size; ++i)
			{
				m_LockfreeQueue.enqueue(i);
			}
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE uint64 Push(T data)
		{
			ASSERT(0 != m_Grow);
			uint id(0);
			ushort uu(0);
			while (!m_LockfreeQueue.try_dequeue(id))
			{
				m_pIThreadRWLock->WriteLock();
				uint was = m_Size;
				m_Size += m_Grow;
				uint now = m_Size;
				m_ClientVector.resize(m_Size);
				m_pIThreadRWLock->Unlock();
				for (size_t i = was; i < now; i++)
				{
					m_LockfreeQueue.enqueue(i);
				}
			}
			m_pIThreadRWLock->ReadLock();
			DataPair& dp = m_ClientVector[id];
			uu = ++m_UUID;
			dp.first = uu;
			dp.second = data;
			m_pIThreadRWLock->Unlock();
			return Utility::ComposeKey(id, uu);
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE void Remove(uint64 key)
		{
			ASSERT(0 != m_Grow);
			uint index;
			ushort uuid;
			Utility::SeparateKey(key, index, uuid);
			m_pIThreadRWLock->ReadLock();
			DataPair& dp = m_ClientVector[index];
			if (dp.first != uuid)
			{
				HADESERROR("Try remove index but uuid is not same");
			}
			else
			{
				dp.first = 0;
				dp.second = T(NULL);
				m_LockfreeQueue.enqueue(index);
			}
			m_pIThreadRWLock->Unlock();
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE bool Have(uint64 key)
		{
			ASSERT(0 != m_Grow);
			uint index;
			ushort uuid;
			Utility::SeparateKey(key, index, uuid);
			bool res = false;
			m_pIThreadRWLock->ReadLock();
			if (index < m_ClientVector.size())
			{
				DataPair& dp = m_ClientVector[index];
				res = dp.first == uuid;
			}
			m_pIThreadRWLock->Unlock();
			return res;
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE bool UncheckHave(uint index)
		{
			ASSERT(0 != m_Grow);
			bool res = false;
			m_pIThreadRWLock->ReadLock();
			if (index < m_ClientVector.size())
			{
				res = T(NULL) != m_ClientVector[index].second;
			}
			m_pIThreadRWLock->Unlock();
			return res;
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE T& Get(uint64 key)
		{
			ASSERT(0 != m_Grow);
			uint index = Utility::GetIndexFormKey(key);
			m_pIThreadRWLock->ReadLock();
			DataPair& dp = m_ClientVector[index];
			ASSERT(dp.first == Utility::GetUUIDFormKey(key));
			m_pIThreadRWLock->Unlock();
			return dp.second;
		}
		//---------------------------------------------------------------------------------------------------------
	};
}