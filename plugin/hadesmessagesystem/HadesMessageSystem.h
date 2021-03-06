#pragma once
#include "network/message/IMessageSystem.h"



namespace hms
{
	class HadesMessageSystem : public hc::IMessageSystem
	{
	public:
		HadesMessageSystem();
		virtual ~HadesMessageSystem();
	private:
		virtual hc::IRawbuffer* _DoRawbufferFactory() const;
		virtual void _DoRecycleBin(hc::IRawbuffer* rb) const;
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	};
}
