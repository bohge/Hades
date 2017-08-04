#include "BaseFile.h"



namespace hfw
{
	//-------------------------------------------------------------------------------------------------------
	BaseFile::BaseFile(const eastl::string& url)
		:m_FileUrl(url),
		m_ActionType(AT_NONE),
		m_isOpen(false),
		m_FileSize(0),
		m_PosIndex(0),
		m_BufferSize(0),
		m_ReadedBufferSize(0),
		m_pDatas(NULL)
	{
	}
	//-------------------------------------------------------------------------------------------------------
	BaseFile::~BaseFile(void)
	{
		ASSERT(false == m_isOpen);
	}
	//-------------------------------------------------------------------------------------------------------
	bool BaseFile::OpenFile(ActionType at)
	{
		ASSERT(m_isOpen == false);
		m_isOpen = _DoOpenFile(at);
		if (m_isOpen)
		{
			switch (at)
			{
			case AT_READ:
			{
							m_BufferSize = m_FileSize > FC_FILEBUFFER ? FC_FILEBUFFER : m_FileSize;
							m_pDatas = NEW byte[m_BufferSize];
							m_ReadedBufferSize += m_BufferSize;
							_DoReadFile(m_pDatas, m_BufferSize);//重新填充
			}break;
			case AT_WRITE:
			{
							 m_BufferSize = FC_FILEBUFFER;
							 m_pDatas = NEW byte[m_BufferSize];
			}break;
			default:ASSERT(false);
			}
		}
		m_ActionType = m_isOpen ? at : AT_NONE;
		return m_isOpen;
	}
	//-------------------------------------------------------------------------------------------------------
	bool BaseFile::CloseFile()
	{
		if (m_isOpen)
		{
			if (m_PosIndex != 0 && AT_WRITE == m_ActionType)
			{
				_DoWriteFile(m_pDatas, m_PosIndex);
			}
			m_isOpen = !_DoCloseFile();
			m_PosIndex = 0;
			SAFE_DELETE_ARRAY(m_pDatas);
		}
		return !m_isOpen;
	}
	//-------------------------------------------------------------------------------------------------------
	int BaseFile::Seek(int to, int whence)
	{
		int result = 0;
		ASSERT(m_isOpen == true);
		switch (m_ActionType)
		{
		case AT_READ:
		{
						switch (whence)
						{
						case SEEK_SET:
						{
										 result = _DoSeekFile(to, whence);
										 m_ReadedBufferSize = to;
										 m_BufferSize = m_FileSize - m_ReadedBufferSize > FC_FILEBUFFER ? FC_FILEBUFFER : m_FileSize - m_ReadedBufferSize;
						}break;
						case SEEK_CUR:
						{
										 int realto = to - m_BufferSize + m_PosIndex;
										 m_ReadedBufferSize += realto;
										 result = _DoSeekFile(realto, whence);
										 m_BufferSize = m_FileSize - m_ReadedBufferSize > FC_FILEBUFFER ? FC_FILEBUFFER : m_FileSize - m_ReadedBufferSize;
						}break;
						case SEEK_END:
						{
										 result = _DoSeekFile(to, whence);
										 m_ReadedBufferSize = m_FileSize - to;
										 m_BufferSize = to > FC_FILEBUFFER ? FC_FILEBUFFER : to;
						}break;
						default: ASSERT(false);
						}
						m_PosIndex = m_BufferSize;//制空当先buffer数据堆
		}break;
		case AT_WRITE:
		{
						 if (0 != m_PosIndex)
						 {
							 _DoWriteFile(m_pDatas, m_PosIndex);//把已有数据写入
							 m_PosIndex = 0;
						 }
						 result = _DoSeekFile(to, whence);
		}break;
		default:
		{
				   ASSERT(false);
		}break;
		}
		return result;
	}
	//-------------------------------------------------------------------------------------------------------
	int BaseFile::Tell()
	{
		ASSERT(m_isOpen == true);
		switch (m_ActionType)
		{
		case AT_READ: return _DoTell() - m_BufferSize + m_PosIndex;
		case AT_WRITE:return _DoTell() + m_PosIndex;
		default: ASSERT(false); break;
		}
		return 0;
	}
	//-------------------------------------------------------------------------------------------------------
	int BaseFile::ReadFile(void* data, int bitesize)
	{
		if (!m_isOpen)
		{
			OpenFile(AT_READ);
		}
		ASSERT(AT_READ == m_ActionType);
		if (bitesize > m_BufferSize || bitesize > (m_BufferSize - m_PosIndex))
		{
			int readsize = m_BufferSize - m_PosIndex;//buffer中的剩余数据加载出来先
			memcpy(data, m_pDatas + m_PosIndex, readsize);
			int remainsize = bitesize - readsize;//剩余需要读取的
			if (remainsize > FC_FILEBUFFER)//当剩余的需要读取的大于buffer
			{
				m_ReadedBufferSize += remainsize;
				if (0 == _DoReadFile(((byte*)data) + readsize, remainsize))//重新填充,0为没有数据
				{
					return 0;
				}
				m_BufferSize = m_FileSize - m_ReadedBufferSize > FC_FILEBUFFER ? FC_FILEBUFFER : m_FileSize - m_ReadedBufferSize;
				m_ReadedBufferSize += m_BufferSize;
				m_PosIndex = 0;
				if (0 == _DoReadFile(m_pDatas, m_BufferSize))//重新填充,0为没有数据
				{
					return 0;
				}
			}
			else
			{
				m_BufferSize = m_FileSize - m_ReadedBufferSize > FC_FILEBUFFER ? FC_FILEBUFFER : m_FileSize - m_ReadedBufferSize;
				m_ReadedBufferSize += m_BufferSize;
				if (0 == _DoReadFile(m_pDatas, m_BufferSize))//重新填充,0为没有数据
				{
					return 0;
				}
				int remainsize = bitesize - readsize;//剩余需要读取的
				memcpy(((byte*)data) + readsize, m_pDatas, remainsize);
				m_PosIndex = remainsize;
			}
		}
		else
		{
			memcpy(data, m_pDatas + m_PosIndex, bitesize);
			m_PosIndex += bitesize;
		}
		return bitesize;
	}
	//-------------------------------------------------------------------------------------------------------
	int BaseFile::WriteFile(const void* data, int bitesize)
	{
		if (!m_isOpen)
		{
			OpenFile(AT_WRITE);
		}
		ASSERT(AT_WRITE == m_ActionType);
		if (bitesize > m_BufferSize || bitesize > (m_BufferSize - m_PosIndex))
		{
			Flush();
		}
		memcpy(m_pDatas + m_PosIndex, data, bitesize);
		m_PosIndex += bitesize;
		m_FileSize += bitesize;
		return bitesize;
	}
	//-------------------------------------------------------------------------------------------------------
	bool BaseFile::Flush()
	{
		bool res = false;
		if (m_PosIndex > 0);
		{
			int writed(0);
			writed = _DoWriteFile(m_pDatas, m_PosIndex);
			res = writed == m_PosIndex;
			res &= _DoFlush();
			ASSERT(m_PosIndex >= writed);
			m_PosIndex -= writed;
		}
		return res;
	}
	//-------------------------------------------------------------------------------------------------------
	const eastl::string& BaseFile::GetFilePath()
	{
		return m_FileUrl;
	}
	//-------------------------------------------------------------------------------------------------------
	uint BaseFile::GetSize()
	{
		return _GetFileSize();
	}
	//-------------------------------------------------------------------------------------------------------
	bool BaseFile::isOpen()
	{
		return m_isOpen;
	}
}