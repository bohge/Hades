#include "serialize/ISerializeSystem.h"
#include "script/IVirtualMachine.h"
#include "LocalServicesSystem.h"
#include "LocalServices.h"
#include "LocalSession.h"
#include "LocalResumer.h"
#include "LocalAgent.h"
#include "ClassCache.h"

#include "LocalResumerWarp.hpp"


using namespace hc;
namespace hlss
{
	void ScriptServicesRecycle(void* obj)
	{
		ScriptServices* ss = static_cast<ScriptServices*>(obj);
		IServicesSystem::Instance()->RecycleScriptService(ss);
	}

	//---------------------------------------------------------------------------------------------------------
	LocalServicesSystem::LocalServicesSystem()
	{		
	}
	//---------------------------------------------------------------------------------------------------------
	LocalServicesSystem::~LocalServicesSystem()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void LocalServicesSystem::Initialize()
	{
		ISerializeSystem::Instance()->RegisterSerializable<LocalAgent>();
		ISerializeSystem::Instance()->RegisterSerializable<LocalResumer>();
		Register_LocalResumer(hc::Configure::NATIVE_LIBRARY_SYSTEM, *IScriptSystem::Instance());
		IServicesSystem::Initialize();
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalServicesSystem::Update()
	{
		IServicesSystem::Update();
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalServicesSystem::Exit()
	{
		IServicesSystem::Exit();
		ClassCache::Destroy();
	}
	//---------------------------------------------------------------------------------------------------------
	hc::SmartPtr<hc::IServices> LocalServicesSystem::_OnServiceFactory(const eastl::string& file, hc::ScriptServices* ss)
	{
		SmartPtr<LocalServices> res = SmartPtr<LocalServices>(NEW LocalServices);
		SmartPtr<ScriptServices> sss = SmartPtr<ScriptServices>(ss, &ScriptServicesRecycle);
		res->SetScriptServices(sss);
		const ClassCache::InfoPair* info = ClassCache::Instance()->GetClassCached(file, ss->GetScriptInstance());
		sss->Initialize(info->second);
		sss->SetSelfHandle(sss);
		res->SetAgentRtti(info->first);
		res->SetClassInfomation(info->second);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	hc::ISession* LocalServicesSystem::_DoAllocSession()
	{
		return NEW LocalSession;
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalServicesSystem::_DoFreeSession(hc::ISession* ss)
	{
		SAFE_DELETE(ss);
	}
	//---------------------------------------------------------------------------------------------------------
	void LocalServicesSystem::_OnServicesStart()
	{
		ClassCache::Create();
	}
	//---------------------------------------------------------------------------------------------------------
	typedef eastl::pair<uint64, hc::ISession*> MachineUserData;
	hc::IResumer* LocalServicesSystem::AllocResumer(hc::IVirtualMachine* vm)
	{
		LocalResumer* rr = static_cast<LocalResumer*>
			(ISerializeSystem::Instance()
			->GetSerializable(IResumer::RTTI().GetFactor()));
		MachineUserData* md =
			static_cast<MachineUserData*>(vm->GetUserData());
		rr->Setup(md->first, md->second->GetSessionID());
		return rr;
	}
}