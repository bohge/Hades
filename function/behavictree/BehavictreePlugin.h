#pragma once
#include "core/Recycleablevector.hpp"
#include "core/Singleton.hpp"
#include "core/SmartPtr.hpp"
#include "core/IFunction.h"



namespace hc
{	
	class IThreadMutex;
	class IThreadSpecific;
	class IVirtualMachine;
	class IScriptInstance;
}
class CommonAgent;
namespace hbtf
{
	class Behavior;
	class TreeTemplate;
	class BehavictreePlugin : public hc::IFunction
	{
		SYSTEM_SINGLETON_DECLEAR(BehavictreePlugin);
		RTTI_DRIVER_TYPE_DECLEAR(BehavictreePlugin, hc::IFunction);
	private:
		typedef hc::Recycleablevector< hc::SmartPtr<CommonAgent> >	CommonAgentArray;
		typedef eastl::map< uint, TreeTemplate* > TemplaterMap;
	private:
		CommonAgentArray*		m_pCommonAgentArray;
		hc::IThreadMutex*		m_pIThreadMutex;
		hc::IThreadSpecific*	m_pThreadTemplate;
		bool					m_isInlitialized;
	public:
		enum Constant
		{
			BTPC_PREPARE_SIZE = 1000,
		};
		enum Fileformat
		{
			EFF_XML		= 1,
			EFF_BSON	= 2,
		};
	public:
		BehavictreePlugin();
		~BehavictreePlugin();
	public:
		virtual void Initialize();
		virtual void Update();
		virtual void Exit();
	public:
		void SetupBehavic(Fileformat format, const char* filepath, const char* metapath);
		void SetupTreeTemplate(hc::IScriptInstance* response, const char* treeName);
		Behavior* CreateBehavior(const char* treeName);
		void DestroyBehavior(Behavior* behav);
		hc::SmartPtr<CommonAgent> GetAgent(uint64 index);
		TreeTemplate* GetTreeTemplate(uint hash);
	};
}
