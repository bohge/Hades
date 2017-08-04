#include "log/ILogSystem.h"
#include "JsonConfigure.h"




using namespace eastl;
using namespace rapidjson;

namespace hjc
{
	//---------------------------------------------------------------------------------------------------------
	JsonConfigure::JsonConfigure(void)
		:m_pDocument( NEW Document )
	{
	}
	//---------------------------------------------------------------------------------------------------------
	JsonConfigure::~JsonConfigure(void)
	{
		SAFE_DELETE(m_pDocument);
	}
	//---------------------------------------------------------------------------------------------------------
	bool JsonConfigure::Parse( const eastl::string& doc )
	{
		m_pDocument->Parse( doc.c_str() );
		return !m_pDocument->HasParseError();
	}
	//---------------------------------------------------------------------------------------------------------
	int JsonConfigure::GetInt( const eastl::string& name )
	{
		int res = 0;
		if (m_pDocument->HasMember(name.c_str()))
		{
			const Value& nodeValue = (*m_pDocument)[name.c_str()];
			res = nodeValue.GetInt();
		}
		else
		{
			HADESERROR("Unkown json member %s", name.c_str());
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	bool JsonConfigure::GetBool( const eastl::string& name )
	{
		bool res = false;
		if (m_pDocument->HasMember(name.c_str()))
		{
			const Value& nodeValue = (*m_pDocument)[name.c_str()];
			res = nodeValue.GetBool();
		}
		else
		{
			HADESERROR("Unkown json member %s", name.c_str());
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	double JsonConfigure::GetDouble( const eastl::string& name )
	{
		double res = 0.0;
		if (m_pDocument->HasMember(name.c_str()))
		{
			const Value& nodeValue = (*m_pDocument)[name.c_str()];
			res = nodeValue.GetDouble();
		}
		else
		{
			HADESERROR("Unkown json member %s", name.c_str());
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	string JsonConfigure::GetString( const eastl::string& name )
	{
		string res;
		if (m_pDocument->HasMember(name.c_str()))
		{
			const Value& nodeValue = (*m_pDocument)[name.c_str()];
			res = nodeValue.GetString();
		}
		else
		{
			HADESERROR("Unkown json member %s", name.c_str());
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	vector<int> JsonConfigure::GetArrayInt( const eastl::string& name )
	{
		vector<int> res;
		if (m_pDocument->HasMember(name.c_str()))
		{
			const Value& arraynode = (*m_pDocument)[name.c_str()];
			int nCount = arraynode.Size();
			res.reserve(nCount);
			for (int i = 0; i < nCount; ++i)
			{
				res.push_back(arraynode[i].GetInt());
			}
		}
		else
		{
			HADESERROR("Unkown json member %s", name.c_str());
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	vector<bool> JsonConfigure::GetArrayBool( const eastl::string& name )
	{
		vector<bool> res;
		if (m_pDocument->HasMember(name.c_str()))
		{
			const Value& arraynode = (*m_pDocument)[name.c_str()];
			int nCount = arraynode.Size();
			res.reserve(nCount);
			for (int i = 0; i < nCount; ++i)
			{
				res.push_back(arraynode[i].GetBool());
			}
		}
		else
		{
			HADESERROR("Unkown json member %s", name.c_str());
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	vector<double> JsonConfigure::GetArrayDouble( const eastl::string& name )
	{
		vector<double> res;
		if (m_pDocument->HasMember(name.c_str()))
		{
			const Value& arraynode = (*m_pDocument)[name.c_str()];
			int nCount = arraynode.Size();
			res.reserve(nCount);
			for (int i = 0; i < nCount; ++i)
			{
				res.push_back(arraynode[i].GetDouble());
			}
		}
		else
		{
			HADESERROR("Unkown json member %s", name.c_str());
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	vector<string> JsonConfigure::GetArrayString( const eastl::string& name )
	{
		vector<string> res;
		if (m_pDocument->HasMember(name.c_str()))
		{
			const Value& arraynode = (*m_pDocument)[name.c_str()];
			int nCount = arraynode.Size();
			res.reserve(nCount);
			for (int i = 0; i < nCount; ++i)
			{
				res.push_back(arraynode[i].GetString());
			}
		}
		else
		{
			HADESERROR("Unkown json member %s", name.c_str());
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	bool JsonConfigure::HasMember(const eastl::string& name)
	{
		return m_pDocument->HasMember(name.c_str());
	}
}