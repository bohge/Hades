#pragma once
#include "core/Configure.h"
#include "core/Object.hpp"



namespace hc
{
	//@需要处理big,little ending的问题
	class IMessage;
	class HADES_CORE_API IRawbuffer : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IRawbuffer);
	private:
		enum Constant
		{
			RBC_MAX_ERROR_COUNT = 1,
		};
	private:
		const uint	m_HeadSize;
		uint		m_ErrorCount;
		uint		m_BufferSize;//当前池总大小
		uint		m_Length;//有效长度
		uint		m_Position;//当前末尾位置
		uint		m_Index;//当前拆分位置
		byte*		m_pBuffer;
	public:
		IRawbuffer(uint headsize);
		virtual ~IRawbuffer();
	private:
		virtual bool _haveValidMessage(byte* buffer, uint& length, uint& index) = 0;
		virtual uint _GetRawbufferLength(byte* buffer) = 0;
	protected:
		bool _PrepareBuffer(uint len);
	public:
		bool UnpackMessage(IMessage*& msg);//如果没有message，则返回空
		byte* NewBuffer(uint len);
		bool Windup();//如果错包次数太多就返回false
		void Reset();
	protected:
		HADES_FORCEINLINE void _ErrorOccurred();//当buffer发生错误时候调用，主要防止盗链
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void IRawbuffer::_ErrorOccurred()
	{
		++m_ErrorCount;
		m_Index = 0;
		m_Length = 0;
		m_Position = 0;
	}
}