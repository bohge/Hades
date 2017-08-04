#pragma once
#include "network/server/IServerSystem.h"



struct ssl_ctx_st;
typedef struct ssl_ctx_st SSL_CTX;
namespace hles
{
	class LibeventServerSystem : public hc::IServerSystem
	{
	private:
		SSL_CTX*		m_Openssl;
	public:
		LibeventServerSystem(void);
		virtual ~LibeventServerSystem(void);
	private:
		virtual hc::IServer* _DoServerFactory(IServerSystem::ServerType st) const;
		virtual void _DoRecycleBin( hc::IServer* is ) const;
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	public:
		HADES_FORCEINLINE SSL_CTX* GetCTX();
	};


	//---------------------------------------------------------------------------------------------------------
	HADES_FORCEINLINE SSL_CTX* LibeventServerSystem::GetCTX()
	{
		return m_Openssl;
	}
}
