#include "script/ScriptValue.h"



namespace hc
{
	//RTTI_TYPE_IMPLEMENT( ScriptValue );
	//-------------------------------------------------------------------------------------------------------
	ScriptValue::ScriptValue(void)
		:m_pValue(NULL),
		m_rpRecycle(NULL),
		m_fClear(NULL)		
	{

	}
	//-------------------------------------------------------------------------------------------------------
	ScriptValue::~ScriptValue(void)
	{
		Clear();
	}
	//-------------------------------------------------------------------------------------------------------
	void ScriptValue::SafeDeleteCleaner(void* rec, Object* obj)
	{
		SAFE_DELETE( obj );
	}
	//-------------------------------------------------------------------------------------------------------
	void ScriptValue::NullCleaner(void* rec, Object* obj)
	{

	}

}