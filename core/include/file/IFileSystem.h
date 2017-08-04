#pragma once
#include "core/Singleton.hpp"
#include "core/IPlugin.h"

#include <EASTL/string.h>


namespace hc
{
	class IFile;
	class HADES_CORE_API IFileSystem : public IPlugin
	{
		SYSTEM_SINGLETON_DECLEAR( IFileSystem );
		RTTI_ABSTRACT_DRIVER_TYPE_DECLEAR(IFileSystem, IPlugin)
	public:
		enum Constant
		{
			IO_IDENTIFIERS_LENGTH = 4,
			IO_SUBPATH_POS = 5,
			IO_MAX_LITE_PATH_SIZE = 1024,
		};
	private:
		eastl::string*		m_pRootPath;
		eastl::string*		m_pDocsPath;
		eastl::string*		m_pCommPath;
		eastl::string*		m_pFuncPath;
		eastl::string*		m_pDepdPath;
		eastl::string*		m_pScrsPath;
	protected:
		IFileSystem(void);
		virtual ~IFileSystem(void);
	private:
		virtual eastl::string _GetRootPath() = 0;
		virtual void _DoMakeFolder( const eastl::string& path ) = 0;
		virtual bool _DoIsFileExist(const eastl::string& filename) = 0;
		virtual bool _DoDeleteFile(const eastl::string& strFileName) = 0;
		virtual IFile* _DoFileFactory( const eastl::string& path ) = 0;
	public:
		virtual void RecycleBin( IFile* file ) = 0;
	public:
		virtual void Initialize( );
		virtual void Update();
		virtual void Exit( );
	public:
		void MakeFolder(const eastl::string& path);
		bool isFileExist(const eastl::string& path);
		bool DeleteFile(const eastl::string& path);
		eastl::string PathAssembly( const eastl::string& path );//Â·¾¶×é×°
		IFile* FileFactory( const eastl::string& path );
		void SetDocumentPath(const eastl::string& path);
		void SetScriptPath(const eastl::string& path);
	};
}
