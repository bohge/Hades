#pragma once
#include "ScriptProperty.h"
#include "IScriptSystem.h"
#include "ScriptValue.h"


#include <EASTL/map.h>

namespace hc
{
	//非线程安全，需要在用一个线程执行同一个虚拟机分配出来的continer
	class IDecoder;
	class IEncoder;
	class IMemberIterator;
	class IFunctionInvoker;
	class IFunctionIterator;
	class ICoroutineInvoker;
	HADES_CORE_API void RecursiveSaveInstance(const IMemberIterator& it, IEncoder* en);
	HADES_CORE_API void RecursiveSetInstance(IScriptInstance* instance, const IDecoder* de);
	class HADES_CORE_API IScriptInstance : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IScriptInstance);
	private:
		struct UserFunction
		{
			volatile int		m_User;
			IFunctionInvoker*	m_pInvoker;
			UserFunction(IFunctionInvoker* inv)
				:m_User(1)
				, m_pInvoker(inv){}
		};
	private:
		typedef eastl::map< uint, UserFunction* > FuncMap;
		typedef eastl::map< intptr_t, ICoroutineInvoker* > CorouMap;
	private:
		eastl::string	m_ClassName;
		FuncMap			m_FunctionMap;
		CorouMap		m_CoroutineMap;
		const uint		m_HostMachineIndex;
		const uint		m_nHashCode;
	public:
		IScriptInstance(uint host, uint hash);
		virtual ~IScriptInstance(void);
	private:
		virtual IFunctionInvoker* _DoSignFunciton(const eastl::string& name, const ScriptProperty::Arguments& args) = 0;
		virtual IFunctionInvoker* _DoSignFunciton(int userdata, const ScriptProperty::Arguments& args) = 0;
		virtual IFunctionInvoker* _DoSignFunciton(const eastl::string& name, uint res, const ScriptProperty::Arguments& args) = 0;
		virtual IFunctionInvoker* _DoSignFunciton(int userdata, uint res, const ScriptProperty::Arguments& args) = 0;
		virtual void _DoReleaseFunction(IFunctionInvoker* func) = 0;
		virtual ICoroutineInvoker* _DoSignCoroutine(const eastl::string& name, const ScriptProperty::Arguments& args) = 0;
		virtual ICoroutineInvoker* _DoSignCoroutine(const eastl::string& name, uint res, const ScriptProperty::Arguments& args) = 0;
		virtual void _DoReleaseCoroutine(ICoroutineInvoker* coro) = 0;
		virtual void _DoClearMember() = 0;
	public:
		virtual void SetMember(const char* name) = 0;
		virtual void SetMember(const char* name, bool obj) = 0;
		virtual void SetMember(const char* name, short obj) = 0;
		virtual void SetMember(const char* name, ushort obj) = 0;
		virtual void SetMember(const char* name, int obj) = 0;
		virtual void SetMember(const char* name, uint obj) = 0;
		virtual void SetMember(const char* name, float obj) = 0;
		virtual void SetMember(const char* name, int64 obj) = 0;
		virtual void SetMember(const char* name, uint64 obj) = 0;
		virtual void SetMember(const char* name, double obj) = 0;
		virtual void SetMember(const char* name, const char* obj, uint len) = 0;
		virtual void SetMember(const char* name, Object* obj, ScriptValue::ClearFunction func) = 0;
		virtual void BeginSetMemberBulk() = 0;
		virtual void ContinuouslySetMember(const char* name) = 0;
		virtual void ContinuouslySetMember(const char* name, bool obj) = 0;
		virtual void ContinuouslySetMember(const char* name, short obj) = 0;
		virtual void ContinuouslySetMember(const char* name, ushort obj) = 0;
		virtual void ContinuouslySetMember(const char* name, int obj) = 0;
		virtual void ContinuouslySetMember(const char* name, uint obj) = 0;
		virtual void ContinuouslySetMember(const char* name, float obj) = 0;
		virtual void ContinuouslySetMember(const char* name, int64 obj) = 0;
		virtual void ContinuouslySetMember(const char* name, uint64 obj) = 0;
		virtual void ContinuouslySetMember(const char* name, double obj) = 0;
		virtual void ContinuouslySetMember(const char* name, const char* obj, uint len) = 0;
		virtual void ContinuouslySetMember(const char* name, Object* obj, ScriptValue::ClearFunction func) = 0;
		virtual void ContinuouslySetMember(int key) = 0;
		virtual void ContinuouslySetMember(int key, bool obj) = 0;
		virtual void ContinuouslySetMember(int key, short obj) = 0;
		virtual void ContinuouslySetMember(int key, ushort obj) = 0;
		virtual void ContinuouslySetMember(int key, int obj) = 0;
		virtual void ContinuouslySetMember(int key, uint obj) = 0;
		virtual void ContinuouslySetMember(int key, float obj) = 0;
		virtual void ContinuouslySetMember(int key, int64 obj) = 0;
		virtual void ContinuouslySetMember(int key, uint64 obj) = 0;
		virtual void ContinuouslySetMember(int key, double obj) = 0;
		virtual void ContinuouslySetMember(int key, const char* obj, uint len) = 0;
		virtual void ContinuouslySetMember(int key, Object* obj, ScriptValue::ClearFunction func) = 0;
		virtual void ContinuouslySetMember(uint64 key) = 0;
		virtual void ContinuouslySetMember(uint64 key, bool obj) = 0;
		virtual void ContinuouslySetMember(uint64 key, short obj) = 0;
		virtual void ContinuouslySetMember(uint64 key, ushort obj) = 0;
		virtual void ContinuouslySetMember(uint64 key, int obj) = 0;
		virtual void ContinuouslySetMember(uint64 key, uint obj) = 0;
		virtual void ContinuouslySetMember(uint64 key, float obj) = 0;
		virtual void ContinuouslySetMember(uint64 key, int64 obj) = 0;
		virtual void ContinuouslySetMember(uint64 key, uint64 obj) = 0;
		virtual void ContinuouslySetMember(uint64 key, double obj) = 0;
		virtual void ContinuouslySetMember(uint64 key, const char* obj, uint len) = 0;
		virtual void ContinuouslySetMember(uint64 key, Object* obj, ScriptValue::ClearFunction func) = 0;
		virtual void EndSetMemberBulk() = 0;
		virtual const void* GetMember(const char* name) = 0;
		virtual const IMemberIterator& GetMemberIterator() = 0;
		virtual const IFunctionIterator& GetFunctionIterator() = 0;
		virtual IScriptInstance* CreateChieldInstance(const char* name) = 0;
		virtual IScriptInstance* CreateChieldInstance(int key) = 0;
		virtual IScriptInstance* CreateChieldInstance(uint64 key) = 0;
		virtual int GetScriptInstance() = 0;//返回脚本的所以，以提供得到原始的脚本数据结构
		virtual void GotError(const eastl::string& error) = 0;
	private:
		bool _isFunctionExist(const eastl::string& name);
		IFunctionInvoker* _DoTryGetFunction(const eastl::string& name, const ScriptProperty::Arguments& args);
		IFunctionInvoker* _DoTryGetFunction(int userdata, const ScriptProperty::Arguments& args);
		IFunctionInvoker* _DoTryGetFunction(const eastl::string& name, uint res, const ScriptProperty::Arguments& args);
		IFunctionInvoker* _DoTryGetFunction(int userdata, uint res, const ScriptProperty::Arguments& args);

		ICoroutineInvoker* _DoTryGetCoroutine(const eastl::string& name, const ScriptProperty::Arguments& args);
		ICoroutineInvoker* _DoTryGetCoroutine(const eastl::string& name, uint res, const ScriptProperty::Arguments& args);
	protected:
		void _SetClassName(const eastl::string& name);
	public:
		void Deserialize(IDecoder* de);
		void Serialize(IEncoder* en);
		//签名一个函数
		IFunctionInvoker* SignFunciton(const eastl::string& name);
		IFunctionInvoker* SignFunciton(const eastl::string& name, uint arg1);
		IFunctionInvoker* SignFunciton(const eastl::string& name, uint arg1, uint arg2);
		IFunctionInvoker* SignFunciton(const eastl::string& name, uint arg1, uint arg2, uint arg3);
		IFunctionInvoker* SignFunciton(const eastl::string& name, uint arg1, uint arg2, uint arg3, uint arg4);
		IFunctionInvoker* SignFunciton(int userdata);
		IFunctionInvoker* SignFunciton(int userdata, int arg1);
		IFunctionInvoker* SignFunciton(int userdata, int arg1, int arg2);
		IFunctionInvoker* SignFunciton(int userdata, int arg1, int arg2, int arg3);
		IFunctionInvoker* SignFunciton(int userdata, int arg1, int arg2, int arg3, int arg4);
		IFunctionInvoker* SignFuncitonWithReturn(uint res, const eastl::string& name);
		IFunctionInvoker* SignFuncitonWithReturn(uint res, const eastl::string& name, uint arg1);
		IFunctionInvoker* SignFuncitonWithReturn(uint res, const eastl::string& name, uint arg1, uint arg2);
		IFunctionInvoker* SignFuncitonWithReturn(uint res, const eastl::string& name, uint arg1, uint arg2, uint arg3);
		IFunctionInvoker* SignFuncitonWithReturn(uint res, const eastl::string& name, uint arg1, uint arg2, uint arg3, uint arg4);
		IFunctionInvoker* SignFuncitonWithReturn(int res, int userdata);
		IFunctionInvoker* SignFuncitonWithReturn(int res, int userdata, int arg1);
		IFunctionInvoker* SignFuncitonWithReturn(int res, int userdata, int arg1, int arg2);
		IFunctionInvoker* SignFuncitonWithReturn(int res, int userdata, int arg1, int arg2, int arg3);
		IFunctionInvoker* SignFuncitonWithReturn(int res, int userdata, int arg1, int arg2, int arg3, int arg4);
		//签名协程
		ICoroutineInvoker* SignCoroutine(const eastl::string& name);
		ICoroutineInvoker* SignCoroutine(const eastl::string& name, uint arg1);
		ICoroutineInvoker* SignCoroutine(const eastl::string& name, uint arg1, uint arg2);
		ICoroutineInvoker* SignCoroutine(const eastl::string& name, uint arg1, uint arg2, uint arg3);
		ICoroutineInvoker* SignCoroutine(const eastl::string& name, uint arg1, uint arg2, uint arg3, uint arg4);
		ICoroutineInvoker* SignCoroutine(uint res, const eastl::string& name);
		ICoroutineInvoker* SignCoroutine(uint res, const eastl::string& name, uint arg1);
		ICoroutineInvoker* SignCoroutine(uint res, const eastl::string& name, uint arg1, uint arg2);
		ICoroutineInvoker* SignCoroutine(uint res, const eastl::string& name, uint arg1, uint arg2, uint arg3);
		ICoroutineInvoker* SignCoroutine(uint res, const eastl::string& name, uint arg1, uint arg2, uint arg3, uint arg4);
		void ClearMember();
		void UnsignInvoker(IFunctionInvoker* func);
		void UnsignInvoker(ICoroutineInvoker* func);
		void ReleaseAllInvoker();
	public:
		HADES_FORCEINLINE uint GetScriptHash() const;
		HADES_FORCEINLINE bool isFunctionExist(const eastl::string& name);
		HADES_FORCEINLINE uint GetMachineIndex() const;
		HADES_FORCEINLINE const eastl::string& GetClassName();
	};



	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const eastl::string& IScriptInstance::GetClassName()
	{
		return m_ClassName;
	}
	//------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool IScriptInstance::isFunctionExist(const eastl::string& name)
	{
		return _isFunctionExist(name);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint IScriptInstance::GetScriptHash() const
	{
		return m_nHashCode;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint IScriptInstance::GetMachineIndex() const
	{
		return m_HostMachineIndex;
	}
}

