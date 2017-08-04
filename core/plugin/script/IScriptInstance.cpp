#include "serialize/ISerializeSystem.h"
#include "script/ICoroutineInvoker.h"
#include "script/IFunctionIterator.h"
#include "script/IFunctionInvoker.h"
#include "serialize/ISerializable.h"
#include "script/IScriptInstance.h"
#include "script/IMemberIterator.h"
#include "serialize/ISerializer.h"
#include "serialize/IDecoder.h"
#include "serialize/IEncoder.h"
#include "core/Utility.hpp"
#include "SaveInstance.hpp"
#include "SetInstance.hpp"
#include "core/Types.h"



using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT(IScriptInstance);
	//------------------------------------------------------------------------------------------------------
	IScriptInstance::IScriptInstance(uint host, uint hash)
		:m_HostMachineIndex(host)
		, m_nHashCode(hash)
	{
	}
	//------------------------------------------------------------------------------------------------------
	IScriptInstance::~IScriptInstance(void)
	{
		ASSERT(0 == m_FunctionMap.size());
	}
	//------------------------------------------------------------------------------------------------------
	void IScriptInstance::ReleaseAllInvoker()
	{
		FOR_EACH(FuncMap, func, m_FunctionMap)
		{
			_DoReleaseFunction(func->second->m_pInvoker);
			SAFE_DELETE(func->second);
		}
		m_FunctionMap.clear();

		FOR_EACH(CorouMap, corou, m_CoroutineMap)
		{
			_DoReleaseCoroutine(corou->second);
		}
		m_FunctionMap.clear();
	}
	//------------------------------------------------------------------------------------------------------
	bool IScriptInstance::_isFunctionExist(const eastl::string& name)
	{
		bool res = false;
		for (const IFunctionIterator* fit = &GetFunctionIterator();
			NULL != fit;
			fit = fit->GetBaseFunctionIterator())
		{
			if (fit->isFunctionExist(name))
			{
				res = true;
				break;
			}
		}
		return res;
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::_DoTryGetFunction(const eastl::string& name, const ScriptProperty::Arguments& args)
	{
		IFunctionInvoker* func = NULL;
		uint nameHash = Utility::HashCode(name);
		FuncMap::iterator funcIt = m_FunctionMap.find(nameHash);
		if (m_FunctionMap.end() != funcIt)
		{
			++funcIt->second->m_User;
			func = funcIt->second->m_pInvoker;
		}
		if (NULL == func)
			//&& _isFunctionExist(name))
		{
			func = _DoSignFunciton(name, args);
			func->SetFunctionID(nameHash);
			m_FunctionMap.insert(make_pair(nameHash, NEW UserFunction(func)));
		}
		return func;
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::_DoTryGetFunction(int userdata, const ScriptProperty::Arguments& args)
	{
		IFunctionInvoker* func = NULL;
		FuncMap::iterator funcIt = m_FunctionMap.find((uint)userdata);
		if (m_FunctionMap.end() != funcIt)
		{
			++funcIt->second->m_User;
			func = funcIt->second->m_pInvoker;
		}
		else
		{
			func = _DoSignFunciton(userdata, args);
			func->SetFunctionID((uint)userdata);
			m_FunctionMap.insert(make_pair((uint)userdata, NEW UserFunction(func)));
		}
		return func;
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::_DoTryGetFunction(const eastl::string& name, uint res, const ScriptProperty::Arguments& args)
	{
		IFunctionInvoker* func = NULL;
		uint nameHash = Utility::HashCode(name);
		FuncMap::iterator funcIt = m_FunctionMap.find(nameHash);
		if (m_FunctionMap.end() != funcIt)
		{
			++funcIt->second->m_User;
			func = funcIt->second->m_pInvoker;
		}
		if (NULL == func)
			//&& _isFunctionExist(name))
		{
			func = _DoSignFunciton(name, res, args);
			func->SetFunctionID(nameHash);
			m_FunctionMap.insert(make_pair(nameHash, NEW UserFunction(func)));
		}
		return func;
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::_DoTryGetFunction(int userdata, uint res, const ScriptProperty::Arguments& args)
	{
		IFunctionInvoker* func = NULL;
		FuncMap::iterator funcIt = m_FunctionMap.find((uint)userdata);
		if (m_FunctionMap.end() != funcIt)
		{
			++funcIt->second->m_User;
			func = funcIt->second->m_pInvoker;
		}
		else
		{
			func = _DoSignFunciton(userdata, res, args);
			func->SetFunctionID((uint)userdata);
			m_FunctionMap.insert(make_pair((uint)userdata, NEW UserFunction(func)));
		}
		return func;
	}
	//------------------------------------------------------------------------------------------------------
	void IScriptInstance::_SetClassName(const eastl::string& name)
	{
		m_ClassName = name;
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFunciton(const eastl::string& name)
	{
		ScriptProperty::Arguments args;
		return _DoTryGetFunction(name, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFunciton(const eastl::string& name, const uint arg1)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		return _DoTryGetFunction(name, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFunciton(const eastl::string& name, const uint arg1, const uint arg2)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		args.push_back(arg2);
		return _DoTryGetFunction(name, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFunciton(const eastl::string& name, const uint arg1, const uint arg2, const uint arg3)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		args.push_back(arg2);
		args.push_back(arg3);
		return _DoTryGetFunction(name, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFunciton(const eastl::string& name, const uint arg1, const uint arg2, const uint arg3, const uint arg4)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		args.push_back(arg2);
		args.push_back(arg3);
		args.push_back(arg4);
		return _DoTryGetFunction(name, args);
	}
	//------------------------------------------------------------------------------------------------------
	uint SIDMapping(int input)
	{
		switch (input)
		{
		case SCRIPT_TYPE_BOOL_VALUE: return Bool::SID();
		case SCRIPT_TYPE_SHORT_VALUE: return Short::SID();
		case SCRIPT_TYPE_USHORT_VALUE: return UShort::SID();
		case SCRIPT_TYPE_INT_VALUE: return Int::SID();
		case SCRIPT_TYPE_UINT_VALUE: return UInt::SID();
		case SCRIPT_TYPE_FLOAT_VALUE: return Float::SID();
		case SCRIPT_TYPE_DOUBLE_VALUE: return Double::SID();
		case SCRIPT_TYPE_STRING_VALUE: return String::SID();
		case SCRIPT_TYPE_NATIVE_VALUE: return Object::SID();
		case SCRIPT_TYPE_INT64_VALUE: return Int64::SID();
		case SCRIPT_TYPE_UINT64_VALUE: return UInt64::SID();
		default:ASSERT(false);
		}
		return nullhandle;
	}
	IFunctionInvoker* IScriptInstance::SignFunciton(int userdata)
	{
		ScriptProperty::Arguments args;
		return _DoTryGetFunction(userdata, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFunciton(int userdata, int arg1)
	{
		ScriptProperty::Arguments args;
		args.push_back(SIDMapping(arg1));
		return _DoTryGetFunction(userdata, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFunciton(int userdata, int arg1, int arg2)
	{
		ScriptProperty::Arguments args;
		args.push_back(SIDMapping(arg1));
		args.push_back(SIDMapping(arg2));
		return _DoTryGetFunction(userdata, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFunciton(int userdata, int arg1, int arg2, int arg3)
	{
		ScriptProperty::Arguments args;
		args.push_back(SIDMapping(arg1));
		args.push_back(SIDMapping(arg2));
		args.push_back(SIDMapping(arg3));
		return _DoTryGetFunction(userdata, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFunciton(int userdata, int arg1, int arg2, int arg3, int arg4)
	{
		ScriptProperty::Arguments args;
		args.push_back(SIDMapping(arg1));
		args.push_back(SIDMapping(arg2));
		args.push_back(SIDMapping(arg3));
		args.push_back(SIDMapping(arg4));
		return _DoTryGetFunction(userdata, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFuncitonWithReturn(uint res, const eastl::string& name)
	{
		ScriptProperty::Arguments args;
		return _DoTryGetFunction(name, res, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFuncitonWithReturn(uint res, const eastl::string& name, uint arg1)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		return _DoTryGetFunction(name, res, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFuncitonWithReturn(uint res, const eastl::string& name, uint arg1, uint arg2)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		args.push_back(arg2);
		return _DoTryGetFunction(name, res, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFuncitonWithReturn(uint res, const eastl::string& name, uint arg1, uint arg2, uint arg3)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		args.push_back(arg2);
		args.push_back(arg3);
		return _DoTryGetFunction(name, res, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFuncitonWithReturn(uint res, const eastl::string& name, uint arg1, uint arg2, uint arg3, uint arg4)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		args.push_back(arg2);
		args.push_back(arg3);
		args.push_back(arg4);
		return _DoTryGetFunction(name, res, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFuncitonWithReturn(int res, int userdata)
	{
		ScriptProperty::Arguments args;
		return _DoTryGetFunction(userdata, res, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFuncitonWithReturn(int res, int userdata, int arg1)
	{
		ScriptProperty::Arguments args;
		args.push_back(SIDMapping(arg1));
		return _DoTryGetFunction(userdata, res, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFuncitonWithReturn(int res, int userdata, int arg1, int arg2)
	{
		ScriptProperty::Arguments args;
		args.push_back(SIDMapping(arg1));
		args.push_back(SIDMapping(arg2));
		return _DoTryGetFunction(userdata, res, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFuncitonWithReturn(int res, int userdata, int arg1, int arg2, int arg3)
	{
		ScriptProperty::Arguments args;
		args.push_back(SIDMapping(arg1));
		args.push_back(SIDMapping(arg2));
		args.push_back(SIDMapping(arg3));
		return _DoTryGetFunction(userdata, res, args);
	}
	//------------------------------------------------------------------------------------------------------
	IFunctionInvoker* IScriptInstance::SignFuncitonWithReturn(int res, int userdata, int arg1, int arg2, int arg3, int arg4)
	{
		ScriptProperty::Arguments args;
		args.push_back(SIDMapping(arg1));
		args.push_back(SIDMapping(arg2));
		args.push_back(SIDMapping(arg3));
		args.push_back(SIDMapping(arg4));
		return _DoTryGetFunction(userdata, res, args);
	}
	//------------------------------------------------------------------------------------------------------
	void IScriptInstance::UnsignInvoker(IFunctionInvoker* func)
	{
		FuncMap::iterator funcIt = m_FunctionMap.find(func->GetFunctionID());
		ASSERT(m_FunctionMap.end() != funcIt);
		if (0 == --funcIt->second->m_User)
		{
			_DoReleaseFunction(funcIt->second->m_pInvoker);
			SAFE_DELETE(funcIt->second);
			m_FunctionMap.erase(funcIt);
		}
	}
	//------------------------------------------------------------------------------------------------------
	void IScriptInstance::UnsignInvoker(ICoroutineInvoker* func)
	{
		if (NULL != func)
		{
			CorouMap::iterator funcIt = m_CoroutineMap.find(func->GetObjectID());
			ASSERT(m_CoroutineMap.end() != funcIt);
			_DoReleaseCoroutine(funcIt->second);
			m_CoroutineMap.erase(funcIt);
		}
	}
	//------------------------------------------------------------------------------------------------------
	ICoroutineInvoker* IScriptInstance::_DoTryGetCoroutine(const eastl::string& name, const ScriptProperty::Arguments& args)
	{
		ICoroutineInvoker* func = NULL;
		//if (_isFunctionExist(name))
		{
			func = _DoSignCoroutine(name, args);
			m_CoroutineMap.insert(make_pair(func->GetObjectID(), func));
		}
		return func;
	}
	//------------------------------------------------------------------------------------------------------
	ICoroutineInvoker* IScriptInstance::_DoTryGetCoroutine(const eastl::string& name, uint res, const ScriptProperty::Arguments& args)
	{
		ICoroutineInvoker* func = NULL;
		//if (_isFunctionExist(name))
		{
			func = _DoSignCoroutine(name, res, args);
			m_CoroutineMap.insert(make_pair(func->GetObjectID(), func));
		}
		return func;
	}
	//------------------------------------------------------------------------------------------------------
	ICoroutineInvoker* IScriptInstance::SignCoroutine(const eastl::string& name)
	{
		ScriptProperty::Arguments args;
		return _DoTryGetCoroutine(name, args);
	}
	//------------------------------------------------------------------------------------------------------
	ICoroutineInvoker* IScriptInstance::SignCoroutine(const eastl::string& name, uint arg1)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		return _DoTryGetCoroutine(name, args);
	}
	//------------------------------------------------------------------------------------------------------
	ICoroutineInvoker* IScriptInstance::SignCoroutine(const eastl::string& name, uint arg1, uint arg2)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		args.push_back(arg2);
		return _DoTryGetCoroutine(name, args);
	}
	//------------------------------------------------------------------------------------------------------
	ICoroutineInvoker* IScriptInstance::SignCoroutine(const eastl::string& name, uint arg1, uint arg2, uint arg3)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		args.push_back(arg2);
		args.push_back(arg3);
		return _DoTryGetCoroutine(name, args);
	}
	//------------------------------------------------------------------------------------------------------
	ICoroutineInvoker* IScriptInstance::SignCoroutine(const eastl::string& name, uint arg1, uint arg2, uint arg3, uint arg4)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		args.push_back(arg2);
		args.push_back(arg3);
		args.push_back(arg4);
		return _DoTryGetCoroutine(name, args);
	}
	//------------------------------------------------------------------------------------------------------
	ICoroutineInvoker* IScriptInstance::SignCoroutine(uint res, const eastl::string& name)
	{
		ScriptProperty::Arguments args;
		return _DoTryGetCoroutine(name, res, args);
	}
	//------------------------------------------------------------------------------------------------------
	ICoroutineInvoker* IScriptInstance::SignCoroutine(uint res, const eastl::string& name, uint arg1)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		return _DoTryGetCoroutine(name, res, args);
	}
	//------------------------------------------------------------------------------------------------------
	ICoroutineInvoker* IScriptInstance::SignCoroutine(uint res, const eastl::string& name, uint arg1, uint arg2)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		args.push_back(arg2);
		return _DoTryGetCoroutine(name, res, args);
	}
	//------------------------------------------------------------------------------------------------------
	ICoroutineInvoker* IScriptInstance::SignCoroutine(uint res, const eastl::string& name, uint arg1, uint arg2, uint arg3)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		args.push_back(arg2);
		args.push_back(arg3);
		return _DoTryGetCoroutine(name, res, args);
	}
	//------------------------------------------------------------------------------------------------------
	ICoroutineInvoker* IScriptInstance::SignCoroutine(uint res, const eastl::string& name, uint arg1, uint arg2, uint arg3, uint arg4)
	{
		ScriptProperty::Arguments args;
		args.push_back(arg1);
		args.push_back(arg2);
		args.push_back(arg3);
		args.push_back(arg4);
		return _DoTryGetCoroutine(name, res, args);
	}
	//------------------------------------------------------------------------------------------------------
	void IScriptInstance::ClearMember()
	{
		_DoClearMember();
	}
	//------------------------------------------------------------------------------------------------------
	void IScriptInstance::Deserialize(IDecoder* de)
	{
		RecursiveSetInstance(this, de);
	}
	//------------------------------------------------------------------------------------------------------
	void IScriptInstance::Serialize(IEncoder* en)
	{
		const IMemberIterator& it = GetMemberIterator();
		RecursiveSaveInstance(it, en);
	}
}