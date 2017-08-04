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
	static int vector4_Construct(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res)
	{
		vector4f* obj = static_cast<vector4f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector4f::RTTI().GetFactor()));
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_Construct_P4(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, float* f1, float* f2, float* f3, float* f4)
	{
		vector4f* obj = static_cast<vector4f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector4f::RTTI().GetFactor()));
		*obj = vector4f(*f1, *f2, *f3, *f4);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_Add_vec(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector4f* v1, vector4f* v2)
	{
		vector4f* obj = static_cast<vector4f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector4f::RTTI().GetFactor()));
		*obj = *v1 + *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_Add_num(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector4f* v1, float* v2)
	{
		vector4f* obj = static_cast<vector4f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector4f::RTTI().GetFactor()));
		*obj = *v1 + *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_Sub_vec(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector4f* v1, vector4f* v2)
	{
		vector4f* obj = static_cast<vector4f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector4f::RTTI().GetFactor()));
		*obj = *v1 - *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_Sub_num(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector4f* v1, float* v2)
	{
		vector4f* obj = static_cast<vector4f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector4f::RTTI().GetFactor()));
		*obj = *v1 - *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_Mul_vec(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector4f* v1, vector4f* v2)
	{
		vector4f* obj = static_cast<vector4f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector4f::RTTI().GetFactor()));
		*obj = *v1 * *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_Mul_num(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector4f* v1, float* v2)
	{
		vector4f* obj = static_cast<vector4f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector4f::RTTI().GetFactor()));
		*obj = *v1 * *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_Mul_M44(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector4f* v1, Matrix44f* m)
	{
		vector4f* obj = static_cast<vector4f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector4f::RTTI().GetFactor()));
		*obj = *v1 * *m;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_Mul_QUA(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector4f* v1, Quaternionf* q)
	{
		vector4f* obj = static_cast<vector4f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector4f::RTTI().GetFactor()));
		*obj = *v1 * *q;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_Div_vec(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector4f* v1, vector4f* v2)
	{
		vector4f* obj = static_cast<vector4f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector4f::RTTI().GetFactor()));
		*obj = *v1 / *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_Div_num(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector4f* v1, float* v2)
	{
		vector4f* obj = static_cast<vector4f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector4f::RTTI().GetFactor()));
		*obj = *v1 / *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_Equal(hc::IVirtualMachine* vm, hc::FunctionRegister* func, bool* res, vector4f* v1, vector4f* v2)
	{
		*res = *v1 == *v2;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_NormalizeSelf(hc::IVirtualMachine* vm, hc::FunctionRegister* func, vector4f* v1)
	{
		v1->NormalizeSelf();
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_Normalize(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector4f* v1)
	{
		vector4f* obj = static_cast<vector4f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector4f::RTTI().GetFactor()));
		*obj = v1->Normalize();
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_Length(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector4f* v1)
	{
		*res = v1->Length();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_LengthPow(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector4f* v1)
	{
		*res = v1->LengthPow();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_X(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector4f* v1)
	{
		*res = v1->m_x;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_Y(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector4f* v1)
	{
		*res = v1->m_y;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_Z(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector4f* v1)
	{
		*res = v1->m_z;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_W(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector4f* v1)
	{
		*res = v1->m_w;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector4_tostring(hc::IVirtualMachine* vm, hc::FunctionRegister* func, eastl::string* res, vector4f* v1)
	{
		char buff[MAX_BUFFER_SIZE];
		int len = sprintf(buff, "vector4(%.3f,%.3f,%.3f,%.3f)",
			v1->m_x, v1->m_y, v1->m_z, v1->m_w);
		*res = buff;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_vector4(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&vector4f::RTTI());

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::String::RTTI());
			sf.Function(&vector4_tostring, hc::ScriptProperty::__STR);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&vector4_X, "x");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&vector4_Y, "y");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&vector4_Z, "z");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&vector4_W, "w");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector4f::RTTI());
			sf.Function(&vector4_Construct, hc::ScriptProperty::__CON);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector4f::RTTI());
			sf.Arguments(&hc::Float::RTTI(), &hc::Float::RTTI(), &hc::Float::RTTI(), &hc::Float::RTTI());
			sf.Function(&vector4_Construct_P4, hc::ScriptProperty::__CON);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector4f::RTTI());
			sf.Arguments(&vector4f::RTTI());
			sf.Function(&vector4_Add_vec, hc::ScriptProperty::__ADD);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector4f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.Function(&vector4_Add_num, hc::ScriptProperty::__ADD);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector4f::RTTI());
			sf.Arguments(&vector4f::RTTI());
			sf.Function(&vector4_Sub_vec, hc::ScriptProperty::__SUB);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector4f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.Function(&vector4_Sub_num, hc::ScriptProperty::__SUB);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector4f::RTTI());
			sf.Arguments(&vector4f::RTTI());
			sf.Function(&vector4_Mul_vec, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector4f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.Function(&vector4_Mul_num, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector4f::RTTI());
			sf.Arguments(&Matrix44f::RTTI());
			sf.Function(&vector4_Mul_M44, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector4f::RTTI());
			sf.Arguments(&Quaternionf::RTTI());
			sf.Function(&vector4_Mul_QUA, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector4f::RTTI());
			sf.Arguments(&vector4f::RTTI());
			sf.Function(&vector4_Div_vec, hc::ScriptProperty::__DIV);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector4f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.Function(&vector4_Div_num, hc::ScriptProperty::__DIV);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Bool::RTTI());
			sf.Arguments(&vector4f::RTTI());
			sf.Function(&vector4_Equal, hc::ScriptProperty::__EQU);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Function(&vector4_NormalizeSelf, "NormalizeSelf");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector4f::RTTI());
			sf.Function(&vector4_Normalize, "Normalize");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&vector4_Length, "Length");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&vector4_LengthPow, "LengthPow");
			sc.ClassFunction(sf);
		}

		sys.SetNativeLibrary(name, sc);
	}
}