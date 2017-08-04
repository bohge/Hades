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
	static int vector3_Construct(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Construct_P3(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, float* f1, float* f2, float* f3)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = vector3f(*f1, *f2, *f3);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Add_vec(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector3f* v1, vector3f* v2)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = *v1 + *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Add_num(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector3f* v1, float* v2)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = *v1 + *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Sub_vec(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector3f* v1, vector3f* v2)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = *v1 - *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Sub_num(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector3f* v1, float* v2)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = *v1 - *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Mul_vec(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector3f* v1, vector3f* v2)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = *v1 * *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Mul_num(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector3f* v1, float* v2)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = *v1 * *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Mul_mat33(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector3f* v1, Matrix33f* m33)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = *v1 * *m33;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Mul_mat44(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector3f* v1, Matrix44f* m44)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = *v1 * *m44;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Mul_qua(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector3f* v1, Quaternionf* qua)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = *v1 * *qua;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Div_vec(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector3f* v1, vector3f* v2)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = *v1 / *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Div_num(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector3f* v1, float* v2)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = *v1 / *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Equal(hc::IVirtualMachine* vm, hc::FunctionRegister* func, bool* res, vector3f* v1, vector3f* v2)
	{
		*res = *v1 == *v2;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Dot(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector3f* v1, vector3f* v2)
	{
		*res = v1->Dot(*v2);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_NormalizeSelf(hc::IVirtualMachine* vm, hc::FunctionRegister* func, vector3f* v1)
	{
		v1->NormalizeSelf();
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Normalize(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector3f* v1)
	{
		vector3f* obj = static_cast<vector3f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector3f::RTTI().GetFactor()));
		*obj = v1->Normalize();
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Length(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector3f* v1)
	{
		*res = v1->Length();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_LengthPow(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector3f* v1)
	{
		*res = v1->LengthPow();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_X(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector3f* v1)
	{
		*res = v1->m_x;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Y(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector3f* v1)
	{
		*res = v1->m_y;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_Z(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector3f* v1)
	{
		*res = v1->m_z;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector3_tostring(hc::IVirtualMachine* vm, hc::FunctionRegister* func, eastl::string* res, vector3f* v1)
	{
		char buff[MAX_BUFFER_SIZE];
		int len = sprintf(buff, "vector3(%.3f,%.3f,%.3f)",
			v1->m_x, v1->m_y, v1->m_z);
		*res = buff;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_vector3(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&vector3f::RTTI());

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::String::RTTI());
			sf.Function(&vector3_tostring, hc::ScriptProperty::__STR);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&vector3_X, "x");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&vector3_Y, "y");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&vector3_Z, "z");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Function(&vector3_Construct, hc::ScriptProperty::__CON);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Arguments(&hc::Float::RTTI(), &hc::Float::RTTI(), &hc::Float::RTTI());
			sf.Function(&vector3_Construct_P3, hc::ScriptProperty::__CON);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Arguments(&vector3f::RTTI());
			sf.Function(&vector3_Add_vec, hc::ScriptProperty::__ADD);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.Function(&vector3_Add_num, hc::ScriptProperty::__ADD);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Arguments(&vector3f::RTTI());
			sf.Function(&vector3_Sub_vec, hc::ScriptProperty::__SUB);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.Function(&vector3_Sub_num, hc::ScriptProperty::__SUB);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Arguments(&vector3f::RTTI());
			sf.Function(&vector3_Mul_vec, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Arguments(&Matrix33f::RTTI());
			sf.Function(&vector3_Mul_mat33, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Arguments(&Matrix44f::RTTI());
			sf.Function(&vector3_Mul_mat44, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Arguments(&Quaternionf::RTTI());
			sf.Function(&vector3_Mul_qua, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.Function(&vector3_Mul_num, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Arguments(&vector3f::RTTI());
			sf.Function(&vector3_Div_vec, hc::ScriptProperty::__DIV);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.Function(&vector3_Div_num, hc::ScriptProperty::__DIV);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Bool::RTTI());
			sf.Arguments(&vector3f::RTTI());
			sf.Function(&vector3_Equal, hc::ScriptProperty::__EQU);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Arguments(&vector3f::RTTI());
			sf.Function(&vector3_Dot, "Dot");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Function(&vector3_NormalizeSelf, "NormalizeSelf");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector3f::RTTI());
			sf.Function(&vector3_Normalize, "Normalize");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&vector3_Length, "Length");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&vector3_LengthPow, "LengthPow");
			sc.ClassFunction(sf);
		}

		sys.SetNativeLibrary(name, sc);
	}
}