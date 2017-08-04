/////////////////////////////////////
//			类成员函数回调		   //
/////////////////////////////////////
#pragma once
#include "../core/Predefine.h"


#include <EASTL/map.h>
#include <EASTL/list.h>



typedef void* CallbackHandle;

namespace hc
{
	//////////////////////////
	//		无参数			//
	//////////////////////////
	#define NAME_FIX 
	#define HAVE_COMMA 
	#define EVENT_PARAM_TEMPLATE 
	#define EVENT_PARAM_TYPES_INPUTS 
	#define EVENT_PARAM_TYPES 
	#define EVENT_PARAM_INPUTS 

	#define EVENT_IMPLECATION
	#include "../core/DelegateImplement.hpp"
	#undef EVENT_IMPLECATION
	//////////////////////////
	//		一个参数			//
	//////////////////////////
	#define NAME_FIX 1
	#define HAVE_COMMA ,
	#define EVENT_PARAM_TEMPLATE typename Arg
	#define EVENT_PARAM_TYPES_INPUTS Arg arg
	#define EVENT_PARAM_TYPES Arg
	#define EVENT_PARAM_INPUTS arg

	#define EVENT_IMPLECATION
	#include "../core/DelegateImplement.hpp"
	#undef EVENT_IMPLECATION
	//////////////////////////
	//		两个参数			//
	//////////////////////////

	#define NAME_FIX 2
	#define HAVE_COMMA ,
	#define EVENT_PARAM_TEMPLATE typename Arg1, typename Arg2
	#define EVENT_PARAM_TYPES_INPUTS Arg1 arg1, Arg2 arg2
	#define EVENT_PARAM_TYPES Arg1, Arg2
	#define EVENT_PARAM_INPUTS arg1, arg2

	#define EVENT_IMPLECATION
	#include "../core/DelegateImplement.hpp"
	#undef EVENT_IMPLECATION


	//////////////////////////
	//		三个参数			//
	//////////////////////////

	#define NAME_FIX 3
	#define HAVE_COMMA ,
	#define EVENT_PARAM_TEMPLATE typename Arg1, typename Arg2, typename Arg3
	#define EVENT_PARAM_TYPES_INPUTS Arg1 arg1, Arg2 arg2, Arg3 arg3
	#define EVENT_PARAM_TYPES Arg1, Arg2, Arg3
	#define EVENT_PARAM_INPUTS arg1, arg2, arg3

	#define EVENT_IMPLECATION
	#include "../core/DelegateImplement.hpp"
	#undef EVENT_IMPLECATION


	//////////////////////////
	//		 四个参数		//
	//////////////////////////

	#define NAME_FIX 4
	#define HAVE_COMMA ,
	#define EVENT_PARAM_TEMPLATE typename Arg1, typename Arg2, typename Arg3, typename Arg4
	#define EVENT_PARAM_TYPES_INPUTS Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4
	#define EVENT_PARAM_TYPES Arg1, Arg2, Arg3, Arg4
	#define EVENT_PARAM_INPUTS arg1, arg2, arg3, arg4

	#define EVENT_IMPLECATION
	#include "../core/DelegateImplement.hpp"
	#undef EVENT_IMPLECATION


	//////////////////////////
	//		五个参数			//
	//////////////////////////

	#define NAME_FIX 5
	#define HAVE_COMMA ,
	#define EVENT_PARAM_TEMPLATE typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5
	#define EVENT_PARAM_TYPES_INPUTS Arg1 arg1, Arg2 arg2, Arg3 arg3, Arg4 arg4, Arg5 arg5
	#define EVENT_PARAM_TYPES Arg1, Arg2, Arg3, Arg4, Arg5
	#define EVENT_PARAM_INPUTS arg1, arg2, arg3, arg4, arg5

	#define EVENT_IMPLECATION
	#include "../core/DelegateImplement.hpp"
	#undef EVENT_IMPLECATION


	//成员函数指针
	#ifdef _TRACK_MEMORY
		#define MEMBER_FUNC_PTR(x) make_bind( this, x, __FILE__, __LINE__ )
	#else
		#define MEMBER_FUNC_PTR(x) make_bind( this, x )
	#endif
}