#pragma once
#include "file/IFileSystem.h"



namespace hfw
{
	class WindowsFileSystem : public hc::IFileSystem
	{
	public:
		WindowsFileSystem(void);
		virtual ~WindowsFileSystem(void);
	private:
		virtual eastl::string _GetRootPath();
		virtual void _DoMakeFolder( const eastl::string& path );
		virtual bool _DoIsFileExist(const eastl::string& filename);
		virtual bool _DoDeleteFile(const eastl::string& strFileName);
		virtual hc::IFile* _DoFileFactory(const eastl::string& path);
	public:		
		virtual void RecycleBin(hc::IFile* file);
	};
}