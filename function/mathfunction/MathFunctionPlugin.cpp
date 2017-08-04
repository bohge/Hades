#include "serialize/ISerializeSystem.h"
#include "script/IScriptSystem.h"
#include "MathFunctionPlugin.h"
#include "AllRegister.hpp"


namespace hmfp
{
	RTTI_TYPE_IMPLEMENT(MathFunctionPlugin);
	//---------------------------------------------------------------------------------------------------------
	MathFunctionPlugin::MathFunctionPlugin()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	MathFunctionPlugin::~MathFunctionPlugin()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void MathFunctionPlugin::Initialize()
	{
		{
			hc::IScriptSystem* sys = hc::IScriptSystem::Instance();
			Register_All("3dmath", *sys);
		}
		{
			hc::ISerializeSystem* sys = hc::ISerializeSystem::Instance();
			sys->RegisterSerializable<vector2f>();
			sys->RegisterSerializable<vector3f>();
			sys->RegisterSerializable<vector4f>();
			sys->RegisterSerializable<Quaternionf>();
			sys->RegisterSerializable<Matrix33f>();
			sys->RegisterSerializable<Matrix44f>();
			sys->RegisterSerializable<Rayf>();
			sys->RegisterSerializable<Planef>();
			sys->RegisterSerializable<Aabbox3df>();
		}		
	}
	//---------------------------------------------------------------------------------------------------------
	void MathFunctionPlugin::Update()
	{

	}
	//---------------------------------------------------------------------------------------------------------
	void MathFunctionPlugin::Exit()
	{

	}

}
