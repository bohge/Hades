#pragma once
#include "Type.h"

namespace hc
{
	class HADES_CORE_API Buffer : public Object
	{//空类型，为脚本参数传递提供支持
		RTTI_ABSTRACT_TYPE_DECLEAR(Buffer)
	public:
		Buffer(void){};
		~Buffer(void){};
	public:
		virtual void CantCreate() const = 0;
	};
}