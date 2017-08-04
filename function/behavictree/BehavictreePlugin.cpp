#include "serialize/ISerializeSystem.h"
#include "thread/IThreadSpecific.h"
#include "script/IScriptSystem.h"
#include "thread/IThreadSystem.h"
#include "thread/IThreadMutex.h"
#include "BehavictreePlugin.h"
#include "behaviac/behaviac.h"
#include "file/IFileSystem.h"
#include "AllRegister.hpp"
#include "TreeTemplate.h"
#include "CommonAgent.h"
#include "Behavior.h"






using namespace hc;
using namespace behaviac;
namespace hbtf
{

	//void CleanupPlayer()
	//{
	//	behaviac::Agent::Destroy(g_player1);
	//}

	RTTI_TYPE_IMPLEMENT(BehavictreePlugin);
	SYSTEM_SINGLETON_IMPLEMENT(BehavictreePlugin);
	//---------------------------------------------------------------------------------------------------------
	BehavictreePlugin::BehavictreePlugin()
		:m_pCommonAgentArray(NULL)
		, m_pIThreadMutex(NULL)
		, m_pThreadTemplate(NULL)
		, m_isInlitialized(false)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//---------------------------------------------------------------------------------------------------------
	BehavictreePlugin::~BehavictreePlugin()
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//---------------------------------------------------------------------------------------------------------
	void BehavictreePlugin::Initialize()
	{
		m_pCommonAgentArray = NEW CommonAgentArray;
		m_pCommonAgentArray->Initialize(BTPC_PREPARE_SIZE);
		m_pIThreadMutex = static_cast<hc::IThreadMutex*>(
			IThreadSystem::Instance()
			->ThreadEntityFactory(ThreadProperty::ET_MUTEX));
		m_pThreadTemplate = static_cast<hc::IThreadSpecific*>(
			IThreadSystem::Instance()
			->ThreadEntityFactory(ThreadProperty::ET_THREAD_SPECIFIC));
		{
			hc::IScriptSystem* sys = hc::IScriptSystem::Instance();
			Register_All("Behavictree", *sys);
		}
		{
			hc::ISerializeSystem* sys = hc::ISerializeSystem::Instance();
			sys->RegisterSerializable<Behavior>();
		}
	}
	//---------------------------------------------------------------------------------------------------------
	void BehavictreePlugin::Update()
	{
	}
	//---------------------------------------------------------------------------------------------------------
	void BehavictreePlugin::Exit()
	{
		SAFE_DELETE(m_pCommonAgentArray);
		IThreadSystem::Instance()->RecycleBin(m_pIThreadMutex);
		IThreadSystem::Instance()->RecycleBin(m_pThreadTemplate);
		behaviac::Agent::UnRegister<CommonAgent>();
		behaviac::Workspace::GetInstance()->Cleanup();
	}
	//---------------------------------------------------------------------------------------------------------
	void BehavictreePlugin::SetupBehavic(Fileformat format, const char* filepath, const char* metapath)
	{
		m_pIThreadMutex->Lock();
		if (!m_isInlitialized)
		{
			m_isInlitialized = true;
			m_pIThreadMutex->Unlock();
			eastl::string fullpath = IFileSystem::Instance()->PathAssembly(filepath);
			eastl::string fullmeta = IFileSystem::Instance()->PathAssembly(metapath);
			Agent::Register<CommonAgent>();
			Workspace::GetInstance()->SetFilePath(fullpath.c_str());
			Workspace::GetInstance()->SetFileFormat((Workspace::EFileFormat)format);
			Workspace::GetInstance()->ExportMetas(fullmeta.c_str());
		}
		m_pIThreadMutex->Unlock();
	}
	//---------------------------------------------------------------------------------------------------------
	void BehavictreePlugin::SetupTreeTemplate(hc::IScriptInstance* response, const char* treeName)
	{
		TemplaterMap* trees = m_pThreadTemplate->GetSpecific<TemplaterMap>();
		if (NULL == trees)
		{
			trees = NEW TemplaterMap;
			m_pThreadTemplate->SetSpecific(trees);
		}
		uint hash = Utility::HashCode(treeName);
		TemplaterMap::iterator it = trees->find(hash);
		if (trees->end() == it)
		{
			TreeTemplate* temp = NEW TreeTemplate;
			temp->Initialize(response);
			trees->insert(eastl::make_pair(hash, temp));
		}
	}
	//---------------------------------------------------------------------------------------------------------
	Behavior* BehavictreePlugin::CreateBehavior(const char* treeName)
	{
		m_pIThreadMutex->Lock();
		hc::SmartPtr<CommonAgent> player = hc::SmartPtr<CommonAgent>(behaviac::Agent::Create<CommonAgent>());
		m_pIThreadMutex->Unlock();

		uint64 gid = m_pCommonAgentArray->Push(player);
		bool bRet = player->btload(treeName);
		player->btsetcurrent(treeName);

		uint hash = Utility::HashCode(treeName);
		Behavior* beh = static_cast<Behavior*>(ISerializeSystem::Instance()
			->GetSerializable(Behavior::RTTI().GetFactor()));
		beh->Initialize(gid, hash);
		return beh;
	}
	//---------------------------------------------------------------------------------------------------------
	void BehavictreePlugin::DestroyBehavior(Behavior* behav)
	{
		uint64 index = behav->GetIndex();
		m_pCommonAgentArray->Remove(index);
		ISerializeSystem::Instance()->FreeSerializable(behav);
	}
	//---------------------------------------------------------------------------------------------------------
	hc::SmartPtr<CommonAgent> BehavictreePlugin::GetAgent(uint64 index)
	{
		hc::SmartPtr<CommonAgent> res;
		if (m_pCommonAgentArray->Have(index))
		{
			res = m_pCommonAgentArray->Get(index);
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	TreeTemplate* BehavictreePlugin::GetTreeTemplate(uint hash)
	{
		TemplaterMap* trees = m_pThreadTemplate->GetSpecific<TemplaterMap>();
		ASSERT(NULL != trees);
		TemplaterMap::iterator it = trees->find(hash);
		ASSERT(trees->end() != it );
		return it->second;
	}

}