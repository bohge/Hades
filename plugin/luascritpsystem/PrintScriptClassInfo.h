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
#include "script/ScriptUtility.h"
#include "script/IScriptSystem.h"
#include "script/ClassRegister.h"
#include "core/Types.h"
#include <EASTL/map.h>



#define _PRINT_LUA_CLASS_INFO

#ifdef _PRINT_LUA_CLASS_INFO
typedef eastl::map< uint, hc::ClassRegister* > ScriptClassMap;
static bool isNotOperater( const eastl::string& name )
{
	if (name == hc::ScriptProperty::__ADD
		|| name == hc::ScriptProperty::__SUB
		|| name == hc::ScriptProperty::__MUL
		|| name == hc::ScriptProperty::__DIV
		|| name == hc::ScriptProperty::__CON
		|| name == hc::ScriptProperty::__EQU
		|| name == hc::ScriptProperty::__LES
		|| name == hc::ScriptProperty::__LEQ )
	{
		return false;
	}
	return true;
}

static const char* GetType(const hc::Rtti* type)
{
	if (
		type->GetTypeID() == hc::Short::RTTI().GetTypeID()
		|| type->GetTypeID() == hc::UShort::RTTI().GetTypeID()
		|| type->GetTypeID() == hc::Int::RTTI().GetTypeID()
		|| type->GetTypeID() == hc::UInt::RTTI().GetTypeID()
		|| type->GetTypeID() == hc::Float::RTTI().GetTypeID()
		|| type->GetTypeID() == hc::Double::RTTI().GetTypeID()
		)
	{
		return "number";
	}
	else if (type->GetTypeID() == hc::Bool::RTTI().GetTypeID())
	{
		return "boolean";
	}
	else if (type->GetTypeID() == hc::String::RTTI().GetTypeID())
	{
		return "string";
	}
	return type->GetTypeName();
}
static eastl::string PrintScriptClassInfo(ScriptClassMap& classes)
{
	eastl::string res;
	static bool call = true;
	if ( call )
	{
		call = false;		
		res += "return {\r\n";
		FOR_EACH( ScriptClassMap, it, classes )
		{
			res += it->second->GetClassTypeName();
			res += " = {\r\n";
			if ( it->second->isDriverType() )
			{
				res += "\tinherits = \"";
				res += it->second->GetBaseClassTypeName();
				res += "\",\r\n";
			}
			res += "\tdescription = \"\",\r\n\ttype = \"class\",\r\n\tchilds = {\r\n";
			if (	it->second->GetClassTypeID() == hc::Bool::RTTI().GetTypeID()
				|| it->second->GetClassTypeID() == hc::Short::RTTI().GetTypeID()
				|| it->second->GetClassTypeID() == hc::UShort::RTTI().GetTypeID()
				|| it->second->GetClassTypeID() == hc::Int::RTTI().GetTypeID()
				|| it->second->GetClassTypeID() == hc::UInt::RTTI().GetTypeID()
				|| it->second->GetClassTypeID() == hc::Float::RTTI().GetTypeID()
				||  it->second->GetClassTypeID() == hc::Double::RTTI().GetTypeID()
				||  it->second->GetClassTypeID() == hc::String::RTTI().GetTypeID() )
			{
				res += "\t\tData = {\r\n\t\t\ttype = \"function\",\r\n\t\t\tdescription = \"\",\r\n";
				if (it->second->GetClassTypeID() == hc::Short::RTTI().GetTypeID()
					|| it->second->GetClassTypeID() == hc::UShort::RTTI().GetTypeID()
					|| it->second->GetClassTypeID() == hc::Int::RTTI().GetTypeID()
					|| it->second->GetClassTypeID() == hc::UInt::RTTI().GetTypeID()
					|| it->second->GetClassTypeID() == hc::Float::RTTI().GetTypeID()
					|| it->second->GetClassTypeID() == hc::Double::RTTI().GetTypeID())
				{
					res += "\t\t\treturns = \"(:number)\"\r\n\t\t}\r\n";
				}
				else if( it->second->GetClassTypeID() == hc::Bool::RTTI().GetTypeID() )
				{
					res += "\t\t\treturns = \"(:boolean)\"\r\n\t\t}\r\n";
				}
				else if( it->second->GetClassTypeID() == hc::String::RTTI().GetTypeID() )
				{
					res += "\t\t\treturns = \"(:string)\"\r\n\t\t}\r\n";
				}
			}
			else
			{
				{//处理定值
					uint constCount = it->second->GetClassConstantCount();
					for ( int i = 0 ; i < constCount ; ++ i )
					{
						res += "\t\t";
						res += it->second->GetClassConstant( i ).first;
						res += " = {\r\n\t\t\tdescription = \"\",\r\n\t\t\ttype = \"value\",\r\n\t\t},\r\n";
					}
				}
				{//处理成员函数
					uint funcCount = it->second->GetFunctionListCount();
					for ( int i = 0 ; i < funcCount ; ++ i )
					{
						if( isNotOperater( it->second->GetFunctionListName(i) ) )
						{
							res += "\t\t";
							res += it->second->GetFunctionListName(i) + " = {\r\n\t\t\ttype = \"function\",\r\n\t\t\tdescription = \"\",\r\n";
							{//处理函数参数
								res += "\t\t\targs = \"";
								const hc::OverlappeArray& functions = it->second->GetFunctions(i);
								FOR_EACH_CONST(hc::OverlappeArray, funcArray, functions)
								{
									FOR_EACH_CONST(hc::FunctionArray, single, *funcArray)
									{
										res += "(";
										uint argCount = (*single)->GetArgumentCount();
										for ( int j = 0 ; j < argCount ; ++ j )
										{
											res += ":";
											//res += (*single)->GetArguments(j)->GetTypeName();
											res += GetType((*single)->GetArguments(j));
											if ( j != argCount - 1 )
											{
												res += ",";
											}
										}
										res += ")";
									}
								}
								res += "\",\r\n";
							}//处理函数参数
							{//处理函数返回值
								bool isNoReturn = true;
								const hc::OverlappeArray& functions = it->second->GetFunctions(i);
								res += "\t\t\treturns = \"";
								FOR_EACH_CONST(hc::OverlappeArray, funcArray, functions)
								{
									FOR_EACH_CONST( hc::FunctionArray, single, *funcArray )
									{
										if ( (*single)->hasReturn() )
										{
											isNoReturn = false;
											res += "(:";
											//res += (*single)->GetReturnType()->GetTypeName();
											res += GetType((*single)->GetReturnType());
											res += ")";
										}
									}
								}
								if ( isNoReturn )
								{
									res += "()\",\r\n";
								}
								else
								{
									res += "\",\r\n";
								}
							}
							res += "\t\t},\r\n";
						}
					}
				}
			}
			res += "\t}\r\n},\r\n";
		}
		res += "}";
	}
	return res;
}
#endif