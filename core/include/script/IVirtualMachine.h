#pragma once
#include "script/IScriptInstance.h"
#include "concurrentqueue.h"
#include "script/Syncdata.h"
#include "core/SmartPtr.hpp"
#include "core/Object.hpp"
#include "ScriptValue.h"


#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <EASTL/set.h>

namespace hc
{
	//该虚拟机并非线程安全，需要在多个线程上启动多个虚拟机
	class ISerializer;
	class ClassRegister;
	class IScriptInstance;
	class HADES_CORE_API IVirtualMachine : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IVirtualMachine);
		enum Constant
		{
			VMC_PREPAR_ARRAY_SIZE = 10000,
		};
	private:
		typedef eastl::set<uint> RegistedClassSet;
		typedef eastl::map< uint, IScriptInstance* > InstanceMap;
		typedef moodycamel::ConcurrentQueue< SmartPtr<Syncdata> > SynchronizeQueue;
	private:
		RegistedClassSet	m_RegistedClassSet;
		InstanceMap			m_InstanceMap;
		SynchronizeQueue	m_SynchronizeQueue;
		ISerializer*		m_pISerializer;
		void*				m_rpUserData;
		const uint			m_MachineIndex;
	public:
		IVirtualMachine(uint index);
		virtual ~IVirtualMachine(void);
	private:
		virtual IScriptInstance* _DoLoadScript(uint hash, const eastl::string& path) = 0;
		virtual IScriptInstance* _DoRegisteInstance(uint hash, int scriptdata) = 0;
		virtual IScriptInstance* _DoCreateInstance() = 0;
		virtual IScriptInstance* _DoCreateInstance(uint hash, const eastl::string& name) = 0;
		virtual void _DoReleaseScript(IScriptInstance* instance) = 0;
		virtual void _DoReferenceClass(ClassRegister* c) = 0;//注册类
		virtual void _DoUpdate() = 0;
	public:
		virtual void AddScriptPath(const eastl::string& path) = 0;
		virtual void AddNativePath(const eastl::string& path) = 0;
		virtual bool isRegisteredClass(const eastl::string& name) = 0;//是否注册
		virtual void SetGlobal(const char* name) = 0;//设置空
		virtual void SetGlobal(const char* name, int obj) = 0;//设置全局变量
		virtual void SetGlobal(const char* name, bool obj) = 0;//设置全局变量		
		virtual void SetGlobal(const char* name, float obj) = 0;//设置全局变量	
		virtual void SetGlobal(const char* name, const eastl::string& obj) = 0;//设置全局变量	
		virtual void SetGlobal(const char* name, uint64 obj) = 0;
		virtual void SetGlobal(const char* name, Object* obj, void* rec, ScriptValue::ClearFunction func) = 0;//设置全局变量
		virtual Object* GetGlobal(const char* name) = 0;//取得全局变量
		//virtual int SetGlobalReference(uint64 id) = 0;//给服务体系使用的，特殊，保证知道调用者
		//virtual uint64 GetGlobalReference(int index) = 0;
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	private:
		IScriptInstance* _GetCreateInstance(const eastl::string& name);
	public:
		void ReferenceClass(ClassRegister* c);//提供rpc用
		IScriptInstance* LoadScript(const eastl::string& path);
		IScriptInstance* RegisteInstance(int scriptdata);
		IScriptInstance* RegisteInstance(int scriptdata, const eastl::string& name);
		void ReleaseScript(IScriptInstance* instance);
	public:
		HADES_FORCEINLINE void Synchronize(SmartPtr<Syncdata>& data);
		HADES_FORCEINLINE uint GetIndex() const;
		HADES_FORCEINLINE void CacheInstance(IScriptInstance* instance);
		HADES_FORCEINLINE IScriptInstance* GetInstance();
		HADES_FORCEINLINE void* GetUserData();
		HADES_FORCEINLINE void SetUserData(void* ud);
	};



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void* IVirtualMachine::GetUserData()
	{
		return m_rpUserData;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IVirtualMachine::SetUserData(void* ud)
	{
		m_rpUserData = ud;
	}
	//---------------------------------------------------------------------------------------------------------
	void IVirtualMachine::Synchronize(SmartPtr<Syncdata>& data)
	{
		m_SynchronizeQueue.enqueue(data);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint IVirtualMachine::GetIndex() const
	{
		return m_MachineIndex;
	}
}
