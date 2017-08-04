#pragma once
#include "serialize/ISerializable.h"
#include "BehavictreePlugin.h"



class CommonAgent;
namespace hc
{
	class IScriptInstance;
}
namespace hbtf
{
	class TreeTemplate;
	class Behavior : public hc::ISerializable
	{		
		SERIALIZABL_TYPE_DECLEAR(Behavior);
		RTTI_DRIVER_TYPE_DECLEAR(Behavior, hc::ISerializable);
	public:
		enum BTStatus
		{
			BT_INVALID = 0,
			BT_SUCCESS,
			BT_FAILURE,
			BT_RUNNING,
		};
	private:
		struct SerData
		{
			uint64	Index;
			uint	Templatehash;
		};
	private:
		hc::SmartPtr<CommonAgent>	m_spCommonAgent;
		SerData						m_Data;
		TreeTemplate*				m_rpTreeTemplate;
	public:
		Behavior();
		~Behavior();
	public:
		virtual const byte* GetBuffer() const;
		virtual uint GetLength() const;
		virtual void FromBuffer(const byte* buff, uint len);
	public:
		void Initialize(uint64 index, uint hash);
		bool Update(hc::IScriptInstance* response);
	public:
		HADES_FORCEINLINE uint64 GetIndex();		
	};
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE uint64 Behavior::GetIndex()
	{
		return m_Data.Index;
	}
}