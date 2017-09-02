#pragma once
#include "network/server/IServerSystem.h"




namespace hles
{
	class LibeventServerSystem : public hc::IServerSystem
	{
	public:
		LibeventServerSystem(void);
		virtual ~LibeventServerSystem(void);
	private:
		virtual hc::IServer* _DoServerFactory(IServerSystem::ServerType st) const;
		virtual void _DoRecycleBin( hc::IServer* is ) const;
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	};
}
