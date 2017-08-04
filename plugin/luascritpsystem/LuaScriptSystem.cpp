#include "GetScriptArgumenFunctions.h"
#include "script/IFunctionInvoker.h"
#include "GetNativeResultFunction.h"
#include "script/IScriptInstance.h"
#include "script/ScriptProperty.h"
#include "LuaVirtualMachine.h"
#include "LuaScriptSystem.h"
#include "Scriptfilecache.h"
#include "LuaArguments.h"
#include "core/Types.h"


using namespace hc;
namespace hlss
{
	//---------------------------------------------------------------------------------------------------------
	LuaScriptSystem::LuaScriptSystem()
	{
		ASSERT(BOOL_RTTI_ID == Bool::SID());
		ASSERT(SHORT_RTTI_ID == Short::SID());
		ASSERT(USHORT_RTTI_ID == UShort::SID());
		ASSERT(INT_RTTI_ID == Int::SID());
		ASSERT(UINT_RTTI_ID == UInt::SID());
		ASSERT(INT64_RTTI_ID == Int64::SID());
		ASSERT(UINT64_RTTI_ID == UInt64::SID());
		ASSERT(FLOAT_RTTI_ID == Float::SID());
		ASSERT(DOUBLE_RTTI_ID == Double::SID());
		ASSERT(STRING_RTTI_ID == String::SID());
		ASSERT(BUFFER_RTTI_ID == Buffer::SID());
		ASSERT(FUNCTION_RTTI_ID == Function::SID());
		ASSERT(INSTANCE_RTTI_ID == Instance::SID());
		ASSERT(FLOATARRAY_RTTI_ID == FloatArray::SID());
		ASSERT(ARGUMENTS_RTTI_ID == IArguments::SID());
	}
	//---------------------------------------------------------------------------------------------------------
	LuaScriptSystem::~LuaScriptSystem()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::Initialize()
	{
		IScriptSystem::Initialize();
		Scriptfilecache::Create();
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::Exit()
	{
		Scriptfilecache::Destroy();
		IScriptSystem::Exit();
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IVirtualMachine* LuaScriptSystem::_DoVirtualMachineFactory(uint index)
	{
		return NEW LuaVirtualMachine(index);
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::_DoRecycleBin(hc::IVirtualMachine* vm)
	{
		SAFE_DELETE(vm);
	}
	//---------------------------------------------------------------------------------------------------------
	IScriptSystem::ArgumentFunction LuaScriptSystem::GetScriptArgumentFunction(const hc::Rtti* rtti) const
	{
		switch (rtti->GetTypeID())
		{
		case BOOL_RTTI_ID: return &GetScriptParameterBool;
		case SHORT_RTTI_ID: return &GetScriptParameterShort;
		case USHORT_RTTI_ID: return &GetScriptParameterUShort;
		case INT_RTTI_ID: return &GetScriptParameterInt;
		case UINT_RTTI_ID: return &GetScriptParameterUInt;
		case FLOAT_RTTI_ID: return &GetScriptParameterFloat;
		case DOUBLE_RTTI_ID: return &GetScriptParameterDouble;
		case STRING_RTTI_ID:return &GetScriptParameterString;
		case BUFFER_RTTI_ID: return &GetScriptParameterBuffer;
		case INT64_RTTI_ID: return &GetScriptParameterInt64;
		case UINT64_RTTI_ID: return &GetScriptParameterUInt64;
		case INSTANCE_RTTI_ID: return &GetScriptParameterInstance;
		case FUNCTION_RTTI_ID: return &GetScriptParameterFunction;
		case ARGUMENTS_RTTI_ID: return &GetScriptParameterArgumens;
		default: return &GetScriptParameterClass;
		}
		return NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	IScriptSystem::ResultFunction LuaScriptSystem::GetNativeResultValueFunction(const hc::Rtti* rtti) const
	{
		switch (rtti->GetTypeID())
		{
		case BOOL_RTTI_ID: return &GetNativeResultValueBool;
		case STRING_RTTI_ID:return &GetNativeResultValueString;
		case SHORT_RTTI_ID: return &GetNativeResultValueShort;
		case USHORT_RTTI_ID: return &GetNativeResultValueUShort;
		case INT_RTTI_ID: return &GetNativeResultValueInt;
		case UINT_RTTI_ID: return &GetNativeResultValueUInt;
		case FLOAT_RTTI_ID: return &GetNativeResultValueFloat;
		case DOUBLE_RTTI_ID: return &GetNativeResultValueDouble;
		case BUFFER_RTTI_ID: return &GetNativeResultValueBuffer;
		case INT64_RTTI_ID: return &GetNativeResultValueInt64;
		case UINT64_RTTI_ID: return &GetNativeResultValueUInt64;
		case INSTANCE_RTTI_ID: return &GetNativeResultValueInstance;
		case FLOATARRAY_RTTI_ID: return &GetNativeResultValueFloatArray;
		case FUNCTION_RTTI_ID: ASSERT(false && "can not be function type"); break;
		case ARGUMENTS_RTTI_ID:ASSERT(false && "can not be arguments type"); break;
		default: return &GetNativeResultValueClass;
		}
		return NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	IScriptSystem::ResultFunction LuaScriptSystem::GetNativeResultPushFunction(const hc::Rtti* rtti) const
	{
		switch (rtti->GetTypeID())
		{
		case BOOL_RTTI_ID: return &GetNativeResultPushBool;
		case STRING_RTTI_ID:return &GetNativeResultPushString;
		case SHORT_RTTI_ID: return &GetNativeResultPushShort;
		case USHORT_RTTI_ID: return &GetNativeResultPushUShort;
		case INT_RTTI_ID: return &GetNativeResultPushInt;
		case UINT_RTTI_ID: return &GetNativeResultPushUInt;
		case FLOAT_RTTI_ID: return &GetNativeResultPushFloat;
		case DOUBLE_RTTI_ID: return &GetNativeResultPushDouble;
		case BUFFER_RTTI_ID: return &GetNativeResultPushBuffer;
		case INT64_RTTI_ID: return &GetNativeResultPushInt64;
		case UINT64_RTTI_ID: return &GetNativeResultPushUInt64;
		case INSTANCE_RTTI_ID: return &GetNativeResultPushInstance;
		case FLOATARRAY_RTTI_ID: return &GetNativeResultPushFloatArray;
		case FUNCTION_RTTI_ID: ASSERT(false && "can not be function type"); break;
		case ARGUMENTS_RTTI_ID:ASSERT(false && "can not be arguments type"); break;
		default: return &GetNativeResultPushClass;
		}
		return NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	bool _TypeCompare(const hc::Rtti* a1, const hc::Rtti* a2)
	{
		if (
			(a1->GetTypeID() == Short::SID()
			|| a1->GetTypeID() == UShort::SID()
			|| a1->GetTypeID() == Int::SID()
			|| a1->GetTypeID() == UInt::SID()
			|| a1->GetTypeID() == Float::SID()
			|| a1->GetTypeID() == Double::SID())
			&&
			(a2->GetTypeID() == Short::SID()
			|| a2->GetTypeID() == UShort::SID()
			|| a2->GetTypeID() == Int::SID()
			|| a2->GetTypeID() == UInt::SID()
			|| a2->GetTypeID() == Float::SID()
			|| a2->GetTypeID() == Double::SID())
			)
		{
			return true;
		}
		return false;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IScriptSystem::TypeCompare LuaScriptSystem::GetTypeCompare()
	{
		return &_TypeCompare;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IArguments* LuaScriptSystem::_DoAllocArguments()
	{
		return NEW LuaArguments;
	}
	//---------------------------------------------------------------------------------------------------------
	void LuaScriptSystem::_DoFreeArguments(hc::IArguments* args)
	{
		SAFE_DELETE(args);
	}
}