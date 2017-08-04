#include "serialize/ISerializeSystem.h"
#include "script/ScriptObjectpool.h"
#include "script/IVirtualMachine.h"
#include "serialize/ISerializer.h"
#include "script/IScriptSystem.h"
#include "thread/IThreadSystem.h"
#include "thread/IThreadMutex.h"
#include "serialize/IEncoder.h"
#include "script/IArguments.h"
#include "keeper/Accessor.h"
#include "core/SmartPtr.hpp"
#include "core/Configure.h"


#include "script/warp/ScriptRegister.hpp"



using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT(Context);
	RTTI_TYPE_IMPLEMENT(NativeUtility);
	RTTI_TYPE_IMPLEMENT(IScriptSystem);
	SYSTEM_SINGLETON_IMPLEMENT(IScriptSystem);
	//---------------------------------------------------------------------------------------------------------
	IScriptSystem::IScriptSystem(void)
		:m_pIThreadMutex(NULL)
		, m_pMachineArray(NULL)
		, m_pIArgumentsQueue(NULL)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	IScriptSystem::~IScriptSystem(void)
	{
		SYSTEM_SINGLETON_DESTROY;
		ASSERT(NULL == m_pMachineArray);
	}
	//---------------------------------------------------------------------------------------------------------
	void IScriptSystem::Initialize()
	{
		m_pIArgumentsQueue = NEW IArgumentsQueue;
		m_pMachineArray = NEW MachineArray;
		ScriptObjectpool::Create();
		m_pClassRegisterGlobalMap = NEW ClassRegisterGlobalMap;
		m_pIThreadMutex = static_cast<IThreadMutex*>(IThreadSystem::Instance()->
			ThreadEntityFactory(ThreadProperty::ET_MUTEX));		
		Register_System(*this);
	}
	//---------------------------------------------------------------------------------------------------------
	void IScriptSystem::Update()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void IScriptSystem::Exit()
	{
		IArguments* arg;
		while (m_pIArgumentsQueue->try_dequeue(arg))
		{
			_DoFreeArguments(arg);
		}
		FOR_EACH(ClassRegisterGlobalMap, it, *m_pClassRegisterGlobalMap)
		{
			FOR_EACH(ClassRegisterList, clr, it->second->ClassList)
			{
				SAFE_DELETE(*clr);
			}
			SAFE_DELETE(it->second);
		}
		SAFE_DELETE(m_pIArgumentsQueue);
		SAFE_DELETE(m_pMachineArray);
		SAFE_DELETE(m_pClassRegisterGlobalMap);
		IThreadSystem::Instance()->RecycleBin(m_pIThreadMutex);
		ScriptObjectpool::Destroy();
	}
	//---------------------------------------------------------------------------------------------------------
	IVirtualMachine* IScriptSystem::VirtualMachineFactory()
	{
		uint index = m_pMachineArray->size();
		IVirtualMachine* vm = _DoVirtualMachineFactory(index);
		vm->Initialize();
		vm->AddScriptPath(Configure::FILE_PATH_SCRIPT);
		vm->AddScriptPath(Configure::FILE_PATH_COMMON_SCRIPT);
		vm->AddNativePath(Configure::FILE_PATH_DEPENDENCY);
		vm->SetGlobal(ScriptProperty::SCRIPT_TYPE_BOOL, SCRIPT_TYPE_BOOL_VALUE);
		vm->SetGlobal(ScriptProperty::SCRIPT_TYPE_SHORT, SCRIPT_TYPE_SHORT_VALUE);
		vm->SetGlobal(ScriptProperty::SCRIPT_TYPE_USHORT, SCRIPT_TYPE_USHORT_VALUE);
		vm->SetGlobal(ScriptProperty::SCRIPT_TYPE_INT, SCRIPT_TYPE_INT_VALUE);
		vm->SetGlobal(ScriptProperty::SCRIPT_TYPE_UINT, SCRIPT_TYPE_UINT_VALUE);
		vm->SetGlobal(ScriptProperty::SCRIPT_TYPE_INT64, SCRIPT_TYPE_INT64_VALUE);
		vm->SetGlobal(ScriptProperty::SCRIPT_TYPE_UINT64, SCRIPT_TYPE_UINT64_VALUE);
		vm->SetGlobal(ScriptProperty::SCRIPT_TYPE_FLOAT, SCRIPT_TYPE_FLOAT_VALUE);
		vm->SetGlobal(ScriptProperty::SCRIPT_TYPE_DOUBLE, SCRIPT_TYPE_DOUBLE_VALUE);
		vm->SetGlobal(ScriptProperty::SCRIPT_TYPE_STRING, SCRIPT_TYPE_STRING_VALUE);
		vm->SetGlobal(ScriptProperty::SCRIPT_TYPE_NATIVE, SCRIPT_TYPE_NATIVE_VALUE);
		m_pIThreadMutex->Lock();
		m_pMachineArray->push_back(vm);
		m_pIThreadMutex->Unlock();
		return vm;
	}
	//---------------------------------------------------------------------------------------------------------
	void IScriptSystem::RecycleBin(IVirtualMachine* vm)
	{
		vm->Exit();
		m_pIThreadMutex->Lock();
		uint index = vm->GetIndex();
		(*m_pMachineArray)[index] = NULL;
		m_pIThreadMutex->Unlock();
		_DoRecycleBin(vm);
	}
	//---------------------------------------------------------------------------------------------------------
	void IScriptSystem::Synchronize(IVirtualMachine* from, IScriptInstance* instance)
	{
		ISerializer* pser = ISerializeSystem::Instance()->SerializerFactory();
		IEncoder* en = pser->GetEncoder();
		instance->Serialize(en);
		SmartPtr<Syncdata> data = SmartPtr<Syncdata>(NEW Syncdata);
		data->uHashCode = instance->GetScriptHash();
		data->pAccessor->CopyFrom(pser->GetBuffer(), pser->GetBufferSize());
		pser->ClearEncoder();
		ISerializeSystem::Instance()->RecycleBin(pser);
		FOR_EACH(MachineArray, vmit, *m_pMachineArray)
		{
			if (NULL != *vmit)
			{
				if ((*vmit)->GetIndex() != from->GetIndex())
				{
					(*vmit)->Synchronize(data);
				}
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	IArguments* IScriptSystem::AllocArguments()
	{
		IArguments* arg;
		if (!m_pIArgumentsQueue->try_dequeue(arg))
		{
			arg = _DoAllocArguments();
		}
		return arg;
	}
	//---------------------------------------------------------------------------------------------------------
	void IScriptSystem::FreeArguments(IArguments* args)
	{
		if (NULL != args)
		{
			args->Reset();
			m_pIArgumentsQueue->enqueue(args);
		}
	}	
	//---------------------------------------------------------------------------------------------------------
	void IScriptSystem::SetNativeLibrary(const eastl::string& systemname, const ClassRegister& clr)
	{
		uint hash = Utility::HashCode(systemname);
		ClassRegisterGlobal* crg = NULL;
		ClassRegisterGlobalMap::iterator it = m_pClassRegisterGlobalMap->find(hash);
		if (m_pClassRegisterGlobalMap->end() == it)
		{
			crg = NEW ClassRegisterGlobal;
			m_pClassRegisterGlobalMap->insert(make_pair(hash, crg));
		}
		else
		{
			crg = it->second;
		}
		ClassRegister* c = NEW ClassRegister(clr);
		crg->ClassList.push_back(c);
	}
	//---------------------------------------------------------------------------------------------------------
	ClassRegister* IScriptSystem::GetNativeLibraryClass(const eastl::string& systemname, const Rtti& rtti)
	{
		ClassRegister* res = NULL;
		uint hash = Utility::HashCode(systemname);		
		ClassRegisterGlobalMap::iterator it = m_pClassRegisterGlobalMap->find(hash);
		if (m_pClassRegisterGlobalMap->end() != it)
		{
			ClassRegisterGlobal* crg = it->second;
			FOR_EACH(ClassRegisterList, clr, crg->ClassList)
			{
				if ((*clr)->isTypeof(rtti))
				{
					res = (*clr);
					break;
				}
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void IScriptSystem::SetNativeGlobal(const eastl::string& systemname, const eastl::string& objname, Object* obj)
	{
		uint hash = Utility::HashCode(systemname);
		ClassRegisterGlobal* crg = NULL;
		ClassRegisterGlobalMap::iterator it = m_pClassRegisterGlobalMap->find(hash);
		if (m_pClassRegisterGlobalMap->end() == it)
		{
			crg = NEW ClassRegisterGlobal;
			m_pClassRegisterGlobalMap->insert(make_pair(hash, crg));
		}
		else
		{
			crg = it->second;
		}
		crg->GlobalList.push_back(eastl::make_pair(objname, obj));
	}
	//---------------------------------------------------------------------------------------------------------
	const IScriptSystem::ClassRegisterGlobal* IScriptSystem::GetNativeLibrary(const eastl::string& name)
	{
		ClassRegisterGlobal* res = NULL;
		uint hash = Utility::HashCode(name);
		ClassRegisterGlobalMap::iterator it = m_pClassRegisterGlobalMap->find(hash);
		if (m_pClassRegisterGlobalMap->end() != it)
		{
			res = it->second;
		}
		return res;
	}
}