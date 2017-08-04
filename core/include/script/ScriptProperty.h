#pragma once
#include <EASTL/string.h>
#include <EASTL/vector.h>

namespace hc
{
	class Rtti;
	namespace ScriptProperty
	{
		typedef eastl::vector<unsigned int>	Arguments;
#define BOOL_RTTI_ID (903450675)
#define SHORT_RTTI_ID (595743710)
#define USHORT_RTTI_ID (2151561849)
#define INT_RTTI_ID (610313007)
#define UINT_RTTI_ID (2535670169)
#define FLOAT_RTTI_ID (1787541143)
#define DOUBLE_RTTI_ID (3415391299)
#define STRING_RTTI_ID (3819627515)
#define FUNCTION_RTTI_ID (2368381682)
#define INSTANCE_RTTI_ID (455858718)
#define BUFFER_RTTI_ID (2127466062)
#define INT64_RTTI_ID (2510899428)
#define UINT64_RTTI_ID (4066825566)
#define FLOATARRAY_RTTI_ID (2737994191)
#define ARGUMENTS_RTTI_ID (2485735940)

		//默认的重载操作符
		static const char* __GET("__get");
		static const char* __SET("__set");
		static const char* __STR("__tostring");
		static const char* __CON("__construct");
		static const char* __ADD("__operator+");
		static const char* __SUB("__operator-");
		static const char* __MUL("__operator*");
		static const char* __DIV("__operator/");
		static const char* __EQU("__operator==");
		static const char* __LES("__operator<");
		static const char* __LEQ("__operator<=");
		static const char* __CAT("__concatenation");

		//类成员变量
		static const char SCRIPT_INERL_PREFIX('_');
		static const char* SCRIPT_ACCESSOR_GETER("_GID");
		static const char* SCRIPT_SELF_HANDLE("_HandleSelf");

		static const char* SCRIPT_TYPE_BOOL("TBool");
		static const char* SCRIPT_TYPE_SHORT("TShort");
		static const char* SCRIPT_TYPE_USHORT("TUShort");
		static const char* SCRIPT_TYPE_INT("TInt");
		static const char* SCRIPT_TYPE_UINT("TUInt");
		static const char* SCRIPT_TYPE_INT64("TInt64");
		static const char* SCRIPT_TYPE_UINT64("TUInt64");
		static const char* SCRIPT_TYPE_FLOAT("TFloat");
		static const char* SCRIPT_TYPE_DOUBLE("TDouble");
		static const char* SCRIPT_TYPE_STRING("TString");
		static const char* SCRIPT_TYPE_NATIVE("TNative");

#define SCRIPT_TYPE_BOOL_VALUE (1)
#define SCRIPT_TYPE_SHORT_VALUE (2)
#define SCRIPT_TYPE_USHORT_VALUE (3)
#define SCRIPT_TYPE_INT_VALUE (4)
#define SCRIPT_TYPE_UINT_VALUE (5)
#define SCRIPT_TYPE_FLOAT_VALUE (6)
#define SCRIPT_TYPE_DOUBLE_VALUE (7)
#define SCRIPT_TYPE_STRING_VALUE (8)
#define SCRIPT_TYPE_NATIVE_VALUE (9)
#define SCRIPT_TYPE_INT64_VALUE (10)
#define SCRIPT_TYPE_UINT64_VALUE (11)

		enum FunctionType
		{
			FT_MEMBER = 0,//成员函数
			FT_STATIC,//静态函数
		};
		
	}
}