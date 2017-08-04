#pragma once
#include "network/message/IRawbuffer.h"



namespace hms
{
	//@需要处理big,little ending的问题
	class IDIPRawbuffer : public hc::IRawbuffer
	{
	public:
		IDIPRawbuffer();
		virtual ~IDIPRawbuffer();
	private:
		virtual bool _haveValidMessage(byte* buffer, uint& length, uint& index);
		virtual uint _GetRawbufferLength(byte* buffer);
	private:
		bool _DoUnpackMessage();
	};
}