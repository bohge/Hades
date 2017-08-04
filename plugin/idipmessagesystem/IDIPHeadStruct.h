#pragma once
#include "core/Predefine.h"
#include "tlbbsy_idip.h"
#ifdef WIN32
#include <winsock2.h>
#else
#include <arpa/inet.h>
#endif

#pragma pack(1)
namespace hms
{
	struct IDIPHeadStruct
	{
	private:
		uint32_t  dwPacketLen;                          /* 包长 */
		uint32_t  dwCmdid;                              /* 命令ID */
		uint32_t  dwSeqid;                              /* 流水号 */
		char      szServiceName[SERVICE_NAME_LENGTH];   /* 服务名 */
		uint32_t  dwSendTime;                           /* 发送时间YYYYMMDD对应的整数 */
		uint32_t  dwVersion;                            /* 版本号 */
		char      szAuthenticate[AUTHENTICATE_LENGTH];  /* 加密串 */
		int32_t   iResult;                              /* 错误码,返回码类型：0：处理成功，需要解开包体获得详细信息,1：处理成功，但包体返回为空，不需要处理包体（eg：查询用户角色，用户角色不存在等），-1: 网络通信异常,-2：超时,-3：数据库操作异常,-4：API返回异常,-5：服务器忙,-6：其他错误,小于-100 ：用户自定义错误，需要填写szRetErrMsg */
		char      szRetErrMsg[ERROR_MSG_LENGTH];        /* 错误信息 */
	public:
		HADES_FORCEINLINE static uint GetHeadSize()
		{
			return sizeof(IDIPHeadStruct);
		}
	public:
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE bool isValid() const
		{
			return true;
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE void SetID(uint id)
		{
			dwCmdid = htonl(id);
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE void SetRawLength(uint len)
		{
			dwPacketLen = htonl(len);
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE uint GetID() const
		{
			return ntohl(dwCmdid);
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE uint GetMessageLength() const
		{
			return ntohl(dwPacketLen) - sizeof(IDIPHeadStruct);
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE uint GetRawLength() const
		{
			return ntohl(dwPacketLen);
		}
		//---------------------------------------------------------------------------------------------------------
		HADES_FORCEINLINE uint GetResult() const
		{
			return ntohl(iResult);
		}
	};
}
#pragma pack()