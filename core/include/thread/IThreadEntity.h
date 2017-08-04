#pragma once
#include "IThreadSystem.h"
#include "core/Object.hpp"


namespace hc
{
	class HADES_CORE_API IThreadEntity : public Object
	{
		RTTI_TYPE_DECLEAR( IThreadEntity )
	private:
		ThreadProperty::EntityType		m_EntityType;
	public:
		IThreadEntity( ThreadProperty::EntityType et ):m_EntityType(et){}
		virtual ~IThreadEntity(){}
	public:
		HADES_FORCEINLINE ThreadProperty::EntityType GetEntityType();
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE ThreadProperty::EntityType IThreadEntity::GetEntityType()
	{
		return m_EntityType;
	}
}