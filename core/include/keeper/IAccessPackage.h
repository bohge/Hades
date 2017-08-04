#pragma once
#include "Accessinfo.h"


#include <EASTL/map.h>
namespace hc
{
	class Accessor;	
	class HADES_CORE_API IAccessPackage : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IAccessPackage);
	private:
		enum Constant
		{
			APC_DEFUALT_ACESSINFO_SIZE = 10,
		};
	private:
		bool					m_haveRequest;
		AccessinfoArray			m_AccessOrder;
		AccessorRequestCallback	m_AccessorRequestCallback;
	public:
		IAccessPackage();
		virtual ~IAccessPackage();
	private:
		virtual void _DoRequest() = 0;//请求，实现者需要考虑再次请求的问题
		virtual void _DoReleaseAccessores() = 0;//释放全部请求
		virtual void _DoReleaseAccessor(uint64 gid) = 0;//释放单个请求
	public:
		virtual uint64 CreateAccessor() = 0;//只创建，不请求，需要再调用请求
		virtual void DestoryAccessor(uint64 id) = 0;
	public:
		//以下全部的callback函数必须是线程安全的，应为keeper系统可能会是多线程返回访问结果

		//非严格模式读取内存数据，读取该数据的时候可能其他数据正在对它进行修改。
		//试用环境，例如当广播玩家为之变更时，需要确定其他玩家是否在可见范围内，
		//可通过这个函数得到一个前一个有效地玩家位置，然后计算，广播
		bool UnrigorousAccessor(uint64 id, int timestamp);

		//严格模式读取锁，当有读写锁在激活状态时，需要读写锁释放后才可以读取
		//但是当有其他读锁在激活时态，该锁可以继续读取。在试用这种方式读取后
		//数据不能被保存。
		bool ReadonlyAccessor(uint64 id, int timestamp);

		//严格模式读写锁，当有读所或者读写锁激活时，需要读写锁释与读锁释放放后才
		//可以读取。用这种方式获取的读写器是唯一可以修改数据的，在释放读写器之后
		//回将修改内容保存在内存中。
		bool ReadwriteAccessor(uint64 id, int timestamp);

	public:
		void MayChangeAccesorMode(uint64 id, Accessor::AccessMode am);
		void BeginRequest();
		void ReleaseAccessores();//全部释放
		void ReleaseAccessor(uint64 gid);//释放控制权以及请求
	private:
		HADES_FORCEINLINE bool _isRquested(uint64 id);
	protected:		
		HADES_FORCEINLINE AccessinfoArray& _GetAccessOrder();
		HADES_FORCEINLINE void _Unicast();
	public:
		//释放访问其，自动判断是否需要保存修改数据。
		HADES_FORCEINLINE void SetAccessorCallback(AccessorRequestCallback::Callback func);
		HADES_FORCEINLINE bool HasRequest();
		HADES_FORCEINLINE bool isReleased();
		HADES_FORCEINLINE void UpdateTimestamp(uint64 id, int t);
	};




	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool IAccessPackage::isReleased()
	{
		return m_AccessOrder.empty();
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IAccessPackage::UpdateTimestamp(uint64 id, int t)
	{
		AccessinfoArray::iterator it = m_AccessOrder.find(id);
		if (m_AccessOrder.end() != it)
		{
			it->second.Timestamp = t;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool IAccessPackage::_isRquested(uint64 id)
	{
		return m_AccessOrder.end() != m_AccessOrder.find(id);
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE AccessinfoArray& IAccessPackage::_GetAccessOrder()
	{
		return m_AccessOrder;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IAccessPackage::_Unicast()
	{
		m_haveRequest = false;
		m_AccessorRequestCallback.Unicast(m_AccessOrder);
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IAccessPackage::SetAccessorCallback(AccessorRequestCallback::Callback func)
	{
		m_AccessorRequestCallback.Add(func);
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool IAccessPackage::HasRequest()
	{
		return m_haveRequest;
	}
}