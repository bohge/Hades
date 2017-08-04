#include "script/ScriptObjectpool.h"
#include "script/IScriptSystem.h"
#include "script/ScriptValue.h"
#include "core/Types.h"

#include <EASTL/string.h>

namespace hc
{
	//---------------------------------------------------------------------------------------------------------
	void* ShortAllocer()
	{
		return NEW short;
	}
	//---------------------------------------------------------------------------------------------------------
	void* UShortAllocer()
	{
		return NEW ushort;
	}
	//---------------------------------------------------------------------------------------------------------
	void* IntAllocer()
	{
		return NEW int;
	}
	//---------------------------------------------------------------------------------------------------------
	void* UIntAllocer()
	{
		return NEW uint;
	}
	//---------------------------------------------------------------------------------------------------------
	void* FloatAllocer()
	{
		return NEW float;
	}
	//---------------------------------------------------------------------------------------------------------
	void* DoubleAllocer()
	{
		return NEW double;
	}
	//---------------------------------------------------------------------------------------------------------
	void* BoolAllocer()
	{
		return NEW bool;
	}
	//---------------------------------------------------------------------------------------------------------
	void* StringPushAllocer()
	{
		return NEW eastl::string;
	}
	//---------------------------------------------------------------------------------------------------------
	void* StringReadAllocer()
	{
		return NEW hc::StringBuffer;
	}
	//---------------------------------------------------------------------------------------------------------
	void* BufferAllocer()
	{
		return NEW hc::StringBuffer;
	}
	//---------------------------------------------------------------------------------------------------------
	void* Int64Allocer()
	{
		return NEW int64;
	}
	//---------------------------------------------------------------------------------------------------------
	void* UInt64Allocer()
	{
		return NEW uint64;
	}
	//---------------------------------------------------------------------------------------------------------
	void* FloatArrayAllocer()
	{
		return NEW hc::ScriptValue();
	}
	//---------------------------------------------------------------------------------------------------------
	void* InstanceAllocer()
	{
		return NEW int(0);
	}
	//---------------------------------------------------------------------------------------------------------
	void* FunctionAllocer()
	{
		return NEW int(0);
	}
	//---------------------------------------------------------------------------------------------------------
	void* ArgumentsAllocer()
	{
		return IScriptSystem::Instance()->AllocArguments();
	}
	//---------------------------------------------------------------------------------------------------------


	//---------------------------------------------------------------------------------------------------------
	void ShortFreer(void* obj)
	{
		short* tag = static_cast<short*>(obj);
		SAFE_DELETE(tag);
	}
	//---------------------------------------------------------------------------------------------------------
	void UShortFreer(void* obj)
	{
		ushort* tag = static_cast<ushort*>(obj);
		SAFE_DELETE(tag);
	}
	//---------------------------------------------------------------------------------------------------------
	void IntFreer(void* obj)
	{
		int* tag = static_cast<int*>(obj);
		SAFE_DELETE(tag);
	}
	//---------------------------------------------------------------------------------------------------------
	void UIntFreer(void* obj)
	{
		uint* tag = static_cast<uint*>(obj);
		SAFE_DELETE(tag);
	}
	//---------------------------------------------------------------------------------------------------------
	void FloatFreer(void* obj)
	{
		float* tag = static_cast<float*>(obj);
		SAFE_DELETE(tag);
	}
	//---------------------------------------------------------------------------------------------------------
	void DoubleFreer(void* obj)
	{
		double* tag = static_cast<double*>(obj);
		SAFE_DELETE(tag);
	}
	//---------------------------------------------------------------------------------------------------------
	void BoolFreer(void* obj)
	{
		bool* tag = static_cast<bool*>(obj);
		SAFE_DELETE(tag);
	}
	//---------------------------------------------------------------------------------------------------------
	void StringPushFreer(void* obj)
	{
		eastl::string* tag = static_cast<eastl::string*>(obj);
		SAFE_DELETE(tag);
	}
	//---------------------------------------------------------------------------------------------------------
	void StringReadFreer(void* obj)
	{
		hc::StringBuffer* tag = static_cast<hc::StringBuffer*>(obj);
		SAFE_DELETE(tag);
	}
	//---------------------------------------------------------------------------------------------------------
	void BufferFreer(void* obj)
	{
		hc::StringBuffer* tag = static_cast<hc::StringBuffer*>(obj);
		SAFE_DELETE(tag);
	}
	//---------------------------------------------------------------------------------------------------------
	void Int64Freer(void* obj)
	{
		int64* tag = static_cast<int64*>(obj);
		SAFE_DELETE(tag);
	}
	//---------------------------------------------------------------------------------------------------------
	void UInt64Freer(void* obj)
	{
		uint64* tag = static_cast<uint64*>(obj);
		SAFE_DELETE(tag);
	}
	//---------------------------------------------------------------------------------------------------------
	void FloatArrayFreer(void* obj)
	{
		hc::ScriptValue* tag = static_cast<hc::ScriptValue*>(obj);
		SAFE_DELETE(tag);
	}
	//---------------------------------------------------------------------------------------------------------
	void InstanceFreer(void* obj)
	{
		int* tag = static_cast<int*>(obj);
		SAFE_DELETE(tag);
	}
	//---------------------------------------------------------------------------------------------------------
	void FunctionFreer(void* obj)
	{
		int* tag = static_cast<int*>(obj);
		SAFE_DELETE(tag);
	}
	//---------------------------------------------------------------------------------------------------------
	void ArgumentsFreer(void* obj)
	{
		IArguments* arg = static_cast<IArguments*>(obj);
		IScriptSystem::Instance()->FreeArguments(arg);
	}

	SINGLETON_IMPLEMENT(ScriptObjectpool);
	//---------------------------------------------------------------------------------------------------------
	ScriptObjectpool::ScriptObjectpool()
	{
		m_pResultPoolDataStructArray = NEW ResultPoolDataStruct[POOL_INDEX_COUNT];
		m_pResultPoolDataStructArray[POOL_INDEX_SHORT].m_ObjectAllocer = &ShortAllocer;
		m_pResultPoolDataStructArray[POOL_INDEX_SHORT].m_ObjectFreer = &ShortFreer;
		m_pResultPoolDataStructArray[POOL_INDEX_USHORT].m_ObjectAllocer = &UShortAllocer;
		m_pResultPoolDataStructArray[POOL_INDEX_USHORT].m_ObjectFreer = &UShortFreer;
		m_pResultPoolDataStructArray[POOL_INDEX_INT].m_ObjectAllocer = &IntAllocer;
		m_pResultPoolDataStructArray[POOL_INDEX_INT].m_ObjectFreer = &IntFreer;
		m_pResultPoolDataStructArray[POOL_INDEX_UINT].m_ObjectAllocer = &UIntAllocer;
		m_pResultPoolDataStructArray[POOL_INDEX_UINT].m_ObjectFreer = &UIntFreer;
		m_pResultPoolDataStructArray[POOL_INDEX_FLOAT].m_ObjectAllocer = &FloatAllocer;
		m_pResultPoolDataStructArray[POOL_INDEX_FLOAT].m_ObjectFreer = &FloatFreer;
		m_pResultPoolDataStructArray[POOL_INDEX_DOUBLE].m_ObjectAllocer = &DoubleAllocer;
		m_pResultPoolDataStructArray[POOL_INDEX_DOUBLE].m_ObjectFreer = &DoubleFreer;
		m_pResultPoolDataStructArray[POOL_INDEX_BOOL].m_ObjectAllocer = &BoolAllocer;
		m_pResultPoolDataStructArray[POOL_INDEX_BOOL].m_ObjectFreer = &BoolFreer;
		m_pResultPoolDataStructArray[POOL_INDEX_STRING_PUSH].m_ObjectAllocer = &StringPushAllocer;
		m_pResultPoolDataStructArray[POOL_INDEX_STRING_PUSH].m_ObjectFreer = &StringPushFreer;
		m_pResultPoolDataStructArray[POOL_INDEX_STRING_READ].m_ObjectAllocer = &StringReadAllocer;
		m_pResultPoolDataStructArray[POOL_INDEX_STRING_READ].m_ObjectFreer = &StringReadFreer;
		m_pResultPoolDataStructArray[POOL_INDEX_BUFFER].m_ObjectAllocer = &BufferAllocer;
		m_pResultPoolDataStructArray[POOL_INDEX_BUFFER].m_ObjectFreer = &BufferFreer;
		m_pResultPoolDataStructArray[POOL_INDEX_INT64].m_ObjectAllocer = &Int64Allocer;
		m_pResultPoolDataStructArray[POOL_INDEX_INT64].m_ObjectFreer = &Int64Freer;
		m_pResultPoolDataStructArray[POOL_INDEX_UINT64].m_ObjectAllocer = &UInt64Allocer;
		m_pResultPoolDataStructArray[POOL_INDEX_UINT64].m_ObjectFreer = &UInt64Freer;
		m_pResultPoolDataStructArray[POOL_INDEX_FLOATARRAY].m_ObjectAllocer = &FloatArrayAllocer;
		m_pResultPoolDataStructArray[POOL_INDEX_FLOATARRAY].m_ObjectFreer = &FloatArrayFreer;
		m_pResultPoolDataStructArray[POOL_INDEX_INSTANCE].m_ObjectAllocer = &InstanceAllocer;
		m_pResultPoolDataStructArray[POOL_INDEX_INSTANCE].m_ObjectFreer = &InstanceFreer;
		m_pResultPoolDataStructArray[POOL_INDEX_FUNCTION].m_ObjectAllocer = &FunctionAllocer;
		m_pResultPoolDataStructArray[POOL_INDEX_FUNCTION].m_ObjectFreer = &FunctionFreer;
		m_pResultPoolDataStructArray[POOL_INDEX_ARGUMENTS].m_ObjectAllocer = &ArgumentsAllocer;
		m_pResultPoolDataStructArray[POOL_INDEX_ARGUMENTS].m_ObjectFreer = &ArgumentsFreer;


		for (int i = 0; i < POOL_INDEX_COUNT; ++i)
		{
			ResultPoolDataStruct& datastr = m_pResultPoolDataStructArray[i];
			for (int j = 0; j < POOL_PREPARE_COUNT; ++j)
			{
				void* data = datastr.m_ObjectAllocer();
				datastr.m_ObjectQueue.enqueue(data);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	ScriptObjectpool::~ScriptObjectpool()
	{
		for (int i = 0; i < POOL_INDEX_COUNT; ++i)
		{
			ResultPoolDataStruct& str = m_pResultPoolDataStructArray[i];
			void* res = NULL;
			while (str.m_ObjectQueue.try_dequeue(res))
			{
				str.m_ObjectFreer(res);
			}
		}
		SAFE_DELETE_ARRAY(m_pResultPoolDataStructArray);
	}
	//---------------------------------------------------------------------------------------------------------
	void* ScriptObjectpool::GetObject(uint index)
	{
		void* res = NULL;
		if (POOL_INDEX_EMPTY != index)
		{
			ResultPoolDataStruct& str = m_pResultPoolDataStructArray[index];
			if (!str.m_ObjectQueue.try_dequeue(res))
			{
				res = str.m_ObjectAllocer();
			}
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void ScriptObjectpool::FreeObject(uint index, void* obj)
	{
		if (POOL_INDEX_EMPTY != index)
		{
			ASSERT(NULL != obj);
			ResultPoolDataStruct& str = m_pResultPoolDataStructArray[index];
			str.m_ObjectQueue.enqueue(obj);
		}
	}
}
