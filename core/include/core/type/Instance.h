#pragma once
#include "Type.h"

namespace hc
{
	class HADES_CORE_API Instance : public Object
	{//空类型，为脚本参数传递提供支持
		RTTI_ABSTRACT_TYPE_DECLEAR(Instance)
	public:
		Instance(void){};
		~Instance(void){};
	public:
		virtual void CantCreate() const = 0;
	};
}