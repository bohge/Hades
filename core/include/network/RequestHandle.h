#pragma once
#include "core/SmartPtr.hpp"
#include "core/Object.hpp"


#define CONNECTINDX 0
#define REQUESTINDX 1
#define DISCONNECTINDX 2
#define FUNCTIONCOUT 3
namespace hc
{
	class IAgent;
	class IServices;
	struct RequestHandle
	{
		IAgent*			pAgent;
		uint			Functions[FUNCTIONCOUT];
		RequestHandle(SmartPtr<IServices>& services, uint connect, uint requestfunc, uint disconnectfunc);
		~RequestHandle(void);
	};
}