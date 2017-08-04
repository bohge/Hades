#pragma once
#include "ThreadSafeOprator.h"
#include "Volatileintpool.h"
#include "Predefine.h"







namespace hc
{
	typedef void (*RecycleFunction) (void*);
	template<typename T>
	class SmartPtr
	{
	private:
		T*					m_pPointer;
		volatile int*		m_pReferences;
		RecycleFunction		m_pRecycleFunction;
	public:
		//------------------------------------------------------------------------------------------------------
		SmartPtr()
			:m_pPointer(NULL),
			m_pReferences(NULL)
			, m_pRecycleFunction(NULL)
		{
		}
		//------------------------------------------------------------------------------------------------------
		explicit SmartPtr(T* ptr)
			:m_pPointer(NULL),
			m_pReferences(NULL)
			, m_pRecycleFunction(NULL)
		{
			_Copy(ptr, NULL);
		}
		//------------------------------------------------------------------------------------------------------
		explicit SmartPtr(T* ptr, RecycleFunction func)
			:m_pPointer(NULL),
			m_pReferences(NULL)
			, m_pRecycleFunction(NULL)
		{
			_Copy(ptr, func);
		}
		//------------------------------------------------------------------------------------------------------
		SmartPtr(T* ptr, volatile int* ref, RecycleFunction func)
			:m_pPointer(NULL),
			m_pReferences(NULL)
			, m_pRecycleFunction(NULL)
		{
			_Copy(ptr, ref, func);
		}
		//------------------------------------------------------------------------------------------------------
		SmartPtr(const SmartPtr<T>& sp)
			:m_pPointer(NULL),
			m_pReferences(NULL)
			, m_pRecycleFunction(NULL)
		{
			_Copy(sp);
		}
		//------------------------------------------------------------------------------------------------------
		template<typename U>
		explicit SmartPtr(U* ptr)
			:m_pPointer(NULL),
			m_pReferences(NULL)
			, m_pRecycleFunction(NULL)
		{
			_Copy(ptr, NULL);
		}
		//------------------------------------------------------------------------------------------------------
		template<typename U>
		SmartPtr(U* ptr, volatile int* ref, RecycleFunction func)
			:m_pPointer(NULL),
			m_pReferences(NULL)
			, m_pRecycleFunction(NULL)
		{
			_Copy(ptr, ref, func);
		}
		//------------------------------------------------------------------------------------------------------
		template<typename U>
		SmartPtr(const SmartPtr<U>& sp)
			:m_pPointer(NULL),
			m_pReferences(NULL)
			, m_pRecycleFunction(NULL)
		{
			_Copy(sp);
		}
		//------------------------------------------------------------------------------------------------------
		~SmartPtr()
		{
			_TryRelease();
		}
	private:
		//------------------------------------------------------------------------------------------------------
		template<typename U>
		HADES_FORCEINLINE void _Copy(U* ptr, RecycleFunction func)
		{
			T* ptrT = reinterpret_cast<T*>(ptr);
			if (m_pPointer != ptrT)
			{
				_TryRelease();
				m_pPointer = ptrT;
				m_pRecycleFunction = func;
				m_pReferences = NULL == ptrT ? NULL : Volatileintpool::Instance()->GetVolatileint(1);
			}
		}
		//------------------------------------------------------------------------------------------------------
		template<typename U>
		HADES_FORCEINLINE void _Copy(U* ptr, volatile int* ref, RecycleFunction func)
		{
			T* ptrT = reinterpret_cast<T*>(ptr);
			if (m_pPointer != ptrT)
			{
				_TryRelease();
				m_pPointer = ptrT;
				m_pReferences = ref;
				m_pRecycleFunction = func;
				if (m_pReferences)ThreadSafeOprator::Increment(m_pReferences);
			}
		}
		//------------------------------------------------------------------------------------------------------
		template<typename U>
		HADES_FORCEINLINE void _Copy(const SmartPtr<U>& sp)//拷贝
		{
			T* ptr = static_cast<T*>(sp.GetUnsafePointer());
			if (m_pPointer != ptr)
			{
				_TryRelease();
				m_pPointer = ptr;
				m_pReferences = sp.GetUnsafeReferences();
				m_pRecycleFunction = sp.GetRecycleFunction();
				if (m_pReferences)ThreadSafeOprator::Increment(m_pReferences);
			}
		}
		//------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE void _TryRelease()
		{
			if (NULL != m_pReferences //短路求值
				&& 0 == ThreadSafeOprator::Decrement(m_pReferences))
			{
				//SAFE_DELETE(m_pReferences);
				Volatileintpool::Instance()->FreeVolatileint(m_pReferences);
				if (m_pRecycleFunction)
				{
					m_pRecycleFunction(m_pPointer);
				}
				else
				{
					DEBUGCODE(char xxx[sizeof(T) > 0 ? 1 : -1]);
					SAFE_DELETE(m_pPointer);
				}
			}
		}
	public:
		//类内使用friend使其成为一个non-member函数
		//------------------------------------------------------------------------------------------------------
		friend HADES_FORCEINLINE bool operator == (const SmartPtr<T>& sp, const T* const ptr)
		{
			return sp.m_pPointer == ptr;
		}
		//------------------------------------------------------------------------------------------------------
		friend HADES_FORCEINLINE bool operator == (const T* const ptr, const SmartPtr<T>& sp)
		{
			return sp.m_pPointer == ptr;
		}
		//------------------------------------------------------------------------------------------------------
		friend HADES_FORCEINLINE bool operator != (const SmartPtr<T>& sp, const T* const ptr)
		{
			return sp.m_pPointer != ptr;
		}
		//------------------------------------------------------------------------------------------------------
		friend HADES_FORCEINLINE bool operator != (const T* const ptr, const SmartPtr<T>& sp)
		{
			return sp.m_pPointer != ptr;
		}
		//------------------------------------------------------------------------------------------------------
		template<typename CASTTO>
		friend HADES_FORCEINLINE SmartPtr<CASTTO> SmartPtr_Static_Cast(const SmartPtr<T>& rhs)
		{
			return SmartPtr<CASTTO>(static_cast<CASTTO*>(rhs.m_pPointer), rhs.m_pReferences);
		}
		//------------------------------------------------------------------------------------------------------
		template<typename CASTTO>
		friend HADES_FORCEINLINE SmartPtr<CASTTO> SmartPtr_Dynamic_Cast(const SmartPtr<T>& rhs)
		{
			return SmartPtr<CASTTO>(dynamic_cast<CASTTO*>(rhs.m_pPointer), rhs.m_pReferences);
		}
	public:
		HADES_FORCEINLINE bool isValid() const
		{
			return NULL != m_pPointer && NULL != m_pReferences;
		}
		HADES_FORCEINLINE T* GetUnsafePointer() const
		{
			return m_pPointer;
		}
		HADES_FORCEINLINE volatile int* GetUnsafeReferences() const
		{
			return m_pReferences;
		}
		HADES_FORCEINLINE RecycleFunction GetRecycleFunction() const
		{
			return m_pRecycleFunction;
		}
		//------------------------------------------------------------------------------------------------------
		template<typename U>
		HADES_FORCEINLINE const SmartPtr<T>& operator = (const SmartPtr<U>& sp)
		{
			_Copy(sp);
			return *this;
		}
		//------------------------------------------------------------------------------------------------------
		//template<typename U>//这个函数太危险了
		//HADES_FORCEINLINE const SmartPtr<T>& operator = ( U* ptr )
		//{
		//	_Copy( ptr );
		//	return *this;
		//}
		//------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE const SmartPtr<T>& operator = (const SmartPtr<T>& sp)
		{
			_Copy(sp);
			return *this;
		}
		//------------------------------------------------------------------------------------------------------
		//HADES_FORCEINLINE const SmartPtr<T>& operator = ( T* ptr )//这个函数太危险了
		//{
		//	_Copy( ptr );
		//	return *this;
		//}
		//------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE T* operator-> () const
		{
			return m_pPointer;
		}
		//------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE T& operator* () const
		{
			return *m_pPointer;
		}
		//------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE bool operator == (const SmartPtr<T>& rhs) const
		{
			return m_pPointer == rhs.m_pPointer;
		}
		//------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE bool operator != (const SmartPtr<T>& rhs) const
		{
			return m_pPointer != rhs.m_pPointer;
		}
		//------------------------------------------------------------------------------------------------------
		//		template<typename CASTTO> HADES_FORCEINLINE operator SmartPtr<CASTTO>& ()
		//		{
		//#ifdef _DEBUG
		//			CASTTO* ptr = this->m_pPointer;//检测指针转换是否是继承关系
		//#endif
		//			return *reinterpret_cast< SmartPtr<CASTTO>* >(this);
		//		}
	};
}