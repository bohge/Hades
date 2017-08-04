#include "QuaternionWrap.hpp"
#include "Matrix33Wrap.hpp"
#include "Matrix44Wrap.hpp"
#include "Vector2Wrap.hpp"
#include "Vector3Wrap.hpp"
#include "Vector4Wrap.hpp"
#include "AabboxWarp.h"
#include "RayWrap.h"



namespace hmfp
{
	//-------------------------------------------------------------------------------------------------------
	static void Register_All(const eastl::string& name, hc::IScriptSystem& sys)
	{
		Register_Matrix33(name, sys);
		Register_Matrix44(name, sys);
		Register_Quaternion(name, sys);
		Register_vector2(name, sys);
		Register_vector3(name, sys);
		Register_vector4(name, sys);
		Register_Aabbox(name, sys);
		Register_Ray(name, sys);
	}
}