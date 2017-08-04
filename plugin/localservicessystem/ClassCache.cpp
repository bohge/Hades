#include "script/ClassRegister.h"
#include "thread/IThreadSystem.h"
#include "thread/IThreadMutex.h"
#include "Serviceshelper.hpp"
#include "ClassCache.h"



using namespace hc;
using namespace eastl;
namespace hlss
{
	SINGLETON_IMPLEMENT(ClassCache);
	//---------------------------------------------------------------------------------------------------------
	ClassCache::ClassCache(void)
		:m_pIThreadMutex(NULL)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	ClassCache::~ClassCache(void)
	{		
	}
	//---------------------------------------------------------------------------------------------------------
	void ClassCache::_OnCreate()
	{
		m_pIThreadMutex = static_cast<IThreadMutex*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_MUTEX));
	}
	//---------------------------------------------------------------------------------------------------------
	void ClassCache::_OnDestroy()
	{
		FOR_EACH(ClassinfoMap, it, m_ClassinfoMap)
		{
			SAFE_DELETE(it->second->first);
			SAFE_DELETE(it->second->second);
			SAFE_DELETE(it->second);
		}
		IThreadSystem::Instance()->RecycleBin(m_pIThreadMutex);
	}
	//---------------------------------------------------------------------------------------------------------
	const ClassCache::InfoPair* ClassCache::_NewClassInfo(const eastl::string& file, IScriptInstance* instance)
	{
		InfoPair* res = NULL;
		uint hash = Utility::HashCode(file);
		if (NULL == instance)
		{
			ASSERT(false);
		}
		eastl::string classname = instance->GetClassName();
		classname += "_RPC_FIXED";
		res = NEW InfoPair;
		res->first = NEW Rtti(sizeof(IAgent), NULL, classname.c_str());
		res->second = NEW ClassRegister;
		const IFunctionIterator& fit = instance->GetFunctionIterator();
		IteratorFunction(res->second, &fit, IServicesSystem::Instance()->GetRPCOptimization());
		EquivalentFunction(res->second);
		res->second->ClassType(res->first);
		m_ClassinfoMap.insert(make_pair(hash, res));
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	const ClassCache::InfoPair* ClassCache::GetClassCached(const eastl::string& file, IScriptInstance* instance)
	{
		const InfoPair* res = NULL;
		uint hash = Utility::HashCode(file);
		m_pIThreadMutex->Lock();
		ClassinfoMap::iterator it = m_ClassinfoMap.find(hash);
		if (m_ClassinfoMap.end() != it)
		{
			res = it->second;
		}
		else
		{
			res = _NewClassInfo(file, instance);
		}
		m_pIThreadMutex->Unlock();
		return res;
	}
}