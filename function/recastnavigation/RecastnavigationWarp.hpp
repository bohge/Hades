#pragma once
#include "serialize/ISerializeSystem.h"
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "RecastnavigationPlugin.h"
#include "NavigationQuery.h"

namespace hrnf
{
	//-------------------------------------------------------------------------------------------------------
	static int Recastnavigation_LoadNavigationMesh(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Recastnavigation* self, int* node, hc::StringBuffer* str)
	{
		eastl::string sstr(static_cast<const char*>(str->pBuffer), str->Length);
		NavigationQuery* obj = self->LoadNavigationMesh(*node, sstr);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Recastnavigation_SetPickExtent(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Recastnavigation* self, vector3* ext)
	{
		self->SetPickExtent(*ext);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Recastnavigation_SetCoordinateSystem(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Recastnavigation* self, int* t)
	{
		self->SetCoordinateSystem((Recastnavigation::Coordinate)*t);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_Recastnavigation(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&Recastnavigation::RTTI());

		{
			hc::FunctionRegister sf;
			sf.Result( &NavigationQuery::RTTI());
			sf.Arguments(&hc::Int::RTTI(), &hc::String::RTTI());
			sf.Function(&Recastnavigation_LoadNavigationMesh, "LoadNavigationMesh");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&vector3::RTTI());
			sf.Function(&Recastnavigation_SetPickExtent, "SetPickExtent");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::Int::RTTI());
			sf.Function(&Recastnavigation_SetCoordinateSystem, "SetCoordinateSystem");
			sc.ClassFunction(sf);
		}

		sc.ClassConstant(Recastnavigation::RN_COORDINATE_ORIGINAL, "RN_COORDINATE_ORIGINAL");
		sc.ClassConstant(Recastnavigation::RN_COORDINATE_UNITY, "RN_COORDINATE_UNITY");
		sc.ClassConstant(Recastnavigation::RN_COORDINATE_UNREAL, "RN_COORDINATE_UNREAL");

		sys.SetNativeLibrary(name, sc);
		sys.SetNativeGlobal(name, Recastnavigation::RTTI().GetTypeName(), Recastnavigation::Instance());
	}
}