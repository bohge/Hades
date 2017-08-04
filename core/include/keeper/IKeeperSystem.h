#pragma once
#include "core/Singleton.hpp"
#include "core/Delegate.hpp"
#include "core/IPlugin.h"



#include <EASTL/string.h>
#include <EASTL/vector.h>

namespace hc
{
	class Accessor;
	class IAccessPackage;	
	//多线程访问控制器
	//实现接口的继承者返回的必须是内存的备份，不能返回内存本身，防止在调用
	//非严格模式读取与写模式读写同一个内存区域产生的崩溃
	class HADES_CORE_API IKeeperSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(IKeeperSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IKeeperSystem, IPlugin);
	public:
		IKeeperSystem(void);
		virtual ~IKeeperSystem(void);
	public:
		virtual void Initialize();
		virtual void Exit();
	public:
		//请求对数据操作，发送一个数据的请求包
		virtual IAccessPackage* AccessPackageFactory() = 0;
		virtual void RecycleBin(IAccessPackage* package) = 0;
		virtual void RequestAccessor(IAccessPackage* pack) = 0;
	public:
		static inline int IncreaseTimestamp(int old);
	};


	//---------------------------------------------------------------------------------------------------------
	int IKeeperSystem::IncreaseTimestamp(int old)
	{
		return old + 1;
	}
}
