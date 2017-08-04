#pragma once
#include "thread/IThreadSystem.h"


namespace hpts
{
	class ThreadSystem : public hc::IThreadSystem
	{
	public:
		ThreadSystem(void);
		~ThreadSystem(void);
	private:
		virtual  hc::IThreadEntity* _DoThreadEntityFactory(hc::ThreadProperty::EntityType et) const;
		virtual void _DoRecycleBin(hc::IThreadEntity* te) const;
	public:
		virtual void Initialize();
		virtual void Exit();
	};
}
