#include "thread/IThreadSystem.h"
#include "thread/IThreadMutex.h"
#include "file/IFileSystem.h"
#include "Scriptfilecache.h"
#include "log/ILogSystem.h"
#include "file/IFile.h"


using namespace hc;
using namespace eastl;
namespace hlss
{
	SINGLETON_IMPLEMENT(Scriptfilecache);
	//---------------------------------------------------------------------------------------------------------
	Scriptfilecache::Scriptfilecache()
	{
		m_pIThreadMutex = static_cast<IThreadMutex*>
			(IThreadSystem::Instance()->ThreadEntityFactory(ThreadProperty::ET_MUTEX));
	}
	//---------------------------------------------------------------------------------------------------------
	Scriptfilecache::~Scriptfilecache()
	{
		IThreadSystem::Instance()->RecycleBin(m_pIThreadMutex);
		FOR_EACH(FileMap, it, m_FileMap)
		{
			SAFE_DELETE(it->second);
		}
	}
	//---------------------------------------------------------------------------------------------------------
	const eastl::string& Scriptfilecache::GetScriptFile(uint hash, const eastl::string& path)
	{
		eastl::string* script = NULL;
		m_pIThreadMutex->Lock();
		FileMap::iterator it = m_FileMap.find(hash);
		if (m_FileMap.end() != it)
		{
			m_pIThreadMutex->Unlock();
			script = it->second;
		}
		else
		{
			m_pIThreadMutex->Unlock();
			if (IFileSystem::Instance()->isFileExist(path))
			{
				IFile* scriptfile = IFileSystem::Instance()->FileFactory(path);
				script = NEW eastl::string;
				scriptfile->OpenFile(IFile::AT_READ);
				script->resize(scriptfile->GetSize());
				scriptfile->ReadFile(&(script->at(0)), scriptfile->GetSize());
				scriptfile->CloseFile();
				IFileSystem::Instance()->RecycleBin(scriptfile);
			}
			else
			{
				HADESERROR("Script %s is no exist", path.c_str());
				script = NEW eastl::string;
			}
			m_pIThreadMutex->Lock();
			m_FileMap.insert(make_pair(hash, script));
			m_pIThreadMutex->Unlock();
		}
		return *script;
	}
}