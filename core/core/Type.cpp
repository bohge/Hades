#include "core/type/FloatArray.h"
#include "core/type/Instance.h"
#include "core/type/Function.h"
#include "core/type/UShort.h"
#include "core/type/String.h"
#include "core/type/Double.h"
#include "core/type/Buffer.h"
#include "core/type/UInt64.h"
#include "core/type/Int64.h"
#include "core/type/Float.h"
#include "core/type/Short.h"
#include "core/type/Bool.h"
#include "core/type/UInt.h"
#include "core/type/Int.h"


namespace hc
{
	RTTI_TYPE_IMPLEMENT(Int);
	RTTI_TYPE_IMPLEMENT(UInt);
	RTTI_TYPE_IMPLEMENT(Bool);
	RTTI_TYPE_IMPLEMENT(Short);
	RTTI_TYPE_IMPLEMENT(Float);
	RTTI_TYPE_IMPLEMENT(Int64);
	RTTI_TYPE_IMPLEMENT(UInt64);
	RTTI_TYPE_IMPLEMENT(Double);
	RTTI_TYPE_IMPLEMENT(Buffer);
	RTTI_TYPE_IMPLEMENT(String);
	RTTI_TYPE_IMPLEMENT(UShort);	
	RTTI_TYPE_IMPLEMENT(Function);
	RTTI_TYPE_IMPLEMENT(Instance);
	RTTI_TYPE_IMPLEMENT(FloatArray);
}