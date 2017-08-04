#pragma once
#include "core/Object.hpp"
#include "core/Rtti.hpp"


#include <EASTL/string.h>
#include <EASTL/vector.h>

namespace hc
{
	//基本类型
	class HADES_CORE_API IConfigure : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IConfigure);
	public:
		IConfigure(void){};
		virtual ~IConfigure(void){};
	public:
		virtual bool HasMember(const eastl::string& name) = 0;
		virtual int GetInt(const eastl::string& name) = 0;
		virtual bool GetBool(const eastl::string& name) = 0;
		virtual double GetDouble(const eastl::string& name) = 0;
		virtual eastl::string GetString(const eastl::string& name) = 0;
		virtual eastl::vector<int> GetArrayInt(const eastl::string& name) = 0;
		virtual eastl::vector<bool> GetArrayBool(const eastl::string& name) = 0;
		virtual eastl::vector<double> GetArrayDouble(const eastl::string& name) = 0;
		virtual eastl::vector<eastl::string> GetArrayString(const eastl::string& name) = 0;
	};
}