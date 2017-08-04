#pragma once
#include "script/FunctionRegister.h"
#include "script/IVirtualMachine.h"
#include "script/ClassRegister.h"
#include "script/ScriptValue.h"
#include "file/IFileSystem.h"
#include "log/ILogSystem.h"
#include "core/Types.h"
#include "file/IFile.h"



namespace hc
{
	//-------------------------------------------------------------------------------------------------------
	static int IFileSystem_isFileExist(hc::IVirtualMachine* vm, hc::FunctionRegister* func, bool* res, IFileSystem* self, StringBuffer* path)
	{
		eastl::string spath(static_cast<const char*>(path->pBuffer), path->Length);
		*res = self->isFileExist(spath);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IFileSystem_PathAssembly(hc::IVirtualMachine* vm, hc::FunctionRegister* func, eastl::string* res, IFileSystem* self, StringBuffer* path)
	{
		eastl::string spath(static_cast<const char*>(path->pBuffer), path->Length);
		*res = self->PathAssembly(spath);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static int IFileSystem_ReadFile(hc::IVirtualMachine* vm, hc::FunctionRegister* func, eastl::string* res, IFileSystem* self, StringBuffer* path)
	{
		eastl::string spath(static_cast<const char*>(path->pBuffer), path->Length);
		IFile* file = self->FileFactory(spath);
		uint size = file->GetSize();
		char* content = NEW char[size + 1];
		content[size] = 0;
		file->ReadFile(content, size);
		file->CloseFile();
		*res = content;
		SAFE_DELETE_ARRAY(content);
		IFileSystem::Instance()->RecycleBin(file);
		return 1;
	}
	//-------------------------------------------------------------------------------------------------------
	static void Register_IFileSystem(IScriptSystem& sys)
	{
		ClassRegister sc;
		sc.ClassType(&IFileSystem::RTTI());

		{
			FunctionRegister sf;
			sf.Result( &String::RTTI());
			sf.Arguments(&String::RTTI());
			sf.Function(&hc::IFileSystem_PathAssembly, "PathAssembly");
			sc.ClassFunction(sf);
		}

		{
			FunctionRegister sf;
			sf.Result(&String::RTTI());
			sf.Arguments(&String::RTTI());
			sf.Function(&hc::IFileSystem_ReadFile, "ReadFile");
			sc.ClassFunction(sf);
		}

		{
			FunctionRegister sf;
			sf.Result( &Bool::RTTI());
			sf.Arguments(&String::RTTI());
			sf.Function(&hc::IFileSystem_isFileExist, "isFileExist");
			sc.ClassFunction(sf);
		}
		sys.SetNativeLibrary(Configure::NATIVE_LIBRARY_SYSTEM, sc);
		sys.SetNativeGlobal(Configure::NATIVE_LIBRARY_SYSTEM, IFileSystem::RTTI().GetTypeName(), IFileSystem::Instance());
	}
}