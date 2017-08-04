#pragma once
#include "Configure.h"



namespace hc
{
	//@需要处理big,little ending的问题
	class Message;
	class Rawbuffer
	{
	private:
		uint		m_BufferSize;//当前池总大小
		uint		m_Length;//有效长度
		uint		m_Position;//当前末尾位置
		uint		m_Index;//当前拆分位置
		byte*		m_pBuffer;
	public:
		Rawbuffer();
		~Rawbuffer();
	private:
		bool _FindNextMessage();
		bool _DoUnpackMessage(Message* msg);
	public:
		bool UnpackMessage(Message* msg);//如果没有message，则返回空
		byte* NewBuffer(uint len);
		void Cleanup();
		void PrepareBuffer(uint len);
	};
}