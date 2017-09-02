#include "ThulacFunction.h"


namespace htf
{
	RTTI_TYPE_IMPLEMENT(ThulacFunction);
	SYSTEM_SINGLETON_IMPLEMENT(ThulacFunction);
	//---------------------------------------------------------------------------------------------------------
	ThulacFunction::ThulacFunction()
		:m_pTHULACQueue(NULL)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	ThulacFunction::~ThulacFunction()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void ThulacFunction::Initialize()
	{
		m_pTHULACQueue = NEW THULACQueue;
	}
	//---------------------------------------------------------------------------------------------------------
	void ThulacFunction::Update()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void ThulacFunction::Exit()
	{
		SAFE_DELETE(m_pTHULACQueue);
	}
	//---------------------------------------------------------------------------------------------------------
	THULAC* ThulacFunction::_GetObject()
	{
		THULAC* sp;
		if (!m_pTHULACQueue->try_dequeue(sp))
		{
			//sp = NEW THULAC;
			//sp->init(m_ModelPath.c_str(), m_UserPath.c_str(), m_isSegOnly, m_isT2S, m_isUfilter);
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
	void std::string ThulacFunction::Split(const std::string& input)
	{

	}

}
