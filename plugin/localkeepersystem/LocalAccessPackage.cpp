#include "thread/IThreadSystem.h"
#include "thread/IThreadMutex.h"
#include "LocalAccessPackage.h"
#include "LocalKeeperSystem.h"
#include "keeper/Accessor.h"


using namespace hc;
namespace hlks
{
	//---------------------------------------------------------------------------------------------------------
	LocalAccessPackage::LocalAccessPackage(LocalKeeperSystem* sys)
		:m_rpLocalKeeperSystem(sys)
	{
		m_AcessCallback = MEMBER_FUNC_PTR(&LocalAccessPackage::OnAccessorCallback);
		m_pIThreadMutex = static_cast<IThreadMutex*>(
			IThreadSystem::Instance()->
			ThreadEntityFactory(ThreadProperty::ET_MUTEX));
	}
	//---------------------------------------------------------------------------------------------------------
	LocalAccessPackage::~LocalAccessPackage()
	{
		SAFE_DELETE(m_AcessCallback);
		IThreadSystem::Instance()->RecycleBin(m_pIThreadMutex);
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 LocalAccessPackage::CreateAccessor()
	{
		return m_rpLocalKeeperSystem->CreateAccessor();
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalAccessPackage::DestoryAccessor(uint64 id)
	{
		ASSERT(nullid != id);
		m_rpLocalKeeperSystem->DestoryAccessor(id);
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalAccessPackage::_DoRequest()
	{
		//此处曾经有一个重大bug，会导致死锁
		//出现原因，这里如果一次性将全部的请求都发送出去。
		//A任务对数据1只读，2只读或者读写，B任务对1读写，对2读写
		//在当前keeper的处理原则下只读可以并行，读写不能并行
		//当AB任务的执行顺序为B进入A在进入之时会发生
		//如果当前keeper中有一个对1的只读操作，那么B进来后无法对1获取访问权限，而得到2的访问权限，退出
		//此时A任务进入到任务队列中，获取了1的只读操作，而后无法获得2的操作，这样会陷入AB互相锁死的境地

		//还有一种情况，一次性都是放带来的线程问题
		//A执行到获取了1的，在获取2之前，线程切换了，B没获取1的但是得到2的了，一样会产生问题
		_DoReleaseAccessores();//先释放
		m_pIThreadMutex->Lock();
		AccessinfoArray& queue = _GetAccessOrder();
		m_SendIterator = queue.begin();
		_ExecuteRequest();
		m_pIThreadMutex->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalAccessPackage::_ExecuteRequest()
	{
		AccessinfoArray& queue = _GetAccessOrder();
		ASSERT(queue.end() != m_SendIterator);
		Accessinfo& ai = m_SendIterator->second;
		++m_SendIterator;
		switch (ai.eAccessMode)
		{
		case Accessor::AM_UNRIGOROUS:
		{
										m_rpLocalKeeperSystem->UnrigorousAccessor(ai.GID, ai.Timestamp, m_AcessCallback);
										break;
		}
		case Accessor::AM_READ_ONLY:
		{
									   m_rpLocalKeeperSystem->ReadonlyAccessor(ai.GID, ai.Timestamp, m_AcessCallback);
									   break;
		}
		case Accessor::AM_READ_WRITE:
		{
										m_rpLocalKeeperSystem->ReadwriteAccessor(ai.GID, ai.Timestamp, m_AcessCallback);
										break;
		}
		default:ASSERT(false);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalAccessPackage::OnAccessorCallback(hc::Accessor* acc)
	{
		//多线程返回不安全
		m_pIThreadMutex->Lock();
		AccessinfoArray& queue = _GetAccessOrder();
		if (NULL != acc)
		{
			AccessinfoArray::iterator it = queue.find(acc->GetID());
			ASSERT(queue.end() != it);
			it->second.pAccessor = acc;
		}
		if (queue.end() == m_SendIterator)
		{
			_Unicast();
		}
		else
		{
			_ExecuteRequest();
		}
		m_pIThreadMutex->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalAccessPackage::_DoReleaseAccessores()
	{
		AccessinfoArray& queue = _GetAccessOrder();
		FOR_EACH(AccessinfoArray, it, queue)
		{
			m_rpLocalKeeperSystem->ReleaseAccessor(it->second.pAccessor);
			it->second.pAccessor = NULL;
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalAccessPackage::_DoReleaseAccessor(uint64 gid)
	{
		ASSERT(nullid != gid);
		AccessinfoArray& queue = _GetAccessOrder();
		AccessinfoArray::iterator it = queue.find(gid);
		ASSERT(queue.end() != it);
		m_rpLocalKeeperSystem->ReleaseAccessor(it->second.pAccessor);
		it->second.pAccessor = NULL;
	}

}