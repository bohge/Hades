#include "services/IServicesSystem.h"
#include "thread/IThreadSystem.h"
#include "thread/IThreadMutex.h"
#include "timer/ITimerSystem.h"
#include "services/IAgent.h"



using namespace eastl;
namespace hc
{
	RTTI_TYPE_IMPLEMENT(ITimerSystem);
	SYSTEM_SINGLETON_IMPLEMENT(ITimerSystem);
	//---------------------------------------------------------------------------------------------------------
	ITimerSystem::ITimerSystem()
		: m_pTimerMutex(NULL)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	ITimerSystem::~ITimerSystem()
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//---------------------------------------------------------------------------------------------------------
	void ITimerSystem::Initialize()
	{
		m_pTimerMutex = static_cast<IThreadMutex*>(IThreadSystem::Instance()->
			ThreadEntityFactory(ThreadProperty::ET_MUTEX));
	}
	//---------------------------------------------------------------------------------------------------------
	void ITimerSystem::Exit()
	{
		IThreadSystem::Instance()->RecycleBin(m_pTimerMutex);
	}
	//---------------------------------------------------------------------------------------------------------
	void ITimerSystem::OnTimerCallback(uint64 gid)
	{
		m_pTimerMutex->Lock();
		UpdateMap::iterator it = m_UpdateMap.find(gid);
		if (m_UpdateMap.end() != it)
		{
			for (ServiceFunctionList::iterator sf = it->second->begin();
				it->second->end() != sf;)
			{
				if (!IServicesSystem::Instance()->hasSerivices(sf->first))
				{
					ServiceFunctionList::iterator was = sf;
					++sf;
					it->second->erase(was);
				}
				else
				{
					Protocol* ptc = IServicesSystem::Instance()->AllocProtocol();
					ptc->SetFunction(sf->second);
					IServicesSystem::Instance()->RemoteInvoke(sf->first, ptc);
					++sf;
				}
			}
		}
		m_pTimerMutex->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void ITimerSystem::RegisterTimer(hc::IAgent* agent, float sec, const eastl::string& function)
	{
		if (NULL != agent)
		{
			uint funcid = agent->GetFunctionIndex(function);
			uint64 servicesid = agent->GetGID();
			if (sec > 0.0001f)
			{
				uint freq = sec * 1000.0f;
				m_pTimerMutex->Lock();
				ServiceFunctionList*  sflist;
				UpdateMap::iterator it = m_UpdateMap.find(freq);
				if (m_UpdateMap.end() != it)
				{
					sflist = it->second;
				}
				else
				{
					sflist = NEW ServiceFunctionList;
					m_UpdateMap.insert(make_pair(freq, sflist));
					ITimerSystem::Instance()->
						RegisterTimer(sec, freq,
						MEMBER_FUNC_PTR(&ITimerSystem::OnTimerCallback));
				}
				sflist->push_back(make_pair(servicesid, funcid));
				m_pTimerMutex->Unlock();
			}
		}
	}
}