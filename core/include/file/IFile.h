#pragma once
#include "core/Object.hpp"
#include "core/Rtti.hpp"


#include <EASTL/string.h>

namespace hc
{
	//基本类型
	class HADES_CORE_API IFile : public Object
	{
		RTTI_ABSTRACT_TYPE_DECLEAR(IFile);
	public:
		enum ActionType
		{
			AT_NONE = 0, //没有初始化的
			AT_READ,
			AT_WRITE,
		};
	public:
		IFile(void){};
		virtual ~IFile(void){};
	public:
		virtual bool OpenFile( ActionType at ) = 0;
		virtual bool isOpen() = 0;
		virtual bool CloseFile() = 0;
		virtual int Seek( int to, int whence ) = 0;
		virtual int Tell() = 0;
		virtual bool Flush() = 0;
		virtual int ReadFile( void* data, int bitesize ) = 0;
		virtual int WriteFile( const void* data, int bitesize ) = 0;
		virtual const eastl::string& GetFilePath() = 0;
		virtual uint GetSize() = 0;
	};
}