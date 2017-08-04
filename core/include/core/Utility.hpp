#pragma once
#include "Predefine.h"



#include <EASTL/string.h>
#include <EASTL/vector.h>
#include <EASTL/list.h>

namespace Utility
{
	//------------------------------------------------------------------------------------------------------------
	static HADES_FORCEINLINE uint64 ComposeKey(uint gid, ushort uuid)
	{
		return (uint64(uuid) << 32) | gid;
	}
	//------------------------------------------------------------------------------------------------------------
	static HADES_FORCEINLINE void SeparateKey(uint64 key, uint& gid, ushort& uuid)
	{
		gid = (uint)key;
		uuid = (ushort)(key >> 32);
	}
	//------------------------------------------------------------------------------------------------------------
	static HADES_FORCEINLINE uint GetIndexFormKey(uint64 key)
	{
		return (uint)key;
	}
	//------------------------------------------------------------------------------------------------------------
	static HADES_FORCEINLINE ushort GetUUIDFormKey(uint64 key)
	{
		return (ushort)(key >> 32);
	}
	//------------------------------------------------------------------------------------------------------------
	template<typename T>
	bool isInclude( const eastl::list<T>& lhs, const T& rhs )
	{
		typedef typename  eastl::list< T >::const_iterator ListCIT;
		for ( ListCIT it = lhs.begin();
			it != lhs.end();
			++ it  )
		{
			if( (*it) == rhs )
			{
				return true;
			}
		}
		return false;
	}
	//------------------------------------------------------------------------------------------------------------
	static unsigned int HashCode( const char * str )
	{
		size_t len = strlen(str);
		unsigned int hash = 0;
		for(size_t i = 0; i < len; ++i) 
			hash = 65599 * hash + str[i];
		return hash ^ (hash >> 16);
	}
	//------------------------------------------------------------------------------------------------------------
	static unsigned int HashCode( const eastl::string& str )
	{
		unsigned int hash = 0;
		for(size_t i = 0; i < str.size(); ++i) 
			hash = 65599 * hash + str[i];
		return hash ^ (hash >> 16);
	}
	//------------------------------------------------------------------------------------------------------------
	static unsigned int HashCode( const eastl::vector<unsigned char>& buf )
	{
		unsigned int hash = 0;
		for(size_t i = 0; i < buf.size(); ++i) 
			hash = 65599 * hash + buf[i];
		return hash ^ (hash >> 16);
	}
	//------------------------------------------------------------------------------------------------------------
	static void RemoveComment( const eastl::string& comsig, eastl::string& str )//去掉以comsig开头的注释段
	{
		int comment = 0;
		do 
		{
			comment = str.find( comsig );
			if ( eastl::string::npos != comment )
			{
				int end = str.find( "\r\n", comment );
				end = eastl::string::npos == end ? str.size() : end;
				str.erase( comment, end - comment );
			}
		} while ( eastl::string::npos != comment );
	}
	//------------------------------------------------------------------------------------------------------------
	static void RemoveTargetString( eastl::string& str, const eastl::string& target )
	{
		do
		{
			int begin = str.find( target );
			if ( -1 == begin )
			{
				break;
			}
			else
			{
				str.erase( begin, target.size() );
			}
		}while(true);
	}
	//------------------------------------------------------------------------------------------------------------
	static eastl::string RemoveTargetString( const eastl::string& str, const eastl::string& target )
	{
		eastl::string loacl = str;
		do
		{
			int begin = loacl.find( target );
			if ( -1 == begin )
			{
				break;
			}
			else
			{
				loacl.erase( begin, target.size() );
			}
		}while(true);
		return loacl;
	}
	//------------------------------------------------------------------------------------------------------------
	static void FindIncludes( eastl::list< eastl::string >& includes, eastl::string& input )//剥离include，将include的文件push到数组中，删除原有文本中的include整个语法段
	{
		int include = 0;
		do 
		{
			include = input.find( "#include" );
			if ( eastl::string::npos != include )
			{
				int ifb = input.find( "\"", include );
				int ife = input.find( "\"", ifb+1 );
				eastl::string includefilename = input.substr( ifb+1, ife-ifb-1 );
				includes.push_back( includefilename );
				input.erase( include, ife - include + 1 );
			}
		} while ( eastl::string::npos != include );
	}
	//------------------------------------------------------------------------------------------------------------
	static bool ReplaceString(eastl::string& str, const eastl::string& from, const eastl::string& to) 
	{
		size_t start_pos = str.find(from);
		if(start_pos == eastl::string::npos)
			return false;
		str.replace(start_pos, from.length(), to);
		return true;
	}
	//------------------------------------------------------------------------------------------------------------
	static void ReplaceAllString(eastl::string& str, const eastl::string& from, const eastl::string& to) 
	{
		size_t start_pos = 0;
		do 
		{
			start_pos = str.find(from, start_pos);
			if( start_pos != eastl::string::npos )
			{
				str.replace(start_pos, from.length(), to);
				start_pos += to.size();//避免重复替换自己
			}
		} while ( start_pos != eastl::string::npos );
	}
	//------------------------------------------------------------------------------------------------------------
	static bool NotVariablePart( char c )//是否是合法的变量声明的一部分
	{
		if ( ( c >= 65 && c <= 90 ) //A~Z
			|| ( c >= 97 && c <= 122 )//a~z
			|| ( c >= 48 && c <= 57 )//0~9
			|| c == '_'
			)
		{
			return false;
		}
		return true;
	}
	//------------------------------------------------------------------------------------------------------------
	static void ReplaceAllParamterName(eastl::string& str, const eastl::string& from, const eastl::string& to, size_t begin = 0)//修改全部的参数，特殊的，需要判断是不是参数,需要完全匹配
	{
		size_t start_pos = begin;
		do 
		{
			start_pos = str.find(from, start_pos);
			if( start_pos != eastl::string::npos )
			{
				char first = start_pos == 0 ? ' ' : str[ start_pos - 1 ];
				char last = str[ start_pos + from.size() ];
				if ( NotVariablePart(last) && NotVariablePart(first) )
				{
					str.replace(start_pos, from.length(), to);
				}
				start_pos += to.size();//避免重复替换自己
			}
		} while ( start_pos != eastl::string::npos );
	}
	//------------------------------------------------------------------------------------------------------------
	static void SeparateAllString( const eastl::string& begin, const eastl::string& end,//语法段的开始函数
		eastl::vector< eastl::string >& parts, eastl::string& func )//输出的函数段数组，输出其他的文本
	{
		int define;
		do
		{
			define = func.find( begin );
			if ( eastl::string::npos != define )
			{
				int endparts = func.find( end );
				parts.push_back( func.substr( define, endparts - define + end.size() ) );
				func.erase( define, endparts - define + end.size() );
			}
		} while ( eastl::string::npos != define );
	}
	//------------------------------------------------------------------------------------------------------------
	static void SeparateString( const eastl::string& begin, const eastl::string& end,//语法段的开始函数
		eastl::string& out, eastl::string& func )//输出的函数段数组，输出其他的文本
	{
		int define = func.find( begin );
		if ( eastl::string::npos != define )
		{
			int endparts = func.find( end );
			out = func.substr( define, endparts - define + end.size() );
			func.erase( define, endparts - define + end.size() );
		}
	}
	//------------------------------------------------------------------------------------------------------------
	static bool MacroName( const eastl::string& macro, const eastl::string& func, eastl::string& name )//得到宏的实体
	{
		int define = func.find( macro );
		if ( eastl::string::npos != define )
		{
			int firstspace = func.find_first_of( ' ', define );//空格开始
			int lastspace = func.find_first_not_of( " \n\r\t", firstspace );//函数开始
			int funcend = func.find_first_of( " \r\n", lastspace );//函数结束
			name = func.substr( lastspace, funcend - lastspace );
			return true;
		}
		return false;
	}
	//------------------------------------------------------------------------------------------------------------
	static void MakeLuaFunction( const eastl::string& begin, const eastl::string& end,//语法段的开始函数
		eastl::string& func )//将材质和shader语法段
	{
		int define = func.find( begin );
		if ( eastl::string::npos != define )
		{
			int firstspace = func.find_first_of( ' ', define );//空格开始
			int lastspace = func.find_first_not_of( ' ', firstspace );//函数开始
			int funcend = func.find_first_of( " \r\n", lastspace );//函数结束
			//生成函数
			func.insert( funcend, "()" );
			ReplaceString( func, end, "end" );
			ReplaceString( func, begin, "function" );
		}
	}
	//------------------------------------------------------------------------------------------------------------
	static void MakeLuaFunction( const eastl::string& begin, const eastl::string& end,//语法段的开始函数
		eastl::string& funcname, eastl::string& func )//将材质和shader语法段改称lua函数
	{
		int define = func.find( begin );
		if ( eastl::string::npos != define )
		{
			int firstspace = func.find_first_of( ' ', define );//空格开始
			int lastspace = func.find_first_not_of( ' ', firstspace );//函数开始
			int funcend = func.find_first_of( " \r\n", lastspace );//函数结束
			funcname = func.substr( lastspace, funcend - lastspace );

			//生成函数
			func.insert( funcend, "()" );
			ReplaceString( func, end, "end" );
			ReplaceString( func, begin, "function" );
		}
	}
	////------------------------------------------------------------------------------------------------------------
	//static uint CaculateRefreshLimit( float dis, float boxdiag )
	//{
	//	float param = boxdiag / dis;
	//	return 1000 / (5 + 25 * Math::Clamp( param * 5.0f, 0.0f, 1.0f ) );
	//}
}