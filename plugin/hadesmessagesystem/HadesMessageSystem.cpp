#include "serialize/ISerializeSystem.h"
#include "HadesMessageSystem.h"
#include "HadesMessageWarp.hpp"
#include "HadesRawbuffer.h"
#include "HadesMessage.h"



using namespace hc;
namespace hms
{
	//---------------------------------------------------------------------------------------------------------
	HadesMessageSystem::HadesMessageSystem()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	HadesMessageSystem::~HadesMessageSystem()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IRawbuffer* HadesMessageSystem::_DoRawbufferFactory() const
	{
		return NEW HadesRawbuffer;
	}
	//---------------------------------------------------------------------------------------------------------
	void HadesMessageSystem::_DoRecycleBin(IRawbuffer* rb) const
	{
		SAFE_DELETE(rb);
	}
	//---------------------------------------------------------------------------------------------------------
	void HadesMessageSystem::Initialize()
	{
		Register_HadesMessage(hc::Configure::NATIVE_LIBRARY_SYSTEM, *IScriptSystem::Instance());
		ISerializeSystem::Instance()->RegisterSerializable<HadesMessage>();
	}
	//---------------------------------------------------------------------------------------------------------
	void HadesMessageSystem::Update()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void HadesMessageSystem::Exit()
	{

	}
}