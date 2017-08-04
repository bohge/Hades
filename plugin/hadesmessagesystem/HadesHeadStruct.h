#pragma once
#include "core/Predefine.h"


#pragma pack(1)
namespace hms
{
	static const unsigned int FOURCC_NETPACKAGE_HEAD('h$3R');
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
			return FOURCC_NETPACKAGE_HEAD == Fourcc;
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

#pragma pack()