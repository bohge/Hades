#pragma once
#include "core/Singleton.hpp"
#include "core/IPlugin.h"

#include "concurrentqueue.h"

#include <EASTL/map.h>

namespace hc
{
	class IArguments;
	class IThreadMutex;
	class ClassRegister;
	class IVirtualMachine;
	class IScriptInstance;
	class HADES_CORE_API IScriptSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(IScriptSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IScriptSystem, IPlugin);
	public:
		typedef bool(*TypeCompare)(const Rtti*, const Rtti*);
		typedef void(*ArgumentFunction)(void*&, void*, void*, void*);
		typedef void*(*ResultFunction)(void*, void*, void*);
	public:
		typedef eastl::list<ClassRegister*>	ClassRegisterList;
		typedef eastl::list<eastl::pair< eastl::string, Object*> > GlobalPairList;
	public:
		struct  ClassRegisterGlobal
		{
			ClassRegisterList	ClassList;
			GlobalPairList		GlobalList;
		};
	private:
		typedef moodycamel::ConcurrentQueue<IArguments*> IArgumentsQueue;
		typedef eastl::vector< IVirtualMachine* > MachineArray;
		typedef eastl::map< uint, ClassRegisterGlobal* > ClassRegisterGlobalMap;
	private:
		IArgumentsQueue*		m_pIArgumentsQueue;
		MachineArray*			m_pMachineArray;
		IThreadMutex*			m_pIThreadMutex;
		ClassRegisterGlobalMap*	m_pClassRegisterGlobalMap;
	public:
		IScriptSystem(void);
		virtual ~IScriptSystem(void);
	private:
		virtual IVirtualMachine* _DoVirtualMachineFactory(uint index) = 0;
		virtual void _DoRecycleBin(IVirtualMachine* vm) = 0;
		virtual IArguments* _DoAllocArguments() = 0;
		virtual void _DoFreeArguments(IArguments* args) = 0;
	public:
		virtual ArgumentFunction GetScriptArgumentFunction(const Rtti* rtti) const = 0;
		virtual ResultFunction GetNativeResultValueFunction(const Rtti* rtti) const = 0;
		virtual ResultFunction GetNativeResultPushFunction(const Rtti* rtti) const = 0;
		virtual TypeCompare GetTypeCompare() = 0;
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	public:
		IVirtualMachine* VirtualMachineFactory();
		void RecycleBin(IVirtualMachine* vm);
		void Synchronize(IVirtualMachine* from, IScriptInstance* instance);//将数据同步到其他虚拟机
		IArguments* AllocArguments();
		void FreeArguments(IArguments* args);
		void SetNativeLibrary(const eastl::string& systemname, const ClassRegister& clr);
		ClassRegister* GetNativeLibraryClass(const eastl::string& systemname, const Rtti& rtti);
		void SetNativeGlobal(const eastl::string& systemname, const eastl::string& objname, Object* obj);
		const ClassRegisterGlobal* GetNativeLibrary(const eastl::string& name);
	public:
		HADES_FORCEINLINE int GetVirtualMachineCount()const;
	};



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE int IScriptSystem::GetVirtualMachineCount() const
	{
		return m_pMachineArray->size();
	}
}
