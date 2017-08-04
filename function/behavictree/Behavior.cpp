#include "serialize/FactorConfigure.h"
#include "BehavictreePlugin.h"
#include "CommonAgent.h"
#include "Behavior.h"




namespace hbtf
{
	RTTI_TYPE_IMPLEMENT_WITH_FACTOR(Behavior, FACTOR_BEHAVIOR);
	//---------------------------------------------------------------------------------------------------------
	Behavior::Behavior()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	Behavior::~Behavior()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	const byte* Behavior::GetBuffer() const
	{
		return (byte*)&m_Data;
	}
	//---------------------------------------------------------------------------------------------------------
	uint Behavior::GetLength() const
	{
		return sizeof(m_Data);
	}
	//---------------------------------------------------------------------------------------------------------
	void Behavior::FromBuffer(const byte* buff, uint len)
	{
		SerData* sd = (SerData*)buff;
		Initialize(sd->Index, sd->Templatehash);
	}
	//---------------------------------------------------------------------------------------------------------
	void Behavior::Initialize(uint64 index, uint hash)
	{
		m_Data.Index = index;
		m_Data.Templatehash = hash;
		m_spCommonAgent = BehavictreePlugin::Instance()->GetAgent(index);
		m_rpTreeTemplate = BehavictreePlugin::Instance()->GetTreeTemplate(hash);
	}
	//---------------------------------------------------------------------------------------------------------
	bool Behavior::Update(hc::IScriptInstance* response)
	{
		if (m_spCommonAgent.isValid())
		{
			m_spCommonAgent->Setup(response, m_rpTreeTemplate);
			m_spCommonAgent->btexec();
			return true;
		}		
		return false;
	}
}