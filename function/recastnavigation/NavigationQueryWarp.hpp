#pragma once
#include "serialize/ISerializeSystem.h"
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include "RecastnavigationPlugin.h"
#include "NavigationQuery.h"
#include "vector3.hpp"

namespace hrnf
{
	//-------------------------------------------------------------------------------------------------------
	static int NavigationQuery_FindPath(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, NavigationQuery* self, int* node, float* step, vector3* begin, vector3* end)
	{
		const vector3array& data = self->FindPath(*node, *step, *begin, *end);
		res->SetValue((hc::Object*)&(data.Data()), &hc::ScriptValue::NullCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int NavigationQuery_FindClosestPoint(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, NavigationQuery* self, vector3* point)
	{
		vector3* obj = static_cast<vector3*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3::RTTI().GetFactor()));
		*obj = self->FindClosestPoint(*point);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int NavigationQuery_FindRandomPointAroundCircle(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, NavigationQuery* self, int* count, float* radius, vector3* cen)
	{
		const vector3array& data = self->FindRandomPointAroundCircle(*count, *radius, *cen);
		res->SetValue((hc::Object*)&(data.Data()), &hc::ScriptValue::NullCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_NavigationQuery(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&NavigationQuery::RTTI());

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::FloatArray::RTTI());
			sf.Arguments(&hc::Int::RTTI(), &hc::Float::RTTI()
				, &vector3::RTTI(), &vector3::RTTI());
			sf.Function(&NavigationQuery_FindPath, "FindPath");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&vector3::RTTI());
			sf.Arguments(&vector3::RTTI());
			sf.Function(&NavigationQuery_FindClosestPoint, "FindClosestPoint");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::FloatArray::RTTI());
			sf.Arguments(&hc::Int::RTTI(), &hc::Float::RTTI(), &vector3::RTTI());
			sf.Function(&NavigationQuery_FindRandomPointAroundCircle, "FindRandomPointAroundCircle");
			sc.ClassFunction(sf);
		}

		sys.SetNativeLibrary(name, sc);
	}
}