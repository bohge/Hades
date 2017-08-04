#include "serialize/ISerializeSystem.h"
#include "IDIPMessageSystem.h"
#include "IDIPMessageWarp.hpp"
#include "IDIPRawbuffer.h"
#include "IDIPMessage.h"

using namespace hc;
namespace hms
{
	//---------------------------------------------------------------------------------------------------------
	IDIPMessageSystem::IDIPMessageSystem()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IDIPMessageSystem::~IDIPMessageSystem()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	IRawbuffer* IDIPMessageSystem::_DoRawbufferFactory() const
	{
		return NEW IDIPRawbuffer;
	}
	//---------------------------------------------------------------------------------------------------------
	void IDIPMessageSystem::_DoRecycleBin(IRawbuffer* rb) const
	{
		SAFE_DELETE(rb);
	}
	//---------------------------------------------------------------------------------------------------------
	void IDIPMessageSystem::Initialize()
	{
		Register_IDIPMessage(hc::Configure::NATIVE_LIBRARY_SYSTEM, *IScriptSystem::Instance());
		ISerializeSystem::Instance()->RegisterSerializable<IDIPMessage>();
	}
	//---------------------------------------------------------------------------------------------------------
	void IDIPMessageSystem::Update()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void IDIPMessageSystem::Exit()
	{

	}
}