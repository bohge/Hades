#pragma once
#include "type/StringBuffer.h"
#include "type/FloatArray.h"
#include "type/Instance.h"
#include "type/Function.h"
#include "type/UShort.h"
#include "type/String.h"
#include "type/Double.h"
#include "type/Buffer.h"
#include "type/UInt64.h"
#include "type/Int64.h"
#include "type/Float.h"
#include "type/Short.h"
#include "type/Bool.h"
#include "type/UInt.h"
#include "type/Int.h"






namespace hc
{
	enum DataType
	{
		DT_NULL = 0,
		DT_BOOL,
		DT_SHORT,
		DT_USHORT,
		DT_INT,
		DT_UINT,
		DT_FLOAT,
		DT_DOUBLE,
		DT_STRING,
		DT_NATIVE,
		DT_CLASS,		
		DT_INT64,
		DT_UINT64,
		DT_BUFFER,
		DT_FLOAT_ARRAY,
	};
}