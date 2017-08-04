#include "thread/IJobDispatcher.h"
#include "thread/IThreadSystem.h"
#include "LibeventConnection.h"
#include "LibeventServer.h"
#include "log/ILogSystem.h"
#include "ConnectionJob.h"
#include "ListenerJob.h"



using namespace hc;
namespace hles
{
	//---------------------------------------------------------------------------------------------------------
	LibeventServer::LibeventServer(hc::IServerSystem::ServerType st)
		:m_spListenerJob(SmartPtr<ListenerJob>(NEW ListenerJob(this, st)))
		, m_pJobThread(NULL)
	{

	}
	//---------------------------------------------------------------------------------------------------------
	LibeventServer::~LibeventServer(void)
	{
		m_spListenerJob = SmartPtr<ListenerJob>(NULL);
		ASSERT(NULL == m_spListenerJob);
	}
	//---------------------------------------------------------------------------------------------------------
	bool LibeventServer::_OnStart(int connectthread, int port)
	{
		m_spListenerJob->Initialize();
		_SetServerAddress(m_spListenerJob->GetIPAddress());
		m_pJobThread =
			static_cast<IJobDispatcher*>(
			IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_JOB_DISPATCHER));
		m_pJobThread->SetSortType(ThreadProperty::ST_UNSORT);
		m_pJobThread->SetThreadCount(connectthread + 1);
		m_pJobThread->SetName("Traffic");
		m_pJobThread->Start();

		//初始化通讯线程
		for (size_t i = 0; i < connectthread; i++)
		{
			hc::SmartPtr<ConnectionJob> conn = m_spListenerJob->MakeConnectionJob();
			if (conn.isValid())
			{
				m_pJobThread->PushJob(conn);
			}
		}
		bool res = m_spListenerJob->StartListen(port);
		if (res)
		{
			m_pJobThread->PushJob(m_spListenerJob);
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServer::_OnStop()
	{
		m_pJobThread->Stop();
		IThreadSystem::Instance()->RecycleBin(m_pJobThread);
		m_pJobThread = NULL;
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServer::SetServerCallback(hc::ServerCallback::Callback func)
	{
		m_spListenerJob->SetServerCallback(func);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServer::Broadcast(uint64 id, hc::IMessage* msg)
	{
		m_spListenerJob->Broadcast(id, msg);
	}
	//---------------------------------------------------------------------------------------------------------
	bool LibeventServer::SendBuffer(uint64 id, IMessage* req)
	{
		return m_spListenerJob->SendBuffer(id, req);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServer::Disconnect(uint64 id)
	{
		m_spListenerJob->Disconnect(id);
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServer::SetSpecific(uint64 id, uint64 spec)
	{
		m_spListenerJob->SetSpecific(id, spec);
	}
	//---------------------------------------------------------------------------------------------------------
	uint64 LibeventServer::GetSpecific(uint64 id)
	{
		return m_spListenerJob->GetSpecific(id);
	}
	//---------------------------------------------------------------------------------------------------------
	hc::IServerSystem::ServerType LibeventServer::GetServerType()
	{
		return m_spListenerJob->GetServerType();
	}
	//---------------------------------------------------------------------------------------------------------
	void LibeventServer::SetHeartBeat(uint idle, uint inter, uint count)
	{
		m_spListenerJob->SetHeartBeat(idle, inter, count);
	}
}