#include "script/ClassRegister.h"
#include "log/ILogSystem.h"
#include "core/Types.h"


using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT(ClassRegister);
	//-------------------------------------------------------------------------------------------------------
	ClassRegister::ClassRegister(void)
		:m_prRtti(NULL),
		m_prBase(NULL)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	ClassRegister::ClassRegister(const ClassRegister& sc)
	{
		_DoCopy(sc);
	}
	//-------------------------------------------------------------------------------------------------------
	ClassRegister::~ClassRegister(void)
	{
		FOR_EACH(ClassConstantArray, constant, m_ConstantArray)
		{
			SAFE_DELETE(constant->second);
		}
		FOR_EACH(ClassFuncitonArray, funcMap, m_FuncArray)
		{
			FOR_EACH(OverlappeArray, funcs, funcMap->second)
			{
				FOR_EACH(FunctionArray, func, *funcs)
				{
					SAFE_DELETE(*func);
				}
			}
		}
	}
	//-------------------------------------------------------------------------------------------------------
	const ClassRegister& ClassRegister::operator=(const ClassRegister& sc)
	{
		_DoCopy(sc);
		return *this;
	}
	//-------------------------------------------------------------------------------------------------------
	void ClassRegister::_DoCopy(const ClassRegister& sc)
	{
		m_prRtti = sc.m_prRtti;
		m_prBase = sc.m_prBase;
		FOR_EACH_CONST(ClassFuncitonArray, funcMap, sc.m_FuncArray)
		{
			FOR_EACH_CONST(OverlappeArray, funcs, funcMap->second)
			{
				FOR_EACH_CONST(FunctionArray, func, *funcs)
				{
					ClassFunction(**func);
				}
			}
		}
		FOR_EACH_CONST(ClassConstantArray, cc, sc.m_ConstantArray)
		{
			m_ConstantArray.push_back(make_pair(cc->first, cc->second->Duplicate()));
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ClassRegister::ClassFunction(const FunctionRegister& func)
	{
		uint index = 0;
		FunctionRegister* pFunc = NEW FunctionRegister(func);
		ClassFuncitonArray::iterator funcOvMp = m_FuncArray.end();

		FOR_EACH(ClassFuncitonArray, it, m_FuncArray)
		{
			if (it->first == pFunc->GetName())
			{
				funcOvMp = it;
				break;
			}
			++index;
		}
		pFunc->SetIndex(index);
		if (m_FuncArray.end() == funcOvMp) //第一次加入这个名称的函数
		{
			OverlappeArray oa;
			oa.resize(CRC_ARGUMENT_COUNT);
			FunctionArray funcArray;
			funcArray.push_back(pFunc);
			int index = pFunc->GetArgumentCount();
			oa[index] = funcArray;
			m_FuncArray.push_back(make_pair(pFunc->GetName(), oa));
		}
		else//找到函数序列了
		{
			bool isinsert = true;
			uint index = pFunc->GetArgumentCount();
			FunctionArray& funcArray = funcOvMp->second[index];//找到对应参数数量的数组
			FOR_EACH(FunctionArray, f, funcArray)
			{
				if ((*f)->isSameFunction(pFunc))
				{
					isinsert = false;
					break;
				}
			}
			if (isinsert)
			{
				funcArray.push_back(pFunc);
			}
			//else
			//{
			//	HADESWARNING("Lua function %s is overrided\r\n", pFunc->GetName().c_str());
			//}
			DEBUGCODE(if (0 == pFunc->GetArgumentCount()){ ASSERT(1 == funcArray.size()); });
		}
	}
	//-------------------------------------------------------------------------------------------------------
	void ClassRegister::ClassType(const Rtti* rtti)
	{
		m_prRtti = rtti;
	}
	//-------------------------------------------------------------------------------------------------------
	void ClassRegister::ClassType(const Rtti* rtti, const Rtti* base)
	{
		ClassType(rtti);
		m_prBase = base;
	}
	//-------------------------------------------------------------------------------------------------------
	uint ClassRegister::GetClassTypeID() const
	{
		return m_prRtti->GetTypeID();
	}
	//-------------------------------------------------------------------------------------------------------
	const char* ClassRegister::GetClassTypeName() const
	{
		return m_prRtti->GetTypeName();
	}
	//-------------------------------------------------------------------------------------------------------
	uint ClassRegister::GetClassTypeSize() const
	{
		return m_prRtti->GetTypeSize();
	}
	//-------------------------------------------------------------------------------------------------------
	const char* ClassRegister::GetBaseClassTypeName() const
	{
		return m_prBase->GetTypeName();
	}
	//-------------------------------------------------------------------------------------------------------
	void ClassRegister::ClassConstant(int v, const string& name)
	{
		m_ConstantArray.push_back(make_pair(name, NEW Int(v)));
	}
	//-------------------------------------------------------------------------------------------------------
	void ClassRegister::ClassConstant(float v, const string& name)
	{
		m_ConstantArray.push_back(make_pair(name, NEW Float(v)));
	}
	//-------------------------------------------------------------------------------------------------------
	void ClassRegister::ClassConstant(double v, const string& name)
	{
		m_ConstantArray.push_back(make_pair(name, NEW Double(v)));
	}
}