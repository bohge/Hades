#pragma once
#include "core/Object.hpp"

namespace hc{class IAgent;}
namespace hsna
{
	struct RequestHandle
	{
		hc::IAgent*		pAgent;
		uint			OnRequestFunction;
		RequestHandle(void);
		~RequestHandle(void);
	};
}