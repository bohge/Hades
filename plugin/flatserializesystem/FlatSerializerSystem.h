#pragma once
#include "serialize/ISerializeSystem.h"


namespace hfbs
{
	class FlatSerializerSystem : public hc::ISerializeSystem
	{
	public:
		FlatSerializerSystem();
		virtual ~FlatSerializerSystem();
	public:
		virtual hc::ISerializer* SerializerFactory();
		virtual void RecycleBin(hc::ISerializer* ser);
	};
}
