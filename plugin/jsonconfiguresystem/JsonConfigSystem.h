#pragma once
#include "configure/IConfigSystem.h"
#include "file/IFileSystem.h"

namespace hjc
{
	class JsonConfigSystem : public hc::IConfigSystem
	{
	public:
		JsonConfigSystem(void);
		virtual ~JsonConfigSystem(void);
	private:
		virtual hc::IConfigure* _DoConfigureFactory(const eastl::string& path);
		virtual void _DoRecycleBin(hc::IConfigure* cf);
	public:
		virtual void Initialize( );
		virtual void Update();
	};
}

