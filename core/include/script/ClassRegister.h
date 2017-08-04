#pragma once
#include "FunctionRegister.h"

#include <EASTL/string.h>
#include <EASTL/map.h>

namespace hc
{
	class Rtti;
	class Object;
	class HADES_CORE_API ClassRegister : public Object
	{
		RTTI_TYPE_DECLEAR(ClassRegister);
	private:
		enum Constant
		{
			CRC_ARGUMENT_COUNT = 10,
		};
	private:
		const Rtti*			m_prRtti;
		const Rtti*			m_prBase;
		ClassFuncitonArray	m_FuncArray;
		ClassConstantArray	m_ConstantArray;
	public:
		ClassRegister(void);
		~ClassRegister(void);
	public:
		ClassRegister(const ClassRegister& sc);
		const ClassRegister& operator = (const ClassRegister& sc);
	private:
		void _DoCopy(const ClassRegister& sc);
	public:
		void ClassFunction(const FunctionRegister& func);//注册成员函数，和c++一样返回值不能做为重载的区分符号
		void ClassType(const Rtti* rtti);//注册一个类
		void ClassType(const Rtti* rtti, const Rtti* base);//注册一个类,继承
		void ClassConstant(int v, const eastl::string& name);
		void ClassConstant(float v, const eastl::string& name);
		void ClassConstant(double v, const eastl::string& name);
		const char* GetClassTypeName() const;//返回类名
		const char* GetBaseClassTypeName() const;//返回类名
		uint GetClassTypeID() const;//得到类信息ID
		uint GetClassTypeSize() const;//返回类大小
	public:
		HADES_FORCEINLINE FunctionRegister* TryGetFunction(int index, uint argcount);
		HADES_FORCEINLINE FunctionRegister* GetFunction(int index, const Rtti* o1);
		HADES_FORCEINLINE FunctionRegister* GetFunction(int index, const Rtti* o1, const Rtti* o2);
		HADES_FORCEINLINE FunctionRegister* GetFunction(int index, const Rtti* o1, const Rtti* o2, const Rtti* o3);
		HADES_FORCEINLINE FunctionRegister* GetFunction(int index, const Rtti* o1, const Rtti* o2, const Rtti* o3, const Rtti* o4);
		HADES_FORCEINLINE FunctionRegister* GetFunction(int index, const Rtti* o1, const Rtti* o2, const Rtti* o3, const Rtti* o4, const Rtti* o5);
		HADES_FORCEINLINE FunctionRegister* GetFunction(int index, const Rtti* o1, const Rtti* o2, const Rtti* o3, const Rtti* o4, const Rtti* o5, const Rtti* o6);
	public:
		HADES_FORCEINLINE bool isDriverType() const;
		HADES_FORCEINLINE bool isOverrideFunction(uint index);
		HADES_FORCEINLINE uint GetFunctionListCount() const;
		HADES_FORCEINLINE uint GetClassConstantCount() const;
		HADES_FORCEINLINE const eastl::string& GetFunctionListName(uint index) const;
		HADES_FORCEINLINE ScriptProperty::FunctionType GetFunctionListType(uint index) const;
		HADES_FORCEINLINE FunctionRegister* GetFunction(int index) const;
		HADES_FORCEINLINE const OverlappeArray& GetFunctions(int index) const;
		HADES_FORCEINLINE CLassConstant GetClassConstant(int index) const;
		HADES_FORCEINLINE bool isTypeof(const Rtti& rhs);
	};



	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE FunctionRegister* ClassRegister::TryGetFunction(int index, uint argcount)
	{
		FunctionRegister* res = NULL;
		const FunctionArray& funcArray = m_FuncArray[index].second[argcount];//一个参数
		if (1 == funcArray.size())
		{
			res = *(funcArray.begin());
		}
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE FunctionRegister* ClassRegister::GetFunction(int index, const Rtti* o1)
	{
		FunctionRegister* res = NULL;
		const FunctionArray& funcArray = m_FuncArray[index].second[1];//一个参数
		ASSERT(1 != funcArray.size());
		FOR_EACH_CONST(FunctionArray, func, funcArray)
		{
			if ((*func)->isMatchArguments(o1))
			{
				res = *func;
				break;
			}
		}
		ASSERT(NULL != res && "no match arguments!");
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE FunctionRegister* ClassRegister::GetFunction(int index, const Rtti* o1, const Rtti* o2)
	{
		FunctionRegister* res = NULL;
		const FunctionArray& funcArray = m_FuncArray[index].second[2];//一个参数
		ASSERT(1 != funcArray.size());
		FOR_EACH_CONST(FunctionArray, func, funcArray)
		{
			if ((*func)->isMatchArguments(o1, o2))
			{
				res = *func;
				break;
			}
		}
		ASSERT(NULL != res && "no match arguments!");
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE FunctionRegister* ClassRegister::GetFunction(int index, const Rtti* o1, const Rtti* o2, const Rtti* o3)
	{
		FunctionRegister* res = NULL;
		const FunctionArray& funcArray = m_FuncArray[index].second[3];//一个参数
		ASSERT(1 != funcArray.size());
		FOR_EACH_CONST(FunctionArray, func, funcArray)
		{
			if ((*func)->isMatchArguments(o1, o2, o3))
			{
				res = *func;
				break;
			}
		}
		ASSERT(NULL != res && "no match arguments!");
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE FunctionRegister* ClassRegister::GetFunction(int index, const Rtti* o1, const Rtti* o2, const Rtti* o3, const Rtti* o4)
	{
		FunctionRegister* res = NULL;
		const FunctionArray& funcArray = m_FuncArray[index].second[4];//一个参数
		ASSERT(1 != funcArray.size());
		FOR_EACH_CONST(FunctionArray, func, funcArray)
		{
			if ((*func)->isMatchArguments(o1, o2, o3, o4))
			{
				res = *func;
				break;
			}
		}
		ASSERT(NULL != res && "no match arguments!");
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE FunctionRegister* ClassRegister::GetFunction(int index, const Rtti* o1, const Rtti* o2, const Rtti* o3, const Rtti* o4, const Rtti* o5)
	{
		FunctionRegister* res = NULL;
		const FunctionArray& funcArray = m_FuncArray[index].second[5];//一个参数
		ASSERT(1 != funcArray.size());
		FOR_EACH_CONST(FunctionArray, func, funcArray)
		{
			if ((*func)->isMatchArguments(o1, o2, o3, o4, o5))
			{
				res = *func;
				break;
			}
		}
		ASSERT(NULL != res && "no match arguments!");
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE FunctionRegister* ClassRegister::GetFunction(int index, const Rtti* o1, const Rtti* o2, const Rtti* o3, const Rtti* o4, const Rtti* o5, const Rtti* o6)
	{
		FunctionRegister* res = NULL;
		const FunctionArray& funcArray = m_FuncArray[index].second[6];//一个参数
		ASSERT(1 != funcArray.size());
		FOR_EACH_CONST(FunctionArray, func, funcArray)
		{
			if ((*func)->isMatchArguments(o1, o2, o3, o4, o5, o6))
			{
				res = *func;
				break;
			}
		}
		ASSERT(NULL != res && "no match arguments!");
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool ClassRegister::isDriverType() const
	{
		return NULL != m_prBase;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool ClassRegister::isOverrideFunction(uint index)
	{
		int count = 0;
		FOR_EACH(OverlappeArray, funcs, m_FuncArray[index].second)
		{
			FOR_EACH(FunctionArray, func, *funcs)
			{
				++count;
			}
		}
		return count > 1;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint ClassRegister::GetFunctionListCount() const
	{
		return m_FuncArray.size();
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint ClassRegister::GetClassConstantCount() const
	{
		return m_ConstantArray.size();
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const eastl::string& ClassRegister::GetFunctionListName(uint index) const
	{
		return m_FuncArray[index].first;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE ScriptProperty::FunctionType ClassRegister::GetFunctionListType(uint index) const
	{
		return GetFunction(index)->GetFunctionType();
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE FunctionRegister* ClassRegister::GetFunction(int index) const
	{
		FunctionRegister* res = NULL;
		FOR_EACH_CONST(OverlappeArray, funcarray, m_FuncArray[index].second)
		{
			if (0 != funcarray->size())
			{
				res = *funcarray->begin();
				break;
			}
		}
		ASSERT(NULL != res);
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE const OverlappeArray& ClassRegister::GetFunctions(int index) const
	{
		return m_FuncArray[index].second;
	}
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE CLassConstant ClassRegister::GetClassConstant(int index) const
	{
		return m_ConstantArray[index];
	}	
	//-------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE bool ClassRegister::isTypeof(const Rtti& rhs)
	{
		return m_prRtti->isTypeOrDriverType(rhs);
	}
}
