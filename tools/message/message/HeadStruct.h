#pragma once
#include "Configure.h"

namespace hc
{
	struct HeadStruct
	{
	public:
		uint		Fourcc;
		uint		RawLength;
		uint		ID;
	public:
		HADES_FORCEINLINE static uint GetHeadSize()
		{
			return sizeof(HeadStruct);
		}
	public:
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE bool isValid() const
		{
			return Configure::FOURCC_NETPACKAGE_HEAD == Fourcc;
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE uint GetID() const
		{
			return ID;
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE uint GetMessageLength() const
		{
			return RawLength - sizeof(HeadStruct);
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE uint GetRawLength() const
		{
			return RawLength;
		}
	};
}