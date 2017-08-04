#pragma once
#include "core/Recycleablevector.hpp"
#include "network/socket/ISocket.h"
#include "core/ObjectPool.hpp"
#include "core/Singleton.hpp"
#include "core/Delegate.hpp"
#include "core/SmartPtr.hpp"
#include "core/IPlugin.h"



namespace hc
{
	class SocketProxy;
	class HADES_CORE_API ISocketSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR(ISocketSystem);
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(ISocketSystem, IPlugin);
	private:
		enum Constant
		{
			SS_SOCKET_SIZE = 100,
		};
	public:
		enum SocketType
		{
			ST_COMMON_TL = 0,
			ST_OPENSSL_TLS,
		};
	private:
		typedef eastl::pair<SmartPtr<ISocket>, volatile int> ISocketPair;
		typedef Recycleablevector< ISocketPair* > ISocketArray;
		typedef ObjectPool< ISocketPair > ISocketPairlQueue;
	public:
		ISocketArray		m_ISocketArray;
		ISocketPairlQueue	m_ISocketPairlQueue;
	public:
		ISocketSystem();
		virtual ~ISocketSystem();
	private:
		virtual SmartPtr<ISocket> _DoSocketFactory(SocketType type) = 0;
		virtual void _DoRecycleBin(SmartPtr<ISocket>& ct) = 0;
	public:
		virtual void Initialize();
	public:
		SocketProxy* SocketFactory(SocketType type);//socket是异步的，所以在disconnect之后不能立即服用，脚本曾每次需要创建新的，底层会做poll
		SocketProxy* AllocSocketProxy(uint64 gid);
		void RecycleBin(SocketProxy* ct);
		SmartPtr<ISocket> GetSocketReference(uint64 gid);
	};
}