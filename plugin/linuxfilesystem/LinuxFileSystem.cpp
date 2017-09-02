#include "LinuxFileSystem.h"
#include "UsualFile.h"
#include "log/ILogSystem.h"

#define MAX_PATH	260

#include <EASTL/string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>

void get_path(char* path)
{
	char arg1[20];
	sprintf(arg1, "/proc/%d/exe", getpid());
	readlink(arg1, path, MAX_PATH);
}

using namespace hc;
using namespace eastl;
namespace hfl
{
	//-------------------------------------------------------------------------------------------------------
	LinuxFileSystem::LinuxFileSystem(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	LinuxFileSystem::~LinuxFileSystem(void)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	eastl::string LinuxFileSystem::_GetRootPath()
	{
		char strFilePath[MAX_PATH + 1] = { 0 };
		get_path(strFilePath);
		(strrchr(strFilePath, ('/')))[1] = 0;
		int res = chdir(strFilePath);
		return strFilePath;
	}
	//-------------------------------------------------------------------------------------------------------
	void LinuxFileSystem::_DoMakeFolder( const eastl::string& path )
	{
		if( access(path.c_str(),0) == -1 ) //如果文件夹不存在
		{
			HADESLOG("make folder %s\n", path.c_str());
			mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
		}
	}
	//-------------------------------------------------------------------------------------------------------
	bool LinuxFileSystem::_DoIsFileExist( const eastl::string& filename )
	{
		if( 0 == access(filename.c_str(), 0) )//如果直接存在
		{
			return true;
		}
		return false;
	}
	//-------------------------------------------------------------------------------------------------------
	bool LinuxFileSystem::_DoDeleteFile( const eastl::string& strFileName )
	{
		if( isFileExist(strFileName) )
		{
			remove( strFileName.c_str() );
		}
		return true;
	}
	////-------------------------------------------------------------------------------------------------------
	//eastl::vector<eastl::string> LinuxFileSystem::GetFileNamesWithExpand( const eastl::string& path, const eastl::string& exp )
	//{
	//	vector<string> files;
	//	string fullpath = path;
	//	struct _finddata_t c_file; 
	//	intptr_t   hFile; 
	//	if(_chdir(fullpath.c_str())) 
	//	{
	//		HADESLOG("Cant' open direction, %s\n", fullpath.c_str() );
	//		ASSERT(false);
	//	} 
	//	else 
	//	{
	//		hFile = _findfirst("*.*", &c_file); 
	//		if( 0 < c_file.size )
	//		{
	//			HADESLOG("include file, %s\n", c_file.name );
	//			string filename = c_file.name;
	//			if ( exp.empty() || string::npos != filename.find(exp) )
	//			{
	//				files.push_back( filename.substr(filename.find_last_of("/\\")+1) );
	//			}
	//		}
	//	}
	//	while(_findnext(hFile, &c_file) == 0) 
	//	{ 
	//		if( 0 < c_file.size )
	//		{
	//			HADESLOG("include file, %s\n", c_file.name );
	//			string filename = c_file.name;
	//			if ( exp.empty() || string::npos != filename.find(exp) )
	//			{
	//				files.push_back( filename.substr(filename.find_last_of("/\\")+1) );
	//			}
	//		}
	//	} 
	//	_findclose(hFile); 
	//	return files;
	//}
	//-------------------------------------------------------------------------------------------------------
	IFile* LinuxFileSystem::_DoFileFactory( const eastl::string& path )
	{
		return NEW UsualFile( path );
	}
	//-------------------------------------------------------------------------------------------------------
	void LinuxFileSystem::RecycleBin( IFile* file )
	{
		SAFE_DELETE( file );
	}

}