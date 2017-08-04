//////////////////////////////////////////////////////////////////////////////////////
//
//						The Bohge Engine License (BEL)
//
//	Copyright (c) 2011-2014 Peng Zhao
//
//	Permission is hereby granted, free of charge, to any person obtaining a copy
//	of this software and associated documentation files (the "Software"), to deal
//	in the Software without restriction, including without limitation the rights
//	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
//	copies of the Software, and to permit persons to whom the Software is
//	furnished to do so, subject to the following conditions:
//
//	The above copyright notice and this permission notice shall be included in 
//	all copies or substantial portions of the Software. And the logo of 
//	Bohge Engine shall be displayed full screen for more than 3 seconds 
//	when the software is started. Copyright holders are allowed to develop 
//	game edit based on Bohge Engine, The edit must be released under the MIT 
//	open source license if it is going to be published. In no event shall 
//	copyright holders be prohibited from using any code of Bohge Engine 
//	to develop any other analogous game engines.
//
//	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
//	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
//	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
//
//////////////////////////////////////////////////////////////////////////////////////
#pragma once
#include "serialize/ISerializeSystem.h"
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/IScriptSystem.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "core/Configure.h"
#include "core/Types.h"
#include "3DMath.h"



namespace hmfp
{
	//-------------------------------------------------------------------------------------------------------
	static int Ray_Construct(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res)
	{
		Rayf* obj = static_cast<Rayf*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Rayf::RTTI().GetFactor()));
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Ray_Construct_Vector3(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector3f* v1, vector3f* v2)
	{
		Rayf* obj = static_cast<Rayf*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Rayf::RTTI().GetFactor()));
		*obj = Rayf(*v1, *v2);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Ray_SetRayOrigin(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Rayf* ray, vector3f* v1)
	{
		ray->SetRayOrigin(*v1);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Ray_SetRayDirection(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Rayf* ray, vector3f* v1)
	{
		ray->SetRayDirection(*v1);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Ray_GetRayOrigin(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Rayf* ray)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = ray->GetRayOrigin();
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Ray_GetRayDirection(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Rayf* ray)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = ray->GetRayDirection();
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Ray_Intersect(hc::IVirtualMachine* vm, hc::FunctionRegister* func, bool* res, Rayf* ray, Aabbox3df* box)
	{
		*res = ray->Intersect(*box);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_Ray(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&Rayf::RTTI());
		{
			hc::FunctionRegister sf;
			sf.Result( &Rayf::RTTI());
			sf.Function(&Ray_Construct, hc::ScriptProperty::__CON);
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &Rayf::RTTI());
			sf.Arguments(&vector3f::RTTI(), &vector3f::RTTI());
			sf.Function(&Ray_Construct_Vector3, hc::ScriptProperty::__CON);
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Arguments(&vector3f::RTTI());
			sf.Function(&Ray_SetRayOrigin, "SetOrigin");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Arguments(&vector3f::RTTI());
			sf.Function(&Ray_SetRayDirection, "SetDirection");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Function(&Ray_GetRayOrigin, "GetOrigin");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Function(&Ray_GetRayDirection, "GetDirection");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &Aabbox3df::RTTI());
			sf.Function(&Ray_Intersect, "Intersect");
			sc.ClassFunction(sf);
		}
		sys.SetNativeLibrary(name, sc);
	}
}