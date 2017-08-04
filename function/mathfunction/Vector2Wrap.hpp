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
	static int vector2_Construct(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res)
	{
		vector2f* obj = static_cast<vector2f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector2f::RTTI().GetFactor()));
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_Construct_P2(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, float* f1, float* f2)
	{
		vector2f* obj = static_cast<vector2f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector2f::RTTI().GetFactor()));
		*obj = vector2f(*f1, *f2);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_Add_vec(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector2f* v1, vector2f* v2)
	{
		vector2f* obj = static_cast<vector2f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector2f::RTTI().GetFactor()));
		*obj = *v1 + *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_Add_num(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector2f* v1, float* v2)
	{
		vector2f* obj = static_cast<vector2f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector2f::RTTI().GetFactor()));
		*obj = *v1 + *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_Sub_vec(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector2f* v1, vector2f* v2)
	{
		vector2f* obj = static_cast<vector2f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector2f::RTTI().GetFactor()));
		*obj = *v1 - *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_Sub_num(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector2f* v1, float* v2)
	{
		vector2f* obj = static_cast<vector2f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector2f::RTTI().GetFactor()));
		*obj = *v1 - *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_Mul_vec(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector2f* v1, vector2f* v2)
	{
		vector2f* obj = static_cast<vector2f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector2f::RTTI().GetFactor()));
		*obj = *v1 * *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_Mul_num(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector2f* v1, float* v2)
	{
		vector2f* obj = static_cast<vector2f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector2f::RTTI().GetFactor()));
		*obj = *v1 * *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_Div_vec(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector2f* v1, vector2f* v2)
	{
		vector2f* obj = static_cast<vector2f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector2f::RTTI().GetFactor()));
		*obj = *v1 / *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_Div_num(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector2f* v1, float* v2)
	{
		vector2f* obj = static_cast<vector2f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector2f::RTTI().GetFactor()));
		*obj = *v1 / *v2;
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_Equal(hc::IVirtualMachine* vm, hc::FunctionRegister* func, bool* res, vector2f* v1, vector2f* v2)
	{
		*res = *v1 == *v2;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_Dot(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector2f* v1, vector2f* v2)
	{
		*res = v1->Dot(*v2);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_NormalizeSelf(hc::IVirtualMachine* vm, hc::FunctionRegister* func, vector2f* v1)
	{
		v1->NormalizeSelf();
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_Normalize(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, vector2f* v1)
	{
		vector2f* obj = static_cast<vector2f*>(hc::ISerializeSystem::Instance()
			->GetSerializable(vector2f::RTTI().GetFactor()));
		*obj = v1->Normalize();
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_Length(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector2f* v1)
	{
		*res = v1->Length();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_LengthPow(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector2f* v1)
	{
		*res = v1->LengthPow();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_X(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector2f* v1)
	{
		*res = v1->m_x;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_Y(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, vector2f* v1)
	{
		*res = v1->m_y;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int vector2_tostring(hc::IVirtualMachine* vm, hc::FunctionRegister* func, eastl::string* res, vector2f* v1)
	{
		char buff[MAX_BUFFER_SIZE];
		int len = sprintf(buff, "vector2(%.3f,%.3f)", v1->m_x, v1->m_y);
		*res = buff;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_vector2(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&vector2f::RTTI());

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&vector2_X, "x");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&vector2_Y, "y");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector2f::RTTI());
			sf.Function(&vector2_Construct, hc::ScriptProperty::__CON);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector2f::RTTI());
			sf.Arguments(&hc::Float::RTTI(), &hc::Float::RTTI());
			sf.Function(&vector2_Construct_P2, hc::ScriptProperty::__CON);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector2f::RTTI());
			sf.Arguments(&vector2f::RTTI());
			sf.Function(&vector2_Add_vec, hc::ScriptProperty::__ADD);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector2f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.Function(&vector2_Add_num, hc::ScriptProperty::__ADD);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector2f::RTTI());
			sf.Arguments(&vector2f::RTTI());
			sf.Function(&vector2_Sub_vec, hc::ScriptProperty::__SUB);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector2f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.Function(&vector2_Sub_num, hc::ScriptProperty::__SUB);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector2f::RTTI());
			sf.Arguments(&vector2f::RTTI());
			sf.Function(&vector2_Mul_vec, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector2f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.Function(&vector2_Mul_num, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector2f::RTTI());
			sf.Arguments(&vector2f::RTTI());
			sf.Function(&vector2_Div_vec, hc::ScriptProperty::__DIV);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector2f::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.Function(&vector2_Div_num, hc::ScriptProperty::__DIV);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Bool::RTTI());
			sf.Arguments(&vector2f::RTTI());
			sf.Function(&vector2_Equal, hc::ScriptProperty::__EQU);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&hc::String::RTTI());
			sf.Function(&vector2_tostring, hc::ScriptProperty::__STR);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Arguments(&vector2f::RTTI());
			sf.Function(&vector2_Dot, "Dot");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Function(&vector2_NormalizeSelf, "NormalizeSelf");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &vector2f::RTTI());
			sf.Function(&vector2_Normalize, "Normalize");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&vector2_Length, "Length");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&vector2_LengthPow, "LengthPow");
			sc.ClassFunction(sf);
		}

		sys.SetNativeLibrary(name, sc);
	}
}