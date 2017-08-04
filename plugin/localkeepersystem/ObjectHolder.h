#pragma once
#include "LocalKeeperSystem.h"
#include "keeper/Accessor.h"

#include <EASTL/list.h>

namespace hc
{
	class Accessor;
	class IThreadMutex;
}
namespace hlks
{
	typedef hc::Unidelegate1<void, hc::Accessor::AccessMode> SaveCallback;
	class ObjectHolder : public hc::Object
	{
		RTTI_TYPE_DECLEAR(ObjectHolder);
	private:
		enum HolderState
		{
			HS_NON_ACESS = 0,
			HS_READ_ONLY,
			HS_READ_WRITE
		};
	private:
		struct HolderInfo
		{
			int							Timestamp;
			hc::Accessor::AccessMode	Mode;
			AccessCallback::Callback	Func;
			HolderInfo(int t, hc::Accessor::AccessMode m, AccessCallback::Callback f)
				:Timestamp(t), Mode(m), Func(f){}
		};
	private:
		//这里为了避免ConcurrentQueue多线程安全的不确定因素，暂时用list+lock
		typedef eastl::list< HolderInfo> AccessList;
	private:
		AccessList				m_AccessList;
		hc::IThreadMutex*		m_pIThreadMutex;
		hc::Accessor*			m_rpMainAccessor;
		SaveCallback::Callback	m_SaveCallback;
		HolderState				m_eHolderState;
		volatile int			m_ReadReferenceCount;
		int						m_Timestamp;
	public:
		ObjectHolder();
		virtual ~ObjectHolder();
	private:
		bool _TryWriteAcess();
		bool _TryReadAcess();
		bool _TryUnrigorousAcess();
		void _ActiveStoragedJob();
		void _WriteRelease();
		void _ReadRelease();
		void _RequestJob(int timestamp, hc::Accessor::AccessMode am, AccessCallback::Callback func);
		void _StorageJob(int timestamp, hc::Accessor::AccessMode am, AccessCallback::Callback func);
		bool _AccessAction(hc::Accessor::AccessMode am);
	public:
		void Initialize(uint64 id);
		void Exit();
	public:
		bool UnrigorousAccessor(int timestamp, AccessCallback::Callback func);
		bool ReadonlyAccessor(int timestamp, AccessCallback::Callback func);
		bool ReadwriteAccessor(int timestamp, AccessCallback::Callback func);
		void ReleaseAccessor(hc::Accessor* acc);
		uint Count();
	public:
		void OnSaved(hc::Accessor::AccessMode am);
	public:
		HADES_FORCEINLINE hc::Accessor* GetMainAccessor();
	};

	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE hc::Accessor* ObjectHolder::GetMainAccessor()
	{
		return m_rpMainAccessor;
	}
}
