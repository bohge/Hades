#include "file/IFile.h"
#include "JsonConfigSystem.h"
#include "JsonConfigure.h"


using namespace hc;
using namespace eastl;

namespace hjc
{
	//---------------------------------------------------------------------------------------------------------
	JsonConfigSystem::JsonConfigSystem( void )
	{

	}
	//---------------------------------------------------------------------------------------------------------
	JsonConfigSystem::~JsonConfigSystem( void )
	{

	}
	//---------------------------------------------------------------------------------------------------------
	IConfigure* JsonConfigSystem::_DoConfigureFactory( const eastl::string& path )
	{
		JsonConfigure* res = NULL;
		IFile* file = IFileSystem::Instance()->FileFactory(path);
		if (file->OpenFile(IFile::AT_READ))
		{
			uint size = file->GetSize();
			char* content = NEW char[size + 1];
			content[size] = 0;
			file->ReadFile(content, size);
			file->CloseFile();
			IFileSystem::Instance()->RecycleBin(file);
			res = NEW JsonConfigure;
			if (!res->Parse(content))
			{
				SAFE_DELETE(res)
			}
			SAFE_DELETE_ARRAY(content);
		}
		return res;
	}
	//---------------------------------------------------------------------------------------------------------
	void JsonConfigSystem::_DoRecycleBin( IConfigure* cf )
	{
		SAFE_DELETE(cf);
	}
	//---------------------------------------------------------------------------------------------------------
	void JsonConfigSystem::Initialize( )
	{
	}
	//-------------------------------------------------------------------------------------------------------
	void JsonConfigSystem::Update()
	{

	}
}
