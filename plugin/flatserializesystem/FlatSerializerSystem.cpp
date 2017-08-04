#include "FlatSerializerSystem.h"
#include "serialize/ISerializer.h"
#include "FlatSerializer.h"


using namespace hc;
namespace hfbs
{
	//---------------------------------------------------------------------------------------------------------
	FlatSerializerSystem::FlatSerializerSystem()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	FlatSerializerSystem::~FlatSerializerSystem()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	hc::ISerializer* FlatSerializerSystem::SerializerFactory()
	{
		return NEW FlatSerializer;
	}
	//---------------------------------------------------------------------------------------------------------
	void FlatSerializerSystem::RecycleBin(hc::ISerializer* ser)
	{
		SAFE_DELETE(ser);
	}
}