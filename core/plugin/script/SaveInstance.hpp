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
	void RecursiveSaveInstance(const IMemberIterator& it, IEncoder* en)
	{
		it.Being();
		while (it.Next())
		{
			switch (it.GetValueType())
			{
			case DT_NULL:
			{
							switch (it.GetKeyType())
							{
							case DT_INT:
							{
										   int name = it.GetIntField();
										   en->SetField(name);
										   break;
							}
							case DT_STRING:
							{
											  uint len;
											  const char* name = it.GetStringField(&len);
											  en->SetField(name, len);
											  break;
							}
							case DT_UINT64:
							{
											  uint64 name = it.GetUInt64Field();
											  en->SetField(name);
											  break;
							}
							default:ASSERT(false); break;
							}
							break;
			}
			case DT_BOOL:
			{							
							switch (it.GetKeyType())
							{
							case DT_INT:
							{
										   int name = it.GetIntField();
										   bool value = it.AsBool();
										   en->SetField(name, value);
										   break;
							}
							case DT_STRING:
							{
											  uint len;
											  const char* name = it.GetStringField(&len);
											  bool value = it.AsBool();
											  en->SetField(name, len, value);
											  break;
							}
							case DT_UINT64:
							{
											  uint64 name = it.GetUInt64Field();
											  bool value = it.AsBool();
											  en->SetField(name, value);
											  break;
							}
							default:ASSERT(false); break;
							}
							break;
			}
			case DT_DOUBLE:
			{
							  switch (it.GetKeyType())
							  {
							  case DT_INT:
							  {
											 int name = it.GetIntField();
											 double value = it.AsDouble();
											 en->SetField(name, value);
											 break;
							  }
							  case DT_STRING:
							  {
												uint len;
												const char* name = it.GetStringField(&len);
												double value = it.AsDouble();
												en->SetField(name, len, value);
												break;
							  }
							  case DT_UINT64:
							  {
												uint64 name = it.GetUInt64Field();
												double value = it.AsDouble();
												en->SetField(name, value);
												break;
							  }
							  default:ASSERT(false); break;
							  }
							  break;
			}
			case DT_INT64:
			{
							 switch (it.GetKeyType())
							 {
							 case DT_INT:
							 {
											int name = it.GetIntField();
											int64 value = it.AsInt64();
											en->SetField(name, value);
											break;
							 }
							 case DT_STRING:
							 {
											   uint len;
											   const char* name = it.GetStringField(&len);
											   int64 value = it.AsInt64();
											   en->SetField(name, len, value);
											   break;
							 }
							 case DT_UINT64:
							 {
											   uint64 name = it.GetUInt64Field();
											   int64 value = it.AsInt64();
											   en->SetField(name, value);
											   break;
							 }
							 default:ASSERT(false); break;
							 }
							 break;
			}
			case DT_UINT64:
			{
							  switch (it.GetKeyType())
							  {
							  case DT_INT:
							  {
											 int name = it.GetIntField();
											 uint64 value = it.AsUInt64();
											 en->SetField(name, value);
											 break;
							  }
							  case DT_STRING:
							  {
												uint len;
												const char* name = it.GetStringField(&len);
												uint64 value = it.AsUInt64();
												en->SetField(name, len, value);
												break;
							  }
							  case DT_UINT64:
							  {
												uint64 name = it.GetUInt64Field();
												uint64 value = it.AsUInt64();
												en->SetField(name, value);
												break;
							  }
							  default:ASSERT(false); break;
							  }
							  break;
			}
			case DT_STRING:
			{
							  uint len = 0;
							  switch (it.GetKeyType())
							  {
							  case DT_INT:
							  {			 
											 int name = it.GetIntField();
											 const char* value = it.AsString(&len);
											 en->SetField(name, value, len);
											 break;
							  }
							  case DT_STRING:
							  {
												uint nlen, len;
												const char* name = it.GetStringField(&nlen);
												const char* value = it.AsString(&len);
												en->SetField(name, nlen, value, len);
												break;
							  }
							  case DT_UINT64:
							  {
												uint64 name = it.GetUInt64Field();
												const char* value = it.AsString(&len);
												en->SetField(name, value, len);
												break;
							  }
							  default:ASSERT(false); break;
							  }
							  break;
			}
			case DT_NATIVE:
			{
							  switch (it.GetKeyType())
							  {
							  case DT_INT:
							  {
											 int name = it.GetIntField();
											 const Object* value = it.AsNative();
											 ASSERT(value->isTypeOrDriverType(ISerializable::RTTI()));
											 en->SetField(name, static_cast<const ISerializable*>(value));
											 break;
							  }
							  case DT_STRING:
							  {
												uint len;
												const char* name = it.GetStringField(&len);
												const Object* value = it.AsNative();
												ASSERT(value->isTypeOrDriverType(ISerializable::RTTI()));
												en->SetField(name, len, static_cast<const ISerializable*>(value));
												break;
							  }
							  case DT_UINT64:
							  {
												uint64 name = it.GetUInt64Field();
												const Object* value = it.AsNative();
												ASSERT(value->isTypeOrDriverType(ISerializable::RTTI()));
												en->SetField(name, static_cast<const ISerializable*>(value));
												break;
							  }
							  default:ASSERT(false); break;
							  }
							  break;
			}
			case DT_CLASS:
			{
							 switch (it.GetKeyType())
							 {
							 case DT_INT:
							 {
											int name = it.GetIntField();
											const IMemberIterator& son = it.AsClass();
											IEncoder* sonen = en->GetChild();
											RecursiveSaveInstance(son, sonen);
											en->SetField(name, sonen);
											break;
							 }
							 case DT_STRING:
							 {
											   uint len;
											   const char* name = it.GetStringField(&len);
											   const IMemberIterator& son = it.AsClass();
											   IEncoder* sonen = en->GetChild();
											   RecursiveSaveInstance(son, sonen);
											   en->SetField(name, len, sonen);
											   break;
							 }
							 case DT_UINT64:
							 {
											   uint64 name = it.GetUInt64Field();
											   const IMemberIterator& son = it.AsClass();
											   IEncoder* sonen = en->GetChild();
											   RecursiveSaveInstance(son, sonen);
											   en->SetField(name, sonen);
											   break;
							 }
							 default:ASSERT(false); break;
							 }
							 break;
			}
			default:ASSERT(false);
			}
		}
		it.End();
	}
}