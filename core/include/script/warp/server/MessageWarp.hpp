#pragma once
#include "network/server/IServerSystem.h"
#include "serialize/ISerializeSystem.h"
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "services/ISession.h"
#include "network/message/IMessage.h"
#include "services/ScriptServices.h"
#include "core/Types.h"



namespace hc
{
	//-------------------------------------------------------------------------------------------------------
	static void IMessageCleaner(void* rec, Object* obj)
	{
		IMessage* msg = static_cast<IMessage*>(obj);
		ISerializeSystem::Instance()->FreeSerializable(msg);
	}
	//-------------------------------------------------------------------------------------------------------
	static int IMessage_Construct(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res)
	{
		IMessage* msg = static_cast<IMessage*>(hc::ISerializeSystem::Instance()
			->GetSerializable(IMessage::RTTI().GetFactor()));
		res->SetValue(msg, IServerSystem::Instance(), &IMessageCleaner);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IMessage_GetID(hc::IVirtualMachine* vm, hc::FunctionRegister* func, uint* res, IMessage* self)
	{
		if (!self->isValid())
		{
			ScriptServices::MachineUserData* md =
				static_cast<ScriptServices::MachineUserData*>(vm->GetUserData());
			md->second->GotError("Got null IMessage! After IMessage send, the IMessage will be reset !");
			*res = nullhandle;
		}
		else
		{
			*res = self->GetID();
		}
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IMessage_GetMessageBuffer(hc::IVirtualMachine* vm, hc::FunctionRegister* func, StringBuffer* res, IMessage* self)
	{
		if (!self->isValid())
		{
			ScriptServices::MachineUserData* md =
				static_cast<ScriptServices::MachineUserData*>(vm->GetUserData());
			md->second->GotError("Got null IMessage! After IMessage send, the IMessage will be reset !");
			res->pBuffer = NULL;
			res->Length = 0;
		}
		else
		{
			res->pBuffer = self->GetMessageBuffer();
			res->Length = self->GetMessageLength();
		}
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IMessage_GetMessageLength(hc::IVirtualMachine* vm, hc::FunctionRegister* func, uint* res, IMessage* self)
	{
		if (!self->isValid())
		{
			ScriptServices::MachineUserData* md =
				static_cast<ScriptServices::MachineUserData*>(vm->GetUserData());
			md->second->GotError("Got null IMessage! After IMessage send, the IMessage will be reset !");
			*res = 0;
		}
		else
		{
			*res = self->GetMessageLength();
		}
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IMessage_PrepareMessage(hc::IVirtualMachine* vm, hc::FunctionRegister* func, StringBuffer* res, IMessage* self, uint* id, uint* len)
	{
		ScriptServices::MachineUserData* md =
			static_cast<ScriptServices::MachineUserData*>(vm->GetUserData());
		if (hc::nullhandle == *len)
		{
			md->second->GotError("PrepareMessage got null length!");
			res->pBuffer = NULL;
		}
		else
		{
			const byte* buf = self->PrepareMessage(*id, *len);
			if (NULL == buf)
			{
				md->second->GotError("Prepare length is out of maximun");
			}
			res->pBuffer = buf;
		}
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IMessage_Duplicate(hc::IVirtualMachine* vm, hc::FunctionRegister* func, hc::ScriptValue* res, IMessage* self)
	{
		IMessage* msg = static_cast<IMessage*>(hc::ISerializeSystem::Instance()
			->GetSerializable(IMessage::RTTI().GetFactor()));
		if (!self->isValid())
		{
			ScriptServices::MachineUserData* md =
				static_cast<ScriptServices::MachineUserData*>(vm->GetUserData());
			md->second->GotError("Got null IMessage! After IMessage send, the IMessage will be reset !");
		}
		else
		{
			*msg = *self;
		}
		res->SetValue(msg, IServerSystem::Instance(), &IMessageCleaner);		
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_IMessage(IScriptSystem& sys)
	{
		ClassRegister sc;
		sc.ClassType(&IMessage::RTTI());

		{
			hc::FunctionRegister sf;
			sf.Result( &IMessage::RTTI());
			sf.Function(&IMessage_Construct, hc::ScriptProperty::__CON);
			sc.ClassFunction(sf);
		}

		{
			hc::FunctionRegister sf;
			sf.Result(&IMessage::RTTI());
			sf.Function(&IMessage_Duplicate, "Duplicate");
			sc.ClassFunction(sf);
		}

		{
			FunctionRegister sf;
			sf.Result( &UInt::RTTI());
			sf.Function(&hc::IMessage_GetID, "GetID");
			sc.ClassFunction(sf);
		}
		{
			FunctionRegister sf;
			sf.Result(&Buffer::RTTI());
			sf.Arguments(&UInt::RTTI(), &UInt::RTTI());
			sf.Function(&hc::IMessage_PrepareMessage, "PrepareMessage");
			sc.ClassFunction(sf);
		}

		{
			FunctionRegister sf;
			sf.Result( &Buffer::RTTI());
			sf.Function(&hc::IMessage_GetMessageBuffer, "GetMessageBuffer");
			sc.ClassFunction(sf);
		}

		{
			FunctionRegister sf;
			sf.Result( &UInt::RTTI());
			sf.Function(&hc::IMessage_GetMessageLength, "GetMessageLength");
			sc.ClassFunction(sf);
		}

		sys.SetNativeLibrary(Configure::NATIVE_LIBRARY_SYSTEM, sc);
	}
}