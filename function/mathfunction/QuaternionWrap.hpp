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
	static int Quaternion_Construct(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res)
	{
		Quaternionf* obj = static_cast<Quaternionf*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Quaternionf::RTTI().GetFactor()));
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_Construct_P4(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, float* f1, float* f2, float* f3, float* f4)
	{
		Quaternionf* obj = static_cast<Quaternionf*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Quaternionf::RTTI().GetFactor()));
		*obj = Quaternionf(*f1, *f2, *f3, *f4);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_Add_qua(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Quaternionf* v1, Quaternionf* v2)
	{
		Quaternionf* obj = static_cast<Quaternionf*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Quaternionf::RTTI().GetFactor()));
		*obj = Quaternionf(*v1 + *v2);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_Div_qua(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Quaternionf* v1, Quaternionf* v2)
	{
		Quaternionf* obj = static_cast<Quaternionf*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Quaternionf::RTTI().GetFactor()));
		*obj = Quaternionf(*v1 - *v2);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_Mul_qua(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Quaternionf* v1, Quaternionf* v2)
	{
		Quaternionf* obj = static_cast<Quaternionf*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Quaternionf::RTTI().GetFactor()));
		*obj = Quaternionf(*v1 * *v2);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_Mul_num(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Quaternionf* v1, float* v2)
	{
		Quaternionf* obj = static_cast<Quaternionf*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Quaternionf::RTTI().GetFactor()));
		*obj = Quaternionf(*v1 * *v2);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_Div_num(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Quaternionf* v1, float* v2)
	{
		Quaternionf* obj = static_cast<Quaternionf*>(hc::ISerializeSystem::Instance()
			->GetSerializable(Quaternionf::RTTI().GetFactor()));
		*obj = Quaternionf(*v1 / *v2);
		res->SetValue(obj, &hc::ISerializeSystem::SerializableCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_Equal(hc::IVirtualMachine* vm, hc::FunctionRegister* func, bool* res, Quaternionf* v1, Quaternionf* v2)
	{
		*res = v1->operator==(*v2);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_NormalizeSelf(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Quaternionf* v1)
	{
		v1->NormalizeSelf();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_Dot(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, Quaternionf* q1, Quaternionf* q2)
	{
		*res = q1->Dot(*q2);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_YawPitchRoll(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Quaternionf* self, float* y, float* p, float* r)
	{
		self->YawPitchRoll(*y, *p, *r);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_RotateXYZ(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Quaternionf* self, float* x, float* y, float* z)
	{
		self->RotateXYZ(*x, *y, *z);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_AxisToAxis(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Quaternionf* self, vector3f* f, vector3f* t)
	{
		self->AxisToAxis(*f, *t);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_RotateAxis(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Quaternionf* self, vector3f* a, float* r)
	{
		self->RotateAxis(*a, *r);
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_Length(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, Quaternionf* v1)
	{
		*res = v1->Length();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_LengthPow(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, Quaternionf* v1)
	{
		*res = v1->LengthPow();
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_GetInverse(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, Quaternionf* v1)
	{
		Quaternionf* qua = NEW Quaternionf;
		v1->GetInverse(*qua);
		res->SetValue(qua, &hc::ScriptValue::SafeDeleteCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_InverseSelf(hc::IVirtualMachine* vm, hc::FunctionRegister* func, Quaternionf* self)
	{
		self->InverseSelf();
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_X(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, Quaternionf* v1)
	{
		*res = v1->m_x;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_Y(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, Quaternionf* v1)
	{
		*res = v1->m_y;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_Z(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, Quaternionf* v1)
	{
		*res = v1->m_z;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int Quaternion_W(hc::IVirtualMachine* vm, hc::FunctionRegister* func, float* res, Quaternionf* v1)
	{
		*res = v1->m_w;
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_Quaternion(const eastl::string& name, hc::IScriptSystem& sys)
	{
		hc::ClassRegister sc;
		sc.ClassType(&Quaternionf::RTTI());

		{
			hc::FunctionRegister sf;
			sf.Result( &Quaternionf::RTTI());
			sf.Function(&Quaternion_Construct, hc::ScriptProperty::__CON);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Quaternionf::RTTI());
			sf.Arguments(&hc::Float::RTTI(), &hc::Float::RTTI(), &hc::Float::RTTI(), &hc::Float::RTTI());
			sf.Function(&Quaternion_Construct_P4, hc::ScriptProperty::__CON);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Quaternionf::RTTI());
			sf.Arguments(&Quaternionf::RTTI());
			sf.Function(&Quaternion_Add_qua, hc::ScriptProperty::__ADD);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Quaternionf::RTTI());
			sf.Arguments(&Quaternionf::RTTI());
			sf.Function(&Quaternion_Div_qua, hc::ScriptProperty::__DIV);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Quaternionf::RTTI());
			sf.Arguments(&Quaternionf::RTTI());
			sf.Function(&Quaternion_Mul_qua, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Quaternionf::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.Function(&Quaternion_Mul_num, hc::ScriptProperty::__MUL);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &Quaternionf::RTTI());
			sf.Arguments(&hc::Float::RTTI());
			sf.Function(&Quaternion_Div_num, hc::ScriptProperty::__DIV);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Bool::RTTI());
			sf.Arguments(&Quaternionf::RTTI());
			sf.Function(&Quaternion_Equal, hc::ScriptProperty::__EQU);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Function(&Quaternion_NormalizeSelf, "NormalizeSelf");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Arguments(&Quaternionf::RTTI());
			sf.Function(&Quaternion_Dot, "Dot");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Arguments(&hc::Float::RTTI(), &hc::Float::RTTI(), &hc::Float::RTTI());
			sf.Function(&Quaternion_YawPitchRoll, "YawPitchRoll");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Arguments(&vector3f::RTTI(), &vector3f::RTTI());
			sf.Function(&Quaternion_AxisToAxis, "AxisToAxis");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Arguments(&vector3f::RTTI(), &hc::Float::RTTI());
			sf.Function(&Quaternion_RotateAxis, "RotateAxis");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&Quaternion_Length, "Length");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&Quaternion_LengthPow, "LengthPow");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &Quaternionf::RTTI());
			sf.Function(&Quaternion_GetInverse, "GetInverse");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Function(&Quaternion_InverseSelf, "InverseSelf");
			sc.ClassFunction(sf);
		}
		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&Quaternion_X, "x");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&Quaternion_Y, "y");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&Quaternion_Z, "z");
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result( &hc::Float::RTTI());
			sf.Function(&Quaternion_W, "w");
			sc.ClassFunction(sf);
		}



		sys.SetNativeLibrary(name, sc);
	}
}