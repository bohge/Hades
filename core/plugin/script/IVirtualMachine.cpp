#include "serialize/ISerializeSystem.h"
#include "script/IVirtualMachine.h"
#include "script/IScriptInstance.h"
#include "serialize/ISerializer.h"
#include "script/ClassRegister.h"
#include "serialize/IDecoder.h"
#include "file/IFileSystem.h"
#include "keeper/Accessor.h"
#include "core/Configure.h"
#include "log/ILogSystem.h"

using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT(IVirtualMachine);
	//---------------------------------------------------------------------------------------------------------
	IVirtualMachine::IVirtualMachine(uint index)
		:m_MachineIndex(index)
		, m_pISerializer(NULL)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IVirtualMachine::~IVirtualMachine(void)
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void IVirtualMachine::Initialize()
	{
		m_pISerializer = ISerializeSystem::Instance()->SerializerFactory();
	}
	//---------------------------------------------------------------------------------------------------------
	void IVirtualMachine::Update()
	{
		SmartPtr<Syncdata> data;
		while (m_SynchronizeQueue.try_dequeue(data))
		{
			IDecoder* de = m_pISerializer->GetDecoderFromBuffer(data->pAccessor->GetBuffer());
			//IScriptInstance* instance = _GetCreateInstance(data->Instance);
			InstanceMap::iterator it = m_InstanceMap.find(data->uHashCode);
			if (m_InstanceMap.end() == it)
			{
				HADESLOG("Unkown instance hash %ud",data->uHashCode);
			}
			else
			{
				IScriptInstance* instance = it->second;
				instance->Deserialize(de);
				m_pISerializer->ClearDecoder();
			}
		}
		_DoUpdate();
	}
	//---------------------------------------------------------------------------------------------------------
	void IVirtualMachine::Exit()
	{
		FOR_EACH(InstanceMap, it, m_InstanceMap)
		{
			it->second->ReleaseAllInvoker();
			_DoReleaseScript(it->second);
		}
		m_InstanceMap.clear();
		ISerializeSystem::Instance()->RecycleBin(m_pISerializer);
	}
	//---------------------------------------------------------------------------------------------------------
	IScriptInstance* IVirtualMachine::RegisteInstance(int scriptdata, const eastl::string& name)
	{
		uint hash = Utility::HashCode(name);
		ASSERT(m_InstanceMap.find(hash) == m_InstanceMap.end());
		IScriptInstance* instance = _DoRegisteInstance(hash, scriptdata);
		m_InstanceMap.insert(make_pair(hash, instance));
		return instance;
	}
	//---------------------------------------------------------------------------------------------------------
	IScriptInstance* IVirtualMachine::RegisteInstance(int scriptdata)
	{
		ASSERT(m_InstanceMap.find(scriptdata) == m_InstanceMap.end());
		IScriptInstance* instance = _DoRegisteInstance(scriptdata, scriptdata);
		//m_InstanceMap.insert(make_pair(scriptdata, instance));
		return instance;
	}
	//---------------------------------------------------------------------------------------------------------
	void IVirtualMachine::ReferenceClass(ClassRegister* c)
	{
		uint hash = Utility::HashCode(c->GetClassTypeName());
		RegistedClassSet::iterator it = m_RegistedClassSet.find(hash);
		if (m_RegistedClassSet.end() == it)
		{
			m_RegistedClassSet.insert(hash);
			_DoReferenceClass(c);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	IScriptInstance* IVirtualMachine::LoadScript(const eastl::string& path)
	{
		uint hash = Utility::HashCode(path);
		ASSERT(m_InstanceMap.find(hash) == m_InstanceMap.end());
		IScriptInstance* instance = _DoLoadScript(hash, path);
		if (NULL != instance)
		{
			m_InstanceMap.insert(make_pair(hash, instance));
		}
		return instance;
	}
	//---------------------------------------------------------------------------------------------------------
	IScriptInstance* IVirtualMachine::_GetCreateInstance(const eastl::string& name)
	{
		IScriptInstance* instance = NULL;
		uint hash = Utility::HashCode(name);
		InstanceMap::iterator it = m_InstanceMap.find(hash);
		if (m_InstanceMap.end() != it)
		{
			instance = it->second;
		}
		else
		{
			instance = _DoCreateInstance(hash, name);
			m_InstanceMap.insert(make_pair(hash, instance));
		}
		return instance;
	}
	//---------------------------------------------------------------------------------------------------------
	void IVirtualMachine::ReleaseScript(IScriptInstance* instance)
	{
		if (instance)
		{
			ASSERT(m_MachineIndex == instance->GetMachineIndex());
			uint hash = instance->GetScriptHash();
			InstanceMap::iterator it = m_InstanceMap.find(hash);
			if (m_InstanceMap.end() != it)
			{
				m_InstanceMap.erase(it);
			}
			_DoReleaseScript(instance);
		}
	}
}