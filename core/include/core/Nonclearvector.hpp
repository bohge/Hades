#pragma once
#include "core/Predefine.h"


#include <EASTL/vector.h>

namespace hc
{
	template<typename T>
	class Nonclearvector
	{
	private:
		typedef eastl::vector<T> ClientVector;
	public:
		typedef typename ClientVector::iterator					iterator;
		typedef typename ClientVector::const_iterator			const_iterator;
	private:
		uint				m_Index;
		ClientVector		m_ClientVector;
	public:
		Nonclearvector() :m_Index(0){}
		~Nonclearvector(){}
	public:
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE void reserve(uint size)
		{
			m_ClientVector.reserve(size);
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE uint size() const
		{
			return m_Index;
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE void push_back(const T& v)
		{
			if (m_Index < m_ClientVector.size())
			{
				m_ClientVector[m_Index] = v;
			}
			else
			{
				m_ClientVector.push_back(v);
			}
			++m_Index;
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE void clear()
		{
			m_Index = 0;
		}
		HADES_FORCEINLINE void erase(typename ClientVector::iterator it)
		{
			--m_Index;
			m_ClientVector.erase(it);
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE typename ClientVector::iterator begin()
		{
			return m_ClientVector.begin();
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE typename ClientVector::const_iterator begin() const
		{
			return m_ClientVector.begin();
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE typename ClientVector::iterator end()
		{
			typename ClientVector::iterator res = m_Index < m_ClientVector.size()
				? &m_ClientVector[m_Index]
				: m_ClientVector.end();
			return res;
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE typename ClientVector::const_iterator end() const
		{
			typename ClientVector::const_iterator res = m_Index < m_ClientVector.size()
				? &m_ClientVector[m_Index]
				: m_ClientVector.end();
			return res;
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE typename ClientVector::const_pointer data() const
		{
			return m_ClientVector.data();
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE typename ClientVector::reference front()
		{
			return m_ClientVector.front();
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE typename ClientVector::const_reference front() const
		{
			return m_ClientVector.front();
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE typename ClientVector::reference back()
		{
			return m_ClientVector.back();
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE typename ClientVector::const_reference back() const
		{
			return m_ClientVector.back();
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE typename ClientVector::reference operator [](uint index)
		{
			return m_ClientVector[index];
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE typename ClientVector::const_reference operator [](uint index) const
		{
			return m_ClientVector[index];
		}
	};
}