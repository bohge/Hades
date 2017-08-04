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



///////////////////////////////////////////////////
//	数学库，请引用此头文件，不要引用其它拆分的实现  //
///////////////////////////////////////////////////
#pragma once
#include "serialize/FactorConfigure.h"
#include "core/Predefine.h"
#include "BaseMathDeclaration.h"
#include "ShapeMathDeclaration.h"

#include <stdlib.h>
#include <string.h>

#ifndef __cplusplus
#error( "Do not include this file in c files" )
#endif


//注意，全部的顶点类型不能有任何的虚函数，应为顶点类型最终是通过opengl的内存指针的形式被指定出来，然后进行绘制
//这种形式要求数据在内存上是连续的，任何虚函数可能导致数据的不连续，所以不能使用虚函数
//2x2矩阵计算4x4其次矩阵的逆时使用，暂时如此
#include "Matrix22.hpp"
#include "Matrix33.hpp"
#include "Matrix44.hpp"
#include "vector2.hpp"
#include "vector3.hpp"
#include "vector4.hpp"
#include "Quaternion.hpp"
#include "BaseMathImplement.hpp"

#include "Ray.hpp"
#include "Line.hpp"
#include "Plane.hpp"
#include "Frustum.hpp"
#include "Aabbox.hpp"
#include "Aabrect.hpp"
#include "Aabround.hpp"
#include "Aabsphere.hpp"
#include "ShapeMathImplement.hpp"


namespace hmfp
{

	template<> RTTI_TYPE_IMPLEMENT_WITH_FACTOR(vector2f, FACTOR_VECTOR2F);

	template<> RTTI_TYPE_IMPLEMENT_WITH_FACTOR(vector3f, FACTOR_VECTOR3F);

	template<> RTTI_TYPE_IMPLEMENT_WITH_FACTOR(vector4f, FACTOR_VECTOR4F);

	template<> RTTI_TYPE_IMPLEMENT_WITH_FACTOR(Matrix22f, FACTOR_MATRIX22F);

	template<> RTTI_TYPE_IMPLEMENT_WITH_FACTOR(Matrix33f, FACTOR_MATRIX33F);

	template<> RTTI_TYPE_IMPLEMENT_WITH_FACTOR(Matrix44f, FACTOR_MATRIX44F);

	template<> RTTI_TYPE_IMPLEMENT_WITH_FACTOR(Quaternionf, FACTOR_QUATERNIONF);

	template<> RTTI_TYPE_IMPLEMENT_WITH_FACTOR(Rayf, FACTOR_RAYF);

	template<> RTTI_TYPE_IMPLEMENT_WITH_FACTOR(Linef, FACTOR_LINEF);

	template<> RTTI_TYPE_IMPLEMENT_WITH_FACTOR(Planef, FACTOR_PLANEF);

	template<> RTTI_TYPE_IMPLEMENT_WITH_FACTOR(Frustumf, FACTOR_FRUSTUMF);

	template<> RTTI_TYPE_IMPLEMENT_WITH_FACTOR(Aabbox3df, FACTOR_AABBOX3DF);

	template<> RTTI_TYPE_IMPLEMENT_WITH_FACTOR(Aabrectf, FACTOR_AABRECTF);

	template<> RTTI_TYPE_IMPLEMENT_WITH_FACTOR(Aabroundf, FACTOR_AABROUNDF);

	template<> RTTI_TYPE_IMPLEMENT_WITH_FACTOR(Aabspheref, FACTOR_AABSPHEREF);
	namespace Math
	{
		enum DataStructType
		{
			DST_VECTOR1 = 0,
			DST_VECTOR2,
			DST_VECTOR3,
			DST_VECTOR4,
			DST_MATRIX22,
			DST_MATRIX33,
			DST_MATRIX44,
		};
	}
}