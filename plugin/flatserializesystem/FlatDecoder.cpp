#include "serialize/ISerializeSystem.h"
#include "serialize/ISerializable.h"
#include "FlatSerializer.h"
#include "FlatDecoder.h"


using namespace hc;
namespace hfbs
{
	//---------------------------------------------------------------------------------------------------------
	FlatDecoder::FlatDecoder(FlatSerializer* ser)
		: m_rpFlatSerializer(ser)
		, m_Size(0)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	FlatDecoder::~FlatDecoder()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void FlatDecoder::Initializer(const TClass* c)
	{
		auto pmembers = c->members();
		if (NULL != pmembers)
		{
			m_Size = pmembers->size();
			for (unsigned int i = 0; i < pmembers->size(); ++i)
			{
				auto member = pmembers->Get(i);
				m_MemberVector.push_back(member);
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	uint FlatDecoder::GetLength() const
	{
		return m_Size;
	}
	//---------------------------------------------------------------------------------------------------------
	const char* FlatDecoder::GetStringField(int index) const
	{
		auto member = m_MemberVector[index];
		auto tf = static_cast<const TString*>(member->key());
		return tf->value()->c_str();
	}
	//---------------------------------------------------------------------------------------------------------
	int FlatDecoder::GetIntField(int index) const
	{
		auto member = m_MemberVector[index];
		auto tf = static_cast<const TInt*>(member->key());
		return tf->value();
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 FlatDecoder::GetUInt64Field(int index) const
	{
		auto member = m_MemberVector[index];
		auto tf = static_cast<const TUInt64*>(member->key());
		return tf->value();
	}
	//---------------------------------------------------------------------------------------------------------
	hc::DataType FlatDecoder::GetKeyType(int index) const
	{
		auto member = m_MemberVector[index];
		return static_cast<hc::DataType>(member->key_type() - 1);//看两个头文件
	}
	//---------------------------------------------------------------------------------------------------------
	hc::DataType FlatDecoder::GetValueType(int index) const
	{
		auto member = m_MemberVector[index];
		return static_cast<hc::DataType>(member->value_type() - 1);//看两个头文件
	}
	//---------------------------------------------------------------------------------------------------------
	bool FlatDecoder::AsBool(int index) const
	{
		auto member = m_MemberVector[index];
		auto tf = static_cast<const TBool*>(member->value());
		return tf->value();
	}
	//---------------------------------------------------------------------------------------------------------
	int FlatDecoder::AsInt(int index) const
	{
		auto member = m_MemberVector[index];
		auto tf = static_cast<const TInt*>(member->value());
		return tf->value();
	}
	//---------------------------------------------------------------------------------------------------------
	float FlatDecoder::AsFloat(int index) const
	{
		auto member = m_MemberVector[index];
		auto tf = static_cast<const TFloat*>(member->value());
		return tf->value();
	}
	//---------------------------------------------------------------------------------------------------------
	const char* FlatDecoder::AsString(int index, uint* out_len) const
	{
		auto member = m_MemberVector[index];
		auto tf = static_cast<const TString*>(member->value());
		*out_len = tf->value()->Length();
		return tf->value()->c_str();
	}
	//---------------------------------------------------------------------------------------------------------
	short FlatDecoder::AsShort(int index) const
	{
		auto member = m_MemberVector[index];
		auto tf = static_cast<const TShort*>(member->value());
		return tf->value();
	}
	//---------------------------------------------------------------------------------------------------------
	ushort FlatDecoder::AsUShort(int index) const
	{
		auto member = m_MemberVector[index];
		auto tf = static_cast<const TUShort*>(member->value());
		return tf->value();
	}
	//---------------------------------------------------------------------------------------------------------
	uint FlatDecoder::AsUInt(int index) const
	{
		auto member = m_MemberVector[index];
		auto tf = static_cast<const TUInt*>(member->value());
		return tf->value();
	}
	//---------------------------------------------------------------------------------------------------------
	int64 FlatDecoder::AsInt64(int index) const
	{
		auto member = m_MemberVector[index];
		auto tf = static_cast<const TInt64*>(member->value());
		return tf->value();
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 FlatDecoder::AsUInt64(int index) const
	{
		auto member = m_MemberVector[index];
		auto tf = static_cast<const TUInt64*>(member->value());
		return tf->value();
	}
	//---------------------------------------------------------------------------------------------------------
	double FlatDecoder::AsDouble(int index) const
	{
		auto member = m_MemberVector[index];
		auto tf = static_cast<const TDouble*>(member->value());
		return tf->value();
	}
	//---------------------------------------------------------------------------------------------------------
	ISerializable* FlatDecoder::AsNative(int index) const
	{
		auto member = m_MemberVector[index];
		auto tn = static_cast<const TNative*>(member->value());
		int f = tn->factor();
		ISerializable* res = ISerializeSystem::Instance()->GetSerializable(f);	
		if (!res->FromBuffer(tn->value()->Data(), tn->value()->Length()))
		{
			ISerializeSystem::Instance()->FreeSerializable(res);
			res = NULL;
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	const hc::IDecoder* FlatDecoder::AsClass(int index) const
	{		
		auto member = m_MemberVector[index];
		auto son = static_cast<const TClass*>(member->value());
		hc::IDecoder* res = m_rpFlatSerializer->GetDecoder(son);
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void FlatDecoder::Clear()
	{
		m_Size = 0;
		m_MemberVector.clear();
	}
}