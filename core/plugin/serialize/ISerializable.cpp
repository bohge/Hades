#include "serialize/ISerializeSystem.h"
#include "serialize/ISerializable.h"


namespace hc
{
	RTTI_TYPE_IMPLEMENT(ISerializable);
	//---------------------------------------------------------------------------------------------------------
	ISerializable::ISerializable()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	ISerializable::~ISerializable(void)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void ISerializable::OnRecycle()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	ScriptValue::ClearFunction ISerializable::GetClearFunction()
	{
		return &ISerializeSystem::SerializableCleaner;
	}
}