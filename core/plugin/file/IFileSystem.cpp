#include "file/IFileSystem.h"
#include "core/Configure.h"
#include "file/IFile.h"
#include "log/ILogSystem.h"


#include <string.h>
#include <stdio.h>
#include <stdlib.h>

using namespace eastl;

namespace hc
{
	RTTI_TYPE_IMPLEMENT( IFile );
	RTTI_TYPE_IMPLEMENT( IFileSystem );
	SYSTEM_SINGLETON_IMPLEMENT( IFileSystem );
	//-------------------------------------------------------------------------------------------------------
	IFileSystem::IFileSystem( void )
		:m_pRootPath(NULL)
	{
		SYSTEM_SINGLETON_INITIALIZE;
	}
	//-------------------------------------------------------------------------------------------------------
	IFileSystem::~IFileSystem( void )
	{
		SYSTEM_SINGLETON_DESTROY;
	}
	//-------------------------------------------------------------------------------------------------------
	void IFileSystem::MakeFolder( const eastl::string& path )
	{
		_DoMakeFolder( PathAssembly(path) );
	}
	//-------------------------------------------------------------------------------------------------------
	bool IFileSystem::isFileExist( const eastl::string& path )
	{
		return _DoIsFileExist( PathAssembly(path) );
	}
	//-------------------------------------------------------------------------------------------------------
	bool IFileSystem::DeleteFile( const eastl::string& path )
	{
		return _DoDeleteFile( PathAssembly(path) );
	}
	//-------------------------------------------------------------------------------------------------------
	IFile* IFileSystem::FileFactory( const eastl::string& path )
	{
		return _DoFileFactory( PathAssembly(path) );
	}
	//-------------------------------------------------------------------------------------------------------
	void IFileSystem::SetDocumentPath(const eastl::string& path)
	{
		*m_pDocsPath = PathAssembly(path);
	}
	//-------------------------------------------------------------------------------------------------------
	void IFileSystem::SetScriptPath(const eastl::string& path)
	{
		*m_pScrsPath = PathAssembly(path);
	}
	//-------------------------------------------------------------------------------------------------------
	void IFileSystem::Initialize( )
	{
		m_pRootPath = NEW string(_GetRootPath());
		m_pCommPath = NEW string(_GetRootPath() + Configure::COMMON_PATH);
		m_pFuncPath = NEW string(_GetRootPath() + Configure::FUNCTION_PATH);
		m_pDepdPath = NEW string(_GetRootPath() + Configure::DEPENDENCY_PATH);
		m_pDocsPath = NEW string;
		m_pScrsPath = NEW string;
	}
	//-------------------------------------------------------------------------------------------------------
	void IFileSystem::Update()
	{

	}
	//-------------------------------------------------------------------------------------------------------
	void IFileSystem::Exit( )
	{
		SAFE_DELETE(m_pRootPath);
		SAFE_DELETE(m_pCommPath);
		SAFE_DELETE(m_pScrsPath);
		SAFE_DELETE(m_pDocsPath);
		SAFE_DELETE(m_pDepdPath);
		SAFE_DELETE(m_pFuncPath);
	}
	//-------------------------------------------------------------------------------------------------------
	eastl::string IFileSystem::PathAssembly( const eastl::string& path )
	{
		//uint id = 0;
		//for ( int i = 0 ; i < IO_IDENTIFIERS_LENGTH ; ++ i )
		//{
		//	id |= static_cast<int>(path[i]) << ( (IO_IDENTIFIERS_LENGTH - i - 1) * 8 );
		//}
		uint id = static_cast<int>(path[0]) << 24
			| static_cast<int>(path[1]) << 16
			| static_cast<int>(path[2]) << 8
			| static_cast<int>(path[3]);
		switch( id )
		{
		case FILE_PATH_FOURCC_ROOT: return *m_pRootPath + path.substr(IO_SUBPATH_POS, path.length());
		case FILE_PATH_FOURCC_DOCUMENT: return *m_pDocsPath + path.substr(IO_SUBPATH_POS, path.length());
		case FILE_PATH_FOURCC_SCRIPT: return *m_pScrsPath + path.substr(IO_SUBPATH_POS, path.length());
		case FILE_PATH_FOURCC_COMMON: return *m_pCommPath + path.substr(IO_SUBPATH_POS, path.length());
		case FILE_PATH_FOURCC_FUNCTION: return *m_pFuncPath + path.substr(IO_SUBPATH_POS, path.length());
		case FILE_PATH_FOURCC_DEPENDENCY: return *m_pDepdPath + path.substr(IO_SUBPATH_POS, path.length());
		default:return path;
		}
	}
}