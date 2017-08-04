#include "TestFunctionRegister.h"
#include "BufferUtility.hpp"

#include "network/message/IMessageSystem.h"
#include "network/server/IServerSystem.h"
#include "serialize/ISerializeSystem.h"
#include "network/message/IRawbuffer.h"
#include "network/message/IMessage.h"


using namespace hc;
namespace hct
{
	REGISTER(RawbufferMessageTester);

	//---------------------------------------------------------------------------------------------------------
	void RawbufferMessageTester()
	{
		IRawbuffer* rb = IMessageSystem::Instance()->RawbufferFactory();
		IMessage* msg = static_cast<IMessage*>(hc::ISerializeSystem::Instance()
			->GetSerializable(IMessage::RTTI().GetFactor()));
		IMessage* pmsg = NULL;
		byte buffer[] = {'a', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'f', 'd'};
		byte error[] = {'1', '3', '4', '4', '2', '5'};
		//memcpy(msg->Data(), buffer, sizeof(buffer));
		msg->CopyMessage(0, buffer, sizeof(buffer));

		//{//message等号测试
		//	IMessage* test = static_cast<IMessage*>(hc::ISerializeSystem::Instance()
		//		->GetSerializable(IMessage::RTTI().GetFactor()));
		//	*test = *msg;
		//	BufferUtility::CompareBuffer(msg->GetRawBuffer(), msg->GetRawLength(), test->GetRawBuffer(), test->GetRawLength(), "message operator = test");
		//	hc::ISerializeSystem::Instance()->FreeSerializable(test);
		//}

		{
			byte* buff = rb->NewBuffer(msg->GetRawLength());
			memcpy(buff, msg->GetRawBuffer(), msg->GetRawLength());
			while (rb->UnpackMessage(pmsg))
			{
				BufferUtility::CompareBuffer(msg->GetRawBuffer(), msg->GetRawLength(), pmsg->GetRawBuffer(), pmsg->GetRawLength(), "unpack message test");
			}
			rb->Windup();
		}

		{//测试粘包
			byte* buff = rb->NewBuffer(msg->GetRawLength());
			memcpy(buff, msg->GetRawBuffer(), msg->GetRawLength());
			buff = rb->NewBuffer(msg->GetRawLength());
			memcpy(buff, msg->GetRawBuffer(), msg->GetRawLength());
			while (rb->UnpackMessage(pmsg))
			{
				BufferUtility::CompareBuffer(msg->GetRawBuffer(), msg->GetRawLength(), pmsg->GetRawBuffer(), pmsg->GetRawLength(), "stick message test");
			}
			rb->Windup();
		}

		{//测试半包
			byte* buff = rb->NewBuffer(msg->GetRawLength());
			memcpy(buff, msg->GetRawBuffer(), msg->GetRawLength());
			//加入一个半包
			buff = rb->NewBuffer(msg->GetRawLength() - sizeof(buffer));
			memcpy(buff, msg->GetRawBuffer(), msg->GetRawLength() - sizeof(buffer));
			while (rb->UnpackMessage(pmsg))
			{
				BufferUtility::CompareBuffer(msg->GetRawBuffer(), msg->GetRawLength(), pmsg->GetRawBuffer(), pmsg->GetRawLength(), "half message test");
			}
			rb->Windup();
			//补全半包
			buff = rb->NewBuffer(sizeof(buffer));
			memcpy(buff, &msg->GetRawBuffer()[msg->GetRawLength() - sizeof(buffer)], sizeof(buffer));
			while (rb->UnpackMessage(pmsg))
			{
				BufferUtility::CompareBuffer(msg->GetRawBuffer(), msg->GetRawLength(), pmsg->GetRawBuffer(), pmsg->GetRawLength(), "half message + remain test");
			}
			rb->Windup();
		}

		{//测试错包
			byte* buff = rb->NewBuffer(sizeof(error));
			memcpy(buff, error, sizeof(error));
			while (rb->UnpackMessage(pmsg))
			{
				BufferUtility::CompareBuffer(msg->GetRawBuffer(), msg->GetRawLength(), pmsg->GetRawBuffer(), pmsg->GetRawLength(), "error package test");
			}
			rb->Windup();
			//添加一个头
			buff = rb->NewBuffer(sizeof(error));
			memcpy(buff, error, sizeof(error));
			buff = rb->NewBuffer(msg->GetHeadSize());
			memcpy(buff, msg->GetRawBuffer(), msg->GetHeadSize());
			while (rb->UnpackMessage(pmsg))
			{
				BufferUtility::CompareBuffer(msg->GetRawBuffer(), msg->GetRawLength(), pmsg->GetRawBuffer(), pmsg->GetRawLength(), "error package + head test");
			}
			rb->Windup();
			//将头之后的添加进去
			buff = rb->NewBuffer(msg->GetRawLength() - msg->GetHeadSize());
			memcpy(buff, &msg->GetRawBuffer()[msg->GetHeadSize()], msg->GetRawLength() - msg->GetHeadSize());
			while (rb->UnpackMessage(pmsg))
			{
				BufferUtility::CompareBuffer(msg->GetRawBuffer(), msg->GetRawLength(), pmsg->GetRawBuffer(), pmsg->GetRawLength(), "error package + head test + remain");
			}
			rb->Windup();
		}

		{
			byte* buff = rb->NewBuffer(msg->GetRawLength());
			memcpy(buff, msg->GetRawBuffer(), msg->GetRawLength());
			while (rb->UnpackMessage(pmsg))
			{
				BufferUtility::CompareBuffer(msg->GetRawBuffer(), msg->GetRawLength(), pmsg->GetRawBuffer(), pmsg->GetRawLength(), "end message test");
			}
			rb->Windup();
		}
	}
}