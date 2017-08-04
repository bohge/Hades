#pragma once
#include "configure/IConfigure.h"


#include <rapidjson/document.h>     // rapidjson's DOM-style API


namespace hjc
{
	class JsonConfigure : public hc::IConfigure
	{
	private:
		 rapidjson::Document* m_pDocument;
	public:
		JsonConfigure(void);
		virtual ~JsonConfigure(void);
	public:
		virtual bool HasMember(const eastl::string& name);
		virtual int GetInt( const eastl::string& name );
		virtual bool GetBool( const eastl::string& name );
		virtual double GetDouble( const eastl::string& name );
		virtual eastl::string GetString( const eastl::string& name );
		virtual eastl::vector<int> GetArrayInt( const eastl::string& name );
		virtual eastl::vector<bool> GetArrayBool( const eastl::string& name );
		virtual eastl::vector<double> GetArrayDouble( const eastl::string& name );
		virtual eastl::vector<eastl::string> GetArrayString( const eastl::string& name );
	public:
		bool Parse( const eastl::string&doc );
	};
}