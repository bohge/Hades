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
	static int Matrix44_Construct(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res)
	{
		Matrix44f* obj = static_cast<Matrix44f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Matrix44f::RTTI().GetFactor()));
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Matrix44_SetTransform(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Matrix44f* self, vector3f* v)
	{
		self->SetTransform(*v);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Matrix44_Mul_mat44(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Matrix44f* v1, Matrix44f* v2)
	{
		Matrix44f* obj = static_cast<Matrix44f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Matrix44f::RTTI().GetFactor()));
		*obj = *v1 * *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Matrix44_GetInverse(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Matrix44f* v1)
	{
		Matrix44f* obj = static_cast<Matrix44f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Matrix44f::RTTI().GetFactor()));
		v1->GetInverse(*obj);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Matrix44_InverseSelf(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Matrix44f* v1)
	{
		v1->InverseSelf();
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Matrix44_CreateRotateMatrix(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector3f* axis, float* f1)
	{
		Matrix44f* obj = static_cast<Matrix44f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Matrix44f::RTTI().GetFactor()));
		*obj = Matrix44f::CreateRotateMatrix(*axis, *f1);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Matrix44_CreateRotateMatrixX(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, float* f1)
	{
		Matrix44f* obj = static_cast<Matrix44f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Matrix44f::RTTI().GetFactor()));
		*obj = Matrix44f::CreateRotateMatrixX(*f1);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Matrix44_CreateRotateMatrixY(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, float* f1)
	{
		Matrix44f* obj = static_cast<Matrix44f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Matrix44f::RTTI().GetFactor()));
		*obj = Matrix44f::CreateRotateMatrixY(*f1);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Matrix44_CreateRotateMatrixZ(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, float* f1)
	{
		Matrix44f* obj = static_cast<Matrix44f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Matrix44f::RTTI().GetFactor()));
		*obj = Matrix44f::CreateRotateMatrixZ(*f1);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_Matrix44(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&Matrix44f::RTTI());
		{
			hc::FunctionRegister sf;
			sf.Result( &Matrix44f::RTTI());
			sf.Function(&Matrix44_Construct, hc::ScriptProperty::__CON);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&vector3f::RTTI());
			sf.Function(&Matrix44_SetTransform, "SetTransform");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&Matrix44f::RTTI());
			sf.Function(&Matrix44_InverseSelf, "InverseSelf");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Matrix44f::RTTI());
			sf.Function(&Matrix44_GetInverse, "GetInverse");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Matrix44f::RTTI());
			sf.Arguments(&Matrix44f::RTTI());
			sf.Function(&Matrix44_Mul_mat44, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Matrix44f::RTTI());
			sf.Arguments(&vector3f::RTTI(), &hc::Float::RTTI());
			sf.SetFunctionType(hc::ScriptProperty::FT_STATIC);
			sf.Function(&Matrix44_CreateRotateMatrix, "CreateRotateMatrix");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Matrix44f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.SetFunctionType(hc::ScriptProperty::FT_STATIC);
			sf.Function(&Matrix44_CreateRotateMatrixX, "CreateRotateMatrixX");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Matrix44f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.SetFunctionType(hc::ScriptProperty::FT_STATIC);
			sf.Function(&Matrix44_CreateRotateMatrixY, "CreateRotateMatrixY");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Matrix44f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.SetFunctionType(hc::ScriptProperty::FT_STATIC);
			sf.Function(&Matrix44_CreateRotateMatrixZ, "CreateRotateMatrixZ");
			sc.ClassFunction(sf);
		}
		sys.SetNativeLibrary(name, sc);
	}
}