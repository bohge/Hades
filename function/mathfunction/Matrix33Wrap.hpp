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
	static int Matrix33_Construct(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res)
	{
		Matrix33f* obj = static_cast<Matrix33f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Matrix33f::RTTI().GetFactor()));
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Matrix33_Construct_P9(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, float* f1, float* f2, float* f3, float* f4, float* f5, float* f6, float* f7, float* f8, float* f9)
	{
		Matrix33f* obj = static_cast<Matrix33f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Matrix33f::RTTI().GetFactor()));
		*obj = Matrix33f(*f1, *f2, *f3, *f4, *f5, *f6, *f7, *f8, *f9);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Matrix33_YawPitchRoll(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Matrix33f* self, float* y, float* p, float* r)
	{
		self->YawPitchRoll(*y, *p, *r);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Matrix33_CreateRotateMatrixX(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, float* f1)
	{
		Matrix33f* obj = static_cast<Matrix33f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Matrix33f::RTTI().GetFactor()));
		*obj = Matrix33f::CreateRotateMatrixX(*f1);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Matrix33_CreateRotateMatrixY(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, float* f1)
	{
		Matrix33f* obj = static_cast<Matrix33f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Matrix33f::RTTI().GetFactor()));
		*obj = Matrix33f::CreateRotateMatrixY(*f1);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Matrix33_CreateRotateMatrixZ(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, float* f1)
	{
		Matrix33f* obj = static_cast<Matrix33f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Matrix33f::RTTI().GetFactor()));
		*obj = Matrix33f::CreateRotateMatrixZ(*f1);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Matrix33_Mul_mat(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Matrix33f* v1, Matrix33f* v2)
	{
		Matrix33f* obj = static_cast<Matrix33f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Matrix33f::RTTI().GetFactor()));
		*obj = *v1 * *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_Matrix33(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&Matrix33f::RTTI());
		{
			hc::FunctionRegister sf;
			sf.Result( &Matrix33f::RTTI());
			sf.Function(&Matrix33_Construct, hc::ScriptProperty::__CON);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Matrix33f::RTTI());
			sf.Arguments(&hc::Float::RTTI(), &hc::Float::RTTI(), &hc::Float::RTTI(), &hc::Float::RTTI(), &hc::Float::RTTI(), &hc::Float::RTTI(), &hc::Float::RTTI(), &hc::Float::RTTI(), &hc::Float::RTTI());
			sf.Function(&Matrix33_Construct_P9, hc::ScriptProperty::__CON);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::Float::RTTI());
			sf.Function(&Matrix33_YawPitchRoll, "YawPitchRoll");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Matrix33f::RTTI());
			sf.Arguments(&Matrix33f::RTTI());
			sf.Function(&Matrix33_Mul_mat, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Matrix33f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.SetFunctionType(hc::ScriptProperty::FT_STATIC);
			sf.Function(&Matrix33_CreateRotateMatrixX, "CreateRotateMatrixX");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Matrix33f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.SetFunctionType(hc::ScriptProperty::FT_STATIC);
			sf.Function(&Matrix33_CreateRotateMatrixY, "CreateRotateMatrixY");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Matrix33f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.SetFunctionType(hc::ScriptProperty::FT_STATIC);
			sf.Function(&Matrix33_CreateRotateMatrixZ, "CreateRotateMatrixZ");
			sc.ClassFunction(sf);
		}
		sys.SetNativeLibrary(name, sc);
	}
}