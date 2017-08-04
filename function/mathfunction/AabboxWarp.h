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
	static int Aabbox_Construct(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res)
	{
		Aabbox3df* obj = static_cast<Aabbox3df*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Aabbox3df::RTTI().GetFactor()));
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_Construct_Vector(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector3f* v1, vector3f* v2)
	{
		Aabbox3df* obj = static_cast<Aabbox3df*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Aabbox3df::RTTI().GetFactor()));
		*obj = Aabbox3df(*v1, *v2);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_Reset(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Aabbox3df* box)
	{
		box->Reset();
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_GetMin(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Aabbox3df* box)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = box->GetMin();
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_GetMax(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Aabbox3df* box)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = box->GetMax();
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_GetCenter(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Aabbox3df* box)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = box->GetCenter();
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_SetMin(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Aabbox3df* box, vector3f* v1)
	{
		box->SetMin(*v1);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_SetMax(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Aabbox3df* box, vector3f* v1)
	{
		box->SetMax(*v1);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_SetCenter(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Aabbox3df* box, vector3f* v1)
	{
		box->SetCenter(*v1);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_MoveDir(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Aabbox3df* box, vector3f* v1)
	{
		box->MoveDir(*v1);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_Mul_Vector3(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Aabbox3df* box, vector3f* v1)
	{
		Aabbox3df* obj = static_cast<Aabbox3df*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Aabbox3df::RTTI().GetFactor()));
		*obj = *box * *v1;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_Mul_Matrix(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Aabbox3df* box, Matrix44f* v1)
	{
		Aabbox3df* obj = static_cast<Aabbox3df*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Aabbox3df::RTTI().GetFactor()));
		*obj = *box * *v1;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_Contains_Vector3(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Aabbox3df* box, vector3f* v1)
	{
		box->Contains(*v1);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_Contains_Aabbox(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Aabbox3df* box, Aabbox3df* v1)
	{
		box->Contains(*v1);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_Intersect_Aabbox(hc::IVirtualMachine* vm, hc::FunctionRegister* func, bool* res, Aabbox3df* box, Aabbox3df* v1)
	{
		*res = box->Intersect(*v1);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_Intersect_Vector3(hc::IVirtualMachine* vm, hc::FunctionRegister* func, bool* res, Aabbox3df* box, vector3f* v1)
	{
		*res = box->Intersect(*v1);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_Intersect_Ray(hc::IVirtualMachine* vm, hc::FunctionRegister* func, bool* res, Aabbox3df* box, Rayf* v1)
	{
		*res = box->Intersect(*v1);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Aabbox_tostring(hc::IVirtualMachine* vm, hc::FunctionRegister* func, eastl::string* res, Aabbox3df* box)
	{
		char buff[MAX_BUFFER_SIZE];
		const vector3f& bmin = box->GetMin();
		const vector3f& bmax = box->GetMax();
		int len = sprintf(buff, "aabbox min(%.3f,%.3f,%.3f) max(%.3f,%.3f,%.3f)",
			bmin.m_x, bmin.m_y, bmin.m_z,
			bmax.m_x, bmax.m_y, bmax.m_z);
		*res = buff;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_Aabbox(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&Aabbox3df::RTTI());
		{
			hc::FunctionRegister sf;
			sf.Result( &Aabbox3df::RTTI());
			sf.Function(&Aabbox_Construct, hc::ScriptProperty::__CON);
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result(&hc::String::RTTI());
			sf.Function(&Aabbox_tostring, hc::ScriptProperty::__STR);
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &Aabbox3df::RTTI());
			sf.Arguments(&vector3f::RTTI(), &vector3f::RTTI());
			sf.Function(&Aabbox_Construct_Vector, hc::ScriptProperty::__CON);
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &Aabbox3df::RTTI());
			sf.Arguments(&vector3f::RTTI());
			sf.Function(&Aabbox_Mul_Vector3, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &Aabbox3df::RTTI());
			sf.Arguments(&Matrix44f::RTTI());
			sf.Function(&Aabbox_Mul_Matrix, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Function(&Aabbox_Reset, "Reset");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Function(&Aabbox_GetMin, "GetMin");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Function(&Aabbox_GetMax, "GetMax");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Function(&Aabbox_GetCenter, "GetCenter");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Arguments(&vector3f::RTTI());
			sf.Function(&Aabbox_SetMin, "SetMin");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Arguments(&vector3f::RTTI());
			sf.Function(&Aabbox_SetMax, "SetMax");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Arguments(&vector3f::RTTI());
			sf.Function(&Aabbox_SetCenter, "SetCenter");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Arguments(&vector3f::RTTI());
			sf.Function(&Aabbox_MoveDir, "Move");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Arguments(&vector3f::RTTI());
			sf.Function(&Aabbox_Contains_Vector3, "Contains");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Arguments(&Aabbox3df::RTTI());
			sf.Function(&Aabbox_Contains_Aabbox, "Contains");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Bool::RTTI());
			sf.Arguments(&Aabbox3df::RTTI());
			sf.Function(&Aabbox_Intersect_Aabbox, "Intersect");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Bool::RTTI());
			sf.Arguments(&vector3f::RTTI());
			sf.Function(&Aabbox_Intersect_Vector3, "Intersect");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Bool::RTTI());
			sf.Arguments(&Rayf::RTTI());
			sf.Function(&Aabbox_Intersect_Ray, "Intersect");
			sc.ClassFunction(sf);
		}
		sys.SetNativeLibrary(name, sc);
	}
}