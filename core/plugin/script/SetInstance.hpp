#include "serialize/ISerializable.h"
#include "script/IScriptInstance.h"
#include "serialize/ISerializer.h"
#include "serialize/IDecoder.h"
#include "serialize/IEncoder.h"
#include "core/Types.h"




using namespace eastl;
namespace hc
{
	//------------------------------------------------------------------------------------------------------
	void RecursiveSetInstance(IScriptInstance* instance, const IDecoder* de)
	{
		int size = de->GetLength();
		instance->BeginSetMemberBulk();
		for (int i = 0; i < size; ++i)
		{
			switch (de->GetValueType(i))
			{
			case DT_NULL:
			{
							switch (de->GetKeyType(i))
							{
							case DT_INT:
							{
										   int name = de->GetIntField(i);
										   instance->ContinuouslySetMember(name);
										   break;
							}
							case DT_STRING:
							{
											  const char* name = de->GetStringField(i);
											  instance->ContinuouslySetMember(name);
											  break;
							}
							case DT_UINT64:
							{
											  uint64 name = de->GetUInt64Field(i);
											  instance->ContinuouslySetMember(name);
											  break;
							}
							default:ASSERT(false); break;
							}
							break;
			}
			case DT_BOOL:
			{
							bool value = de->AsBool(i);
							switch (de->GetKeyType(i))
							{
							case DT_INT:
							{
										   int name = de->GetIntField(i);
										   instance->ContinuouslySetMember(name, value);
										   break;
							}
							case DT_STRING:
							{
											  const char* name = de->GetStringField(i);
											  instance->ContinuouslySetMember(name, value);
											  break;
							}
							case DT_UINT64:
							{
											  uint64 name = de->GetUInt64Field(i);
											  instance->ContinuouslySetMember(name, value);
											  break;
							}
							default:ASSERT(false); break;
							}
							break;
			}
			case DT_SHORT:
			{
							 short value = de->AsShort(i);
							 switch (de->GetKeyType(i))
							 {
							 case DT_INT:
							 {
											int name = de->GetIntField(i);
											instance->ContinuouslySetMember(name, value);
											break;
							 }
							 case DT_STRING:
							 {
											   const char* name = de->GetStringField(i);
											   instance->ContinuouslySetMember(name, value);
											   break;
							 }
							 case DT_UINT64:
							 {
											   uint64 name = de->GetUInt64Field(i);
											   instance->ContinuouslySetMember(name, value);
											   break;
							 }
							 default:ASSERT(false); break;
							 }
							 break;
			}
			case DT_USHORT:
			{
							  ushort value = de->AsUShort(i);
							  switch (de->GetKeyType(i))
							  {
							  case DT_INT:
							  {
											 int name = de->GetIntField(i);
											 instance->ContinuouslySetMember(name, value);
											 break;
							  }
							  case DT_STRING:
							  {
												const char* name = de->GetStringField(i);
												instance->ContinuouslySetMember(name, value);
												break;
							  }
							  case DT_UINT64:
							  {
												uint64 name = de->GetUInt64Field(i);
												instance->ContinuouslySetMember(name, value);
												break;
							  }
							  default:ASSERT(false); break;
							  }
							  break;
			}
			case DT_INT:
			{
						   int value = de->AsInt(i);
						   switch (de->GetKeyType(i))
						   {
						   case DT_INT:
						   {
										  int name = de->GetIntField(i);
										  instance->ContinuouslySetMember(name, value);
										  break;
						   }
						   case DT_STRING:
						   {
											 const char* name = de->GetStringField(i);
											 instance->ContinuouslySetMember(name, value);
											 break;
						   }
						   case DT_UINT64:
						   {
											 uint64 name = de->GetUInt64Field(i);
											 instance->ContinuouslySetMember(name, value);
											 break;
						   }
						   default:ASSERT(false); break;
						   }
						   break;
			}
			case DT_UINT:
			{
							uint value = de->AsUInt(i);
							switch (de->GetKeyType(i))
							{
							case DT_INT:
							{
										   int name = de->GetIntField(i);
										   instance->ContinuouslySetMember(name, value);
										   break;
							}
							case DT_STRING:
							{
											  const char* name = de->GetStringField(i);
											  instance->ContinuouslySetMember(name, value);
											  break;
							}
							case DT_UINT64:
							{
											  uint64 name = de->GetUInt64Field(i);
											  instance->ContinuouslySetMember(name, value);
											  break;
							}
							default:ASSERT(false); break;
							}
							break;
			}
			case DT_FLOAT:
			{
							 float value = de->AsFloat(i);
							 switch (de->GetKeyType(i))
							 {
							 case DT_INT:
							 {
											int name = de->GetIntField(i);
											instance->ContinuouslySetMember(name, value);
											break;
							 }
							 case DT_STRING:
							 {
											   const char* name = de->GetStringField(i);
											   instance->ContinuouslySetMember(name, value);
											   break;
							 }
							 case DT_UINT64:
							 {
											   uint64 name = de->GetUInt64Field(i);
											   instance->ContinuouslySetMember(name, value);
											   break;
							 }
							 default:ASSERT(false); break;
							 }
							 break;
			}
			case DT_DOUBLE:
			{
							  double value = de->AsDouble(i);
							  switch (de->GetKeyType(i))
							  {
							  case DT_INT:
							  {
											 int name = de->GetIntField(i);
											 instance->ContinuouslySetMember(name, value);
											 break;
							  }
							  case DT_STRING:
							  {
												const char* name = de->GetStringField(i);
												instance->ContinuouslySetMember(name, value);
												break;
							  }
							  case DT_UINT64:
							  {
												uint64 name = de->GetUInt64Field(i);
												instance->ContinuouslySetMember(name, value);
												break;
							  }
							  default:ASSERT(false); break;
							  }
							  break;
			}
			case DT_INT64:
			{
							 int64 value = de->AsInt64(i);
							 switch (de->GetKeyType(i))
							 {
							 case DT_INT:
							 {
											int name = de->GetIntField(i);
											instance->ContinuouslySetMember(name, value);
											break;
							 }
							 case DT_STRING:
							 {
											   const char* name = de->GetStringField(i);
											   instance->ContinuouslySetMember(name, value);
											   break;
							 }
							 case DT_UINT64:
							 {
											   uint64 name = de->GetUInt64Field(i);
											   instance->ContinuouslySetMember(name, value);
											   break;
							 }
							 default:ASSERT(false); break;
							 }
							 break;
			}
			case DT_UINT64:
			{
							  uint64 value = de->AsUInt64(i);
							  switch (de->GetKeyType(i))
							  {
							  case DT_INT:
							  {
											 int name = de->GetIntField(i);
											 instance->ContinuouslySetMember(name, value);
											 break;
							  }
							  case DT_STRING:
							  {
												const char* name = de->GetStringField(i);
												instance->ContinuouslySetMember(name, value);
												break;
							  }
							  case DT_UINT64:
							  {
												uint64 name = de->GetUInt64Field(i);
												instance->ContinuouslySetMember(name, value);
												break;
							  }
							  default:ASSERT(false); break;
							  }
							  break;
			}
			case DT_STRING:
			{
							  uint len = 0;
							  const char* value = de->AsString(i, &len);
							  switch (de->GetKeyType(i))
							  {
							  case DT_INT:
							  {
											 int name = de->GetIntField(i);
											 instance->ContinuouslySetMember(name, value, len);
											 break;
							  }
							  case DT_STRING:
							  {
												const char* name = de->GetStringField(i);
												instance->ContinuouslySetMember(name, value, len);
												break;
							  }
							  case DT_UINT64:
							  {
												uint64 name = de->GetUInt64Field(i);
												instance->ContinuouslySetMember(name, value, len);
												break;
							  }
							  default:ASSERT(false); break;
							  }
							  break;
			}
			case DT_NATIVE:
			{
							  ISerializable* value = de->AsNative(i);
							  switch (de->GetKeyType(i))
							  {
							  case DT_INT:
							  {
											 int name = de->GetIntField(i);
											 instance->ContinuouslySetMember(name, value, value->GetClearFunction());
											 break;
							  }
							  case DT_STRING:
							  {
												const char* name = de->GetStringField(i);
												instance->ContinuouslySetMember(name, value, value->GetClearFunction());
												break;
							  }
							  case DT_UINT64:
							  {
												uint64 name = de->GetUInt64Field(i);
												instance->ContinuouslySetMember(name, value, value->GetClearFunction());
												break;
							  }
							  default:ASSERT(false); break;
							  }
							  break;
			}
			case DT_CLASS:
			{
							 const IDecoder* sonde = de->AsClass(i);
							 switch (de->GetKeyType(i))
							 {
							 case DT_INT:
							 {
											int name = de->GetIntField(i);
											IScriptInstance* chiled = instance->CreateChieldInstance(name);
											RecursiveSetInstance(chiled, sonde);
											break;
							 }
							 case DT_STRING:
							 {
											   const char* name = de->GetStringField(i);
											   IScriptInstance* chiled = instance->CreateChieldInstance(name);
											   RecursiveSetInstance(chiled, sonde);
											   break;
							 }
							 case DT_UINT64:
							 {
											   uint64 name = de->GetUInt64Field(i);
											   IScriptInstance* chiled = instance->CreateChieldInstance(name);
											   RecursiveSetInstance(chiled, sonde);
											   break;
							 }
							 default:ASSERT(false); break;
							 }
							 break;
			}
			default:ASSERT(false && "unkown data type");
			}
		}
		instance->EndSetMemberBulk();
	}
}