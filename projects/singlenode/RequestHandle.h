#pragma once
#include "script/ICoroutineInvoker.h"
#include "keeper/IAccessPackage.h"
#include "RequestUserData.h"
#include "keeper/Accessor.h"
#include "thread/IJob.h"
#include "core/Types.h"



#include <EASTL/vector.h>

namespace hc
{
	enum CallbackType;
	class Message;
	class IDecoder;
	class IEncoder;
	class IAccessPackage;
	class IScriptInstance;
}
namespace hsna
{
	class RequestUserData;
	class RequestHandle : public hc::IJob
	{
		RTTI_DRIVER_TYPE_DECLEAR(RequestHandle, hc::IJob);
	private:
		enum Constant
		{
			RHC_DEFUALT_ACESS_SIZE = 10,
		};
	private:
		struct AccessPackage
		{
			bool						Auto;
			uint						ReferenceCount;
			hc::Accessor*				Acessor;
			hc::IScriptInstance*		Instance;
			AccessPackage(void);
			AccessPackage(hc::IScriptInstance* ins);
		};
	private:
		//typedef hc::Nonclearvector<AccessPackage>	AccessPackageArrary;
		typedef eastl::map<uint64, AccessPackage>	AccessPackageArrary;
		typedef eastl::pair<uint, hc::Object*> HoldPair;
	private:
		AccessPackageArrary			m_AccessPackageArrary;
		uint64						m_ID;
		hc::IAccessPackage*			m_pIAccessPackage;
		hc::ICoroutineInvoker*		m_pCurrentInvoke;
		hc::Message*				m_rpMessage;
		hc::CallbackType			m_CallbackType;
		RequestUserData*			m_rpCurrentRequestUserData;
		hc::IScriptInstance*		m_rpImmediatelyInstance;
		//double						m_BegineTime;
	public:
		RequestHandle();
		virtual ~RequestHandle();
	private:
		virtual void _DoYieldJob();
		virtual void _DoJob(hc::SmartPtr<hc::IJob>& self, hc::Object* userdata);
	private:
		bool _DoSetInstanceValue(uint64& errorid);
		void _DoSaveInstanceValue(hc::Accessor* acc, hc::IScriptInstance* instance);
		hc::IScriptInstance* _TryReferenceAccessor(uint64 id, hc::Accessor::AccessMode am);
		void _DoCreateAccessor(hc::IScriptInstance* instance);
		void _DoUnrigorousAccessor(uint64 id, hc::IScriptInstance* instance);
		void _DoReadonlyAccessor(uint64 id, hc::IScriptInstance* instance);
		void _DoReadwriteAccessor(uint64 id, hc::IScriptInstance* instance);
	public:
		bool BeginRequest(int& magic);
		void ReleaseAccessores();
		void ReleaseAccessor(uint64 gid);
	public:
		void OnAccessorCallback(const hc::AccessinfoArray& accs);
		bool hasRequest();
	private:
		HADES_FORCEINLINE void _CheckGID(uint64 gid);
		HADES_FORCEINLINE void _CheckInstance(hc::IScriptInstance* instance);
		HADES_FORCEINLINE void _CheckCachedInstance(uint64 gid, hc::IScriptInstance* instacne);
	public:
		HADES_FORCEINLINE hc::IScriptInstance* CreateAccessorImmediately(bool& ispasue, int& magic);
		HADES_FORCEINLINE hc::IScriptInstance* UnrigorousAccessorImmediately(uint64 id, bool& ispasue, int& magic);
		HADES_FORCEINLINE hc::IScriptInstance* ReadonlyAccessorImmediately(uint64 id, bool& ispasue, int& magic);
		HADES_FORCEINLINE hc::IScriptInstance* ReadwriteAccessorImmediately(uint64 id, bool& ispasue, int& magic);
		HADES_FORCEINLINE hc::IScriptInstance* CreateAccessor();
		HADES_FORCEINLINE hc::IScriptInstance* UnrigorousAccessor(uint64 id);
		HADES_FORCEINLINE hc::IScriptInstance* ReadonlyAccessor(uint64 id);
		HADES_FORCEINLINE hc::IScriptInstance* ReadwriteAccessor(uint64 id);
		HADES_FORCEINLINE void CreateAccessor(hc::IScriptInstance* instance);
		HADES_FORCEINLINE void UnrigorousAccessor(uint64 id, hc::IScriptInstance* instance);
		HADES_FORCEINLINE void ReadonlyAccessor(uint64 id, hc::IScriptInstance* instance);
		HADES_FORCEINLINE void ReadwriteAccessor(uint64 id, hc::IScriptInstance* instance);
		HADES_FORCEINLINE void SetMessage(hc::Message* req);
		HADES_FORCEINLINE void SetCallbackType(hc::CallbackType type);
		HADES_FORCEINLINE void SetClient(uint64 id);
	};



	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RequestHandle::_CheckGID(uint64 gid)
	{
		if (hc::nullid == gid)
		{
			m_pCurrentInvoke->GotError("\r\nNullid request!");
		}
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RequestHandle::_CheckInstance(hc::IScriptInstance* instance)
	{
		if (NULL == instance)
		{
			m_pCurrentInvoke->GotError("\r\nPass a nil script instance handle!");
		}
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RequestHandle::_CheckCachedInstance(uint64 gid, hc::IScriptInstance* instacne)
	{
		AccessPackageArrary::iterator it = m_AccessPackageArrary.find(gid);
		if (m_AccessPackageArrary.end() != it)
		{
			if (instacne != it->second.Instance)
			{
				m_pCurrentInvoke->GotError("\r\nYou're requested a same GID, but want to set in two deffrent instance.");
			}
		}
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::IScriptInstance* RequestHandle::CreateAccessorImmediately(bool& ispasue, int& magic)
	{
		m_rpImmediatelyInstance = CreateAccessor();
		ispasue = BeginRequest(magic);
		return m_rpImmediatelyInstance;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::IScriptInstance* RequestHandle::UnrigorousAccessorImmediately(uint64 id, bool& ispasue, int& magic)
	{
		m_rpImmediatelyInstance = UnrigorousAccessor(id);
		ispasue = BeginRequest(magic);
		return m_rpImmediatelyInstance;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::IScriptInstance* RequestHandle::ReadonlyAccessorImmediately(uint64 id, bool& ispasue, int& magic)
	{
		m_rpImmediatelyInstance = ReadonlyAccessor(id);
		ispasue = BeginRequest(magic);
		return m_rpImmediatelyInstance;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::IScriptInstance* RequestHandle::ReadwriteAccessorImmediately(uint64 id, bool& ispasue, int& magic)
	{
		m_rpImmediatelyInstance = ReadwriteAccessor(id);
		ispasue = BeginRequest(magic);
		return m_rpImmediatelyInstance;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::IScriptInstance* RequestHandle::CreateAccessor()
	{
		hc::IScriptInstance* instance = m_rpCurrentRequestUserData->GetInstance(hc::nullid);
		_DoCreateAccessor(instance);
		return instance;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::IScriptInstance* RequestHandle::UnrigorousAccessor(uint64 id)
	{
		hc::IScriptInstance* instance = _TryReferenceAccessor(id, hc::Accessor::AM_UNRIGOROUS);
		if (NULL == instance)
		{
			instance = m_rpCurrentRequestUserData->GetInstance(id);
			_DoUnrigorousAccessor(id, instance);
		}
		return instance;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::IScriptInstance* RequestHandle::ReadonlyAccessor(uint64 id)
	{
		hc::IScriptInstance* instance = _TryReferenceAccessor(id, hc::Accessor::AM_READ_ONLY);
		if (NULL == instance)
		{
			instance = m_rpCurrentRequestUserData->GetInstance(id);
			_DoReadonlyAccessor(id, instance);
		}
		return instance;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::IScriptInstance* RequestHandle::ReadwriteAccessor(uint64 id)
	{
		hc::IScriptInstance* instance = _TryReferenceAccessor(id, hc::Accessor::AM_READ_WRITE);
		if (NULL == instance)
		{
			instance = m_rpCurrentRequestUserData->GetInstance(id);
			_DoReadwriteAccessor(id, instance);
		}
		return instance;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RequestHandle::CreateAccessor(hc::IScriptInstance* instance)
	{
		_CheckInstance(instance);
		_DoCreateAccessor(instance);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RequestHandle::UnrigorousAccessor(uint64 id, hc::IScriptInstance* instance)
	{
		_CheckInstance(instance);
		_CheckCachedInstance(id, instance);
		_DoUnrigorousAccessor(id, instance);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RequestHandle::ReadonlyAccessor(uint64 id, hc::IScriptInstance* instance)
	{
		_CheckInstance(instance);
		_CheckCachedInstance(id, instance);
		_DoReadonlyAccessor(id, instance);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RequestHandle::ReadwriteAccessor(uint64 id, hc::IScriptInstance* instance)
	{
		_CheckInstance(instance);
		_CheckCachedInstance(id, instance);
		_DoReadwriteAccessor(id, instance);
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RequestHandle::SetMessage(hc::Message* req)
	{
		m_rpMessage = req;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RequestHandle::SetCallbackType(hc::CallbackType type)
	{
		m_CallbackType = type;
	}
	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE void RequestHandle::SetClient(uint64 id)
	{
		m_ID = id;
	}
}
