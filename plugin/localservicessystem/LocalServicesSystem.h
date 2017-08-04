#pragma once
#include "services/IServicesSystem.h"



namespace hlss
{
	class LocalServicesSystem : public hc::IServicesSystem
	{
	public:
		LocalServicesSystem();
		virtual ~LocalServicesSystem();
	private:
		virtual hc::SmartPtr<hc::IServices> _OnServiceFactory(const eastl::string& file, hc::ScriptServices* ss);
		virtual hc::ISession* _DoAllocSession();
		virtual void _DoFreeSession(hc::ISession* ss);
		virtual void _OnServicesStart();
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	public:
		virtual hc::IResumer* AllocResumer(hc::IVirtualMachine* vm);
	};
}
