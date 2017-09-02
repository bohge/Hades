#include "ThulacFunction.h"
#include "AllRegister.hpp"
#include "thulac.h"


namespace htf
{
	RTTI_TYPE_IMPLEMENT(ThulacFunction);
	SYSTEM_SINGLETON_IMPLEMENT(ThulacFunction);
	//---------------------------------------------------------------------------------------------------------
	ThulacFunction::ThulacFunction()
		:m_pTHULACQueue(NULL)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	ThulacFunction::~ThulacFunction()
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//---------------------------------------------------------------------------------------------------------
	void ThulacFunction::Initialize()
	{
		{
			hc::IScriptSystem* sys = hc::IScriptSystem::Instance();
			Register_All("thulac", *sys);
		}
		m_pTHULACQueue = NEW THULACQueue;
	}
	//---------------------------------------------------------------------------------------------------------
	void ThulacFunction::Update()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void ThulacFunction::Exit()
	{
		THULAC* sp;
		while (m_pTHULACQueue->try_dequeue(sp))
		{
			SAFE_DELETE(sp);
		}
		SAFE_DELETE(m_pTHULACQueue);
	}
	//---------------------------------------------------------------------------------------------------------
	THULAC* ThulacFunction::_NewObject()
	{
		THULAC* sp = NEW THULAC;
		const char* userpat = m_UserPath.empty() ? NULL : m_UserPath.c_str();
		sp->init(m_ModelPath.c_str(), userpat, m_isSegOnly, m_isT2S, m_isUfilter);
		return sp;
	}
	//---------------------------------------------------------------------------------------------------------
	THULAC* ThulacFunction::_GetObject()
	{
		THULAC* sp;
		if (!m_pTHULACQueue->try_dequeue(sp))
		{
			sp = _NewObject();
		}
		return sp;
	}
	//---------------------------------------------------------------------------------------------------------
	void ThulacFunction::_FreeObject(THULAC* obj)
	{
		if (NULL != obj)
		{
			m_pTHULACQueue->enqueue(obj);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void ThulacFunction::InitializeThulac(const char* model_path, const char* user_path, int just_seg, int t2s, int ufilter, char separator)
	{
		m_ModelPath = model_path;
		m_UserPath = user_path;
		m_isSegOnly = just_seg;
		m_isT2S = t2s;
		m_isUfilter = ufilter;
		m_Separator = separator;
		for (int i = 0; i < TC_PREPAER_SIZE; ++ i)
		{
			THULAC* sp = _NewObject();
			_FreeObject(sp);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	eastl::string ThulacFunction::Split(const std::string& input)
	{
		THULAC_result res;
		THULAC* spliter = _GetObject();
		spliter->cut(input, res);
		std::string str = spliter->toString(res);
		_FreeObject(spliter);
		return eastl::string(str.c_str());
	}
}
