int len = evbuffer_get_length(m_rpInputBuffer);\\读取网络中buffer长度
byte* buffer = m_pRawbuffer->NewBuffer(len);\\准备该长度buffer
bufferevent_read(m_pBufferevent, buffer, len);\\读入buffer
Message msg;
while (m_pRawbuffer->UnpackMessage(msg))\\拆分消息
{
	m_rpHost->OnReceive(m_ID, msg)\\处理消息
}
m_pRawbuffer->Cleanup();\\收尾