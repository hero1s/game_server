
#include "Network.h"
#include "Session.h"
#include "SessionPool.h"
#include "SendBuffer.h"
#include "RecvBuffer.h"
#include "network/NetworkObject.h"
#include "IoHandler.h"
#include "svrlib.h"
#include "crypt/base64.hpp"
#include "crypt/sha1.h"
#include <istream>
#include <iostream>
#include <arpa/inet.h>
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif

#define WEB_SOCKET_HANDS_RE "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: %s\r\n\r\n"
#define MAGIC_KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

using namespace std;

namespace Network
{
	void Session::ShakeHandsHandle(const char *buf, int buflen) {
		char key[512];
		memset(key, 0, 512);
		for (int i = 0; i < buflen; ++i) {
			if (FindHttpParam("Sec-WebSocket-Key", buf + i)) {
				short k = i + 17, ki = 0;
				while (*(buf + k) != '\r' && *(buf + k) != '\n') {
					if (*(buf + k) == ':' || *(buf + k) == ' ') {
						++k;
						continue;
					} else {
						key[ki++] = *(buf + k);
					}
					++k;
				}
				break;
			}
		}
		LOG_DEBUG("key:{}...", key);
		memcpy(key + strlen(key), MAGIC_KEY, sizeof(MAGIC_KEY));
		LOG_DEBUG("megerkey:{}...", key);
		//求哈希1
		SHA1 sha;
		unsigned int message_digest[5];
		sha.Reset();
		sha << key;
		sha.Result(message_digest);
		for (int i = 0; i < 5; i++) {
			message_digest[i] = htonl(message_digest[i]);
		}

		memset(key, 0, 512);
		base64::encode(key, reinterpret_cast<const char *>(message_digest), 20);
		char http_res[512] = "";
		sprintf(http_res, WEB_SOCKET_HANDS_RE, key);
		Send((uint8_t*)http_res, strlen(http_res));
		LOG_DEBUG("shake hand success,res:{}...",http_res);//fkYTdNEVkParesYkrM4S
		shake_hands_ = true;
	}
	bool Session::FindHttpParam(const char * param, const char * buf) {
		while (*param == *buf) {
			if (*(param + 1) == '\0') return true;
			++param; ++buf;
		}
		return false;
	}
	int Session::wsDecodeFrame(std::string inFrame, std::string &outMessage)
	{
		LOG_DEBUG("解包:{}--len:{}",inFrame,inFrame.length());
		int ret = WS_OPENING_FRAME;
		const char *frameData = inFrame.c_str();
		const int frameLength = inFrame.size();
		if (frameLength < 2)
		{
			ret = WS_ERROR_FRAME;
			LOG_DEBUG("长度小于2");
		}

		// 检查扩展位并忽略
		if ((frameData[0] & 0x70) != 0x0)
		{
			ret = WS_ERROR_FRAME;
			LOG_DEBUG("扩展位错误");
		}

		// fin位: 为1表示已接收完整报文, 为0表示继续监听后续报文
		ret = (frameData[0] & 0x80);
		if ((frameData[0] & 0x80) != 0x80)
		{
			ret = WS_ERROR_FRAME;
			LOG_DEBUG("未完整报文或继续监听");
		}

		// mask位, 为1表示数据被加密
		if ((frameData[1] & 0x80) != 0x80)
		{
			ret = WS_ERROR_FRAME;
			LOG_DEBUG("是否加密");
		}

		// 操作码
		uint16_t payloadLength = 0;
		uint8_t payloadFieldExtraBytes = 0;
		uint8_t opcode = static_cast<uint8_t >(frameData[0] & 0x0f);
		if (opcode == WS_TEXT_FRAME)
		{
			// 处理utf-8编码的文本帧
			payloadLength = static_cast<uint16_t >(frameData[1] & 0x7f);
			if (payloadLength == 0x7e)
			{
				uint16_t payloadLength16b = 0;
				payloadFieldExtraBytes = 2;
				memcpy(&payloadLength16b, &frameData[2], payloadFieldExtraBytes);
				payloadLength = ntohs(payloadLength16b);
			}
			else if (payloadLength == 0x7f)
			{
				// 数据过长,暂不支持
				ret = WS_ERROR_FRAME;
				LOG_DEBUG("数据长度过长");
			}
		}
		else if (opcode == WS_BINARY_FRAME || opcode == WS_PING_FRAME || opcode == WS_PONG_FRAME)
		{
			// 二进制/ping/pong帧暂不处理
			LOG_DEBUG("二进制ping/pong");
		}
		else if (opcode == WS_CLOSING_FRAME)
		{
			ret = WS_CLOSING_FRAME;
			LOG_DEBUG("socket已关闭");
		}
		else
		{
			ret = WS_ERROR_FRAME;
			LOG_DEBUG("其它错误");
		}

		// 数据解码
		if ((ret != WS_ERROR_FRAME) && (payloadLength > 0))
		{
			// header: 2字节, masking key: 4字节
			const char *maskingKey = &frameData[2 + payloadFieldExtraBytes];
			char *payloadData = new char[payloadLength + 1];
			memset(payloadData, 0, payloadLength + 1);
			memcpy(payloadData, &frameData[2 + payloadFieldExtraBytes + 4], payloadLength);
			for (int i = 0; i < payloadLength; i++)
			{
				payloadData[i] = payloadData[i] ^ maskingKey[i % 4];
			}

			outMessage = payloadData;
			delete[] payloadData;
		}

		return ret;
	}
	int Session::wsEncodeFrame(std::string inMessage, std::string &outFrame, enum WS_FrameType frameType)
	{
		int ret = WS_EMPTY_FRAME;
		const uint32_t messageLength = inMessage.size();
		if (messageLength > 32767)
		{
			// 暂不支持这么长的数据
			return WS_ERROR_FRAME;
		}

		uint8_t payloadFieldExtraBytes = (messageLength <= 0x7d) ? 0 : 2;
		// header: 2字节, mask位设置为0(不加密), 则后面的masking key无须填写, 省略4字节
		uint8_t frameHeaderSize = 2 + payloadFieldExtraBytes;
		uint8_t *frameHeader = new uint8_t[frameHeaderSize];
		memset(frameHeader, 0, frameHeaderSize);
		// fin位为1, 扩展位为0, 操作位为frameType
		frameHeader[0] = static_cast<uint8_t>(0x80 | frameType);

		// 填充数据长度
		if (messageLength <= 0x7d)
		{
			frameHeader[1] = static_cast<uint8_t>(messageLength);
		}
		else
		{
			frameHeader[1] = 0x7e;
			uint16_t len = htons(messageLength);
			memcpy(&frameHeader[2], &len, payloadFieldExtraBytes);
		}

		// 填充数据
		uint32_t frameSize = frameHeaderSize + messageLength;
		char *frame = new char[frameSize + 1];
		memcpy(frame, frameHeader, frameHeaderSize);
		memcpy(frame + frameHeaderSize, inMessage.c_str(), messageLength);
		frame[frameSize] = '\0';
		outFrame = frame;

		delete[] frame;
		delete[] frameHeader;
		return ret;
	}
	int32_t Session::GetWebSocketPacketLen(const uint8_t* pData, uint16_t wLen){
		if(shake_hands_){
			// 检查扩展位并忽略
			if ((pData[0] & 0x70) != 0x0)
			{
				return -1;
			}
			// fin位: 为1表示已接收完整报文, 为0表示继续监听后续报文
			if ((pData[0] & 0x80) != 0x80)
			{
				return -1;
			}
			// mask位, 为1表示数据被加密
			if ((pData[1] & 0x80) != 0x80)
			{
				return -1;
			}

			// 操作码
			uint16_t payloadLength = 0;
			uint8_t payloadFieldExtraBytes = 0;
			uint8_t opcode = static_cast<uint8_t >(pData[0] & 0x0f);
			if (opcode == WS_TEXT_FRAME)
			{
				// 处理utf-8编码的文本帧
				payloadLength = static_cast<uint16_t >(pData[1] & 0x7f);
				if (payloadLength == 0x7e)
				{
					uint16_t payloadLength16b = 0;
					payloadFieldExtraBytes = 2;
					memcpy(&payloadLength16b, &pData[2], payloadFieldExtraBytes);
					payloadLength = ntohs(payloadLength16b);
				}
				else if (payloadLength == 0x7f)
				{
					// 数据过长,暂不支持
					return -1;
				}
				return 2+payloadFieldExtraBytes+payloadLength;
			}
			else if (opcode == WS_BINARY_FRAME || opcode == WS_PING_FRAME || opcode == WS_PONG_FRAME)
			{
				// 二进制/ping/pong帧暂不处理
				return 2+payloadFieldExtraBytes+payloadLength;
			}
			else if (opcode == WS_CLOSING_FRAME)
			{
				return -1;
			}
			else
			{
				return -1;
			}
		}else{
			return wLen;
		}
	}

//=============================================================================================================================
/**
	@remarks
	@param	dwSendBufferSize
	@param	dwRecvBufferSize
	@param	dwTimeOut
*/
//=============================================================================================================================
Session::Session(uint32_t dwSendBufferSize, uint32_t dwRecvBufferSize, uint32_t dwMaxPacketSize, uint32_t dwTimeOut)
{
	m_pSendBuffer = new SendBuffer();
	m_pSendBuffer->Create(dwSendBufferSize, 0);// modify toney

	m_pRecvBuffer = new RecvBuffer();
	m_pRecvBuffer->Create(dwRecvBufferSize, dwMaxPacketSize);

	m_dwTimeOut     = dwTimeOut;
	m_socket        = INVALID_SOCKET;
	m_bAcceptSocket = false;
	m_bCanSend      = true;

	m_pNetworkObject = NULL;
	m_openMsgQueue   = false;
	m_wMaxPacketSize = dwMaxPacketSize;
	m_webSocket		 = false;
	shake_hands_ 	 = false;

	ResetTimeOut();
}

Session::~Session()
{
	CloseSocket();

	if (m_pSendBuffer) delete m_pSendBuffer;
	if (m_pRecvBuffer) delete m_pRecvBuffer;
}
void Session::SetOpenMsgQueue(bool openMsgQueue)
{
	m_openMsgQueue = openMsgQueue;
}
void Session::SetWebSocket(bool webSocket){
    m_webSocket = webSocket;
    if(m_webSocket)m_openMsgQueue = true;
}
//=============================================================================================================================
/**
	@remarks
	@param	socket
	@param	sockaddr
	@retval	int
*/
//=============================================================================================================================
void Session::Init()
{
	m_pSendBuffer->Clear();
	m_pRecvBuffer->Clear();

	ResetKillFlag();

	m_bDisconnectOrdered = false;
	m_bCanSend           = true;
	ResetTimeOut();
}
//=============================================================================================================================
/**
	@remarks
	@param	pMsg
	@param	wSize
	@retval	int
*/
//=============================================================================================================================
bool Session::Send(uint8_t* pMsg, uint16_t wSize)
{
	if(m_webSocket){
		if(!shake_hands_)return false;
		string smsg((const char*)pMsg,wSize);
		string outMsg;
		wsEncodeFrame(smsg,outMsg,WS_TEXT_FRAME);
		LOG_DEBUG("wsSendMsg {} --> {}",smsg,outMsg);
		if (m_pSendBuffer->Write(pMsg, wSize) == false)
		{
			LOG_ERROR("m_pSendBuffer->Write fail. data length = {}, {},ip:{}", m_pSendBuffer->GetLength(), wSize, GetIP());
			Remove();
			return false;
		}
		return true;
	}
	if (m_pSendBuffer->Write(pMsg, wSize) == false)
	{
		LOG_ERROR("m_pSendBuffer->Write fail. data length = {}, {},ip:{}", m_pSendBuffer->GetLength(), wSize, GetIP());
		Remove();
		return false;
	}
	return true;
}
bool Session::ProcessRecvdPacket()
{
	if (m_openMsgQueue)
	{// 开启队列模式
		return HandleRecvMessage();
	}

	uint8_t* pPacket;
	uint32_t msgNum = 0;
	while (m_pRecvBuffer->GetRecvDataLen() >= m_pNetworkObject->GetHeadLen() && (msgNum++) < m_pNetworkObject->MaxTickPacket())
	{
		pPacket             = m_pRecvBuffer->GetFirstPacketPtr(m_pNetworkObject->GetHeadLen());
		uint16_t iPacketLen = m_pNetworkObject->GetPacketLen(pPacket, m_pNetworkObject->GetHeadLen());
		if (iPacketLen >= m_wMaxPacketSize)
		{
			LOG_ERROR("max packet is big than:{},ip:{}", iPacketLen, GetIP());
			return false;
		}
		pPacket = m_pRecvBuffer->GetFirstPacketPtr(iPacketLen);
		if (pPacket == NULL)
			return true;

		int iRet = m_pNetworkObject->OnRecv(pPacket, iPacketLen);
		if (iRet < 0)
		{
			LOG_DEBUG("process msg return < 0,disconnect");
			return false;
		}

		m_pRecvBuffer->RemoveFirstPacket(iPacketLen);

		ResetTimeOut();
	}
	return true;
}
//处理消息
bool Session::HandleRecvMessage()
{
	uint32_t msgNum = 0;
	while (!m_QueueMessage.empty() && (msgNum++) < m_pNetworkObject->MaxTickPacket())
	{
		auto message = m_QueueMessage.pop();
		int  iRet    = m_pNetworkObject->OnRecv(message->Data(), message->Length());
		if (iRet < 0)
		{
			LOG_ERROR("process msg return < 0,disconnect");
			return false;
		}
		ResetTimeOut();
	}
	return true;
}
//解码消息到消息队列
bool Session::DecodeMsgToQueue()
{
	if(m_webSocket)return DecodeWebSocketToQueue();

	uint8_t* pPacket;
	while (m_pRecvBuffer->GetRecvDataLen() >= m_pNetworkObject->GetHeadLen() && (m_QueueMessage.size() < m_pNetworkObject->MaxTickPacket()))
	{
		pPacket = m_pRecvBuffer->GetFirstPacketPtr(m_pNetworkObject->GetHeadLen());
		uint32_t iPacketLen = m_pNetworkObject->GetPacketLen(pPacket, m_pNetworkObject->GetHeadLen());
		if (iPacketLen >= m_wMaxPacketSize)
		{
			LOG_ERROR("max packet is big than:{},ip:{}", iPacketLen, GetIP());
			return false;
		}
		pPacket = m_pRecvBuffer->GetFirstPacketPtr(iPacketLen);
		if (pPacket == NULL)
			return true;
		//放入消息队列
		auto message = std::make_shared<CMessage>(pPacket, iPacketLen);
		m_QueueMessage.push(message);
		//移除缓存
		m_pRecvBuffer->RemoveFirstPacket(iPacketLen);

		ResetTimeOut();
	}
	return true;
}
//解码websocket消息到队列
bool Session::DecodeWebSocketToQueue(){
		uint8_t* pPacket;
		if(!shake_hands_){
			pPacket = m_pRecvBuffer->GetFirstPacketPtr(m_pRecvBuffer->GetRecvDataLen());
			ShakeHandsHandle((const char*)pPacket,m_pRecvBuffer->GetRecvDataLen());
			if(shake_hands_){
				m_pRecvBuffer->Clear();
				return true;
			}else{
				LOG_DEBUG("握手失败，等待或者断开");
				return true;
			}
		}
		while (m_pRecvBuffer->GetRecvDataLen() >= 2 && (m_QueueMessage.size() < m_pNetworkObject->MaxTickPacket()))
		{
			pPacket = m_pRecvBuffer->GetFirstPacketPtr(2);
			int32_t iPacketLen = GetWebSocketPacketLen(pPacket, m_pNetworkObject->GetHeadLen());
			if(iPacketLen < 0)break;

			if (iPacketLen >= m_wMaxPacketSize)
			{
				LOG_ERROR("max packet is big than:{},ip:{}", iPacketLen, GetIP());
				return false;
			}
			pPacket = m_pRecvBuffer->GetFirstPacketPtr(iPacketLen);
			if (pPacket == NULL)
				return true;
			//解码webSocket
			string outMsg;
			auto iRet = wsDecodeFrame(string((const char*)pPacket,iPacketLen),outMsg);
			if(WS_ERROR_FRAME != iRet){
				//放入消息队列
				auto message = std::make_shared<CMessage>(outMsg.c_str(),outMsg.length());
				m_QueueMessage.push(message);
			}else{
				LOG_DEBUG("websocket 解码错误:{}",iRet);
				return false;
			}
			//移除缓存
			m_pRecvBuffer->RemoveFirstPacket(iPacketLen);
			ResetTimeOut();
		}

	return true;
}

//=============================================================================================================================
/**
	@remarks
			send.
	@retval int
			- TRUE.
			- send FALSE.
*/
//=============================================================================================================================
bool Session::OnSend()
{
	LOG_DEBUG("[Session::OnSend]");

	m_lockSend.Lock();
	m_bCanSend = true;
	m_lockSend.Unlock();

	return m_bCanSend;
}

int Session::PreSend(IoHandler* pIoHandler)
{
	if (!m_bRemove && m_bCanSend && m_pSendBuffer->IsReadyToSend())
	{
		// add to io_thread
		struct epoll_event event;
		event.events   = 0x800;
		event.data.ptr = this;

		pIoHandler->AddIoEvent(&event);
	}
	return TRUE;
}

int Session::DoSend(IoHandler* pIoHandler)
{
	TGuard gd(m_lockSend);

	if (m_bCanSend && m_bRemove == FALSE)
	{
		uint8_t* buf;
		int len;
		if (m_pSendBuffer->GetSendParam(&buf, len) == false)
			return TRUE;

		assert (len > 0);

		int ret = send(m_socket, buf, len, 0);
		if (ret == SOCKET_ERROR)
		{
			if (errno == EAGAIN)
			{
				ret = 0;
			}
			else
			{
				LOG_DEBUG("[Session::DoSend] send error = {} .", errno);
				return FALSE;
			}
		}

		if (ret < len)
		{
			// now the send buffer is full, wait EPOLLOUT then send
			m_bCanSend = false;
			uint32_t event = EPOLLIN | EPOLLOUT | EPOLLET | EPOLLERR | EPOLLHUP;
			pIoHandler->ModEpollEvent(this, event);

			LOG_DEBUG("[Session::DoSend] send ret = {}/1024, len = {}/1024 EAGAIN.", ret, len);
		}

		m_pSendBuffer->Completion(ret);
	}

	return TRUE;
}

//=============================================================================================================================
/**
	@remarks
			IOCP recv.
	@retval int
			recv FALSE.
*/
//=============================================================================================================================
int Session::DoRecv()
{
	TGuard gd(m_lockRecv);

	char* buf;
	int ret = 0, len = 0;

	while (m_bRemove == FALSE)
	{
		m_pRecvBuffer->GetRecvParam((uint8_t**) &buf, len);
		if (len <= 0)
		{
			LOG_ERROR("[Session::OnRecv] no more recv buffer.");
			Remove();
			return FALSE;
		}

		ret = recv(m_socket, buf, len, 0);
		if (ret == SOCKET_ERROR)
		{
			if (errno == EAGAIN)
			{
				//OnLogString("[Session::OnRecv] recv error = EAGAIN .");
				return TRUE;
			}
			else
			{
				LOG_DEBUG("[Session::OnRecv] recv error = {} .", errno);
				Remove();
				return FALSE;
			}
		}
		if (ret == 0) // peer closed
		{
			LOG_DEBUG("[Session::OnRecv] recv ret = 0.peer disconnect");
			Remove();

			return FALSE;
		}
		m_pRecvBuffer->Completion(ret);
		if (ret < len)
			break;
	}
	if (m_openMsgQueue)
	{// 开启队列模式
		if (!DecodeMsgToQueue())
		{
			LOG_ERROR("DecodeMsg Error:ip:{},Remove",GetIP());
			Remove();
		}
	}
	return TRUE;
}

SOCKET Session::CreateSocket()
{
	int nRet  = 0;
	int nZero = 0;

	SOCKET newSocket = socket(AF_INET, SOCK_STREAM, 0);

	if (newSocket == INVALID_SOCKET)
	{
		return newSocket;
	}

	SocketOpt::Nonblocking(newSocket);
	SocketOpt::DisableBuffering(newSocket);

	return newSocket;
}

void Session::BindNetworkObject(NetworkObject* pNetworkObject)
{
	m_pNetworkObject = pNetworkObject;
	pNetworkObject->SetSession(this);
}

void Session::UnbindNetworkObject()
{
	if (m_pNetworkObject == NULL)
	{
		return;
	}
	m_pNetworkObject->SetSession(NULL);

	m_pNetworkObject = NULL;
}

void Session::OnAccept()
{
	ResetKillFlag();

	ResetTimeOut();

	m_pNetworkObject->OnAccept(GetIndex());
}

void Session::OnConnect(bool bSuccess)
{
	Init();

	NetworkObject* pNetworkObject = m_pNetworkObject;

	if (!bSuccess)
	{
		UnbindNetworkObject();
	}

	pNetworkObject->OnConnect(bSuccess, GetIndex());
}

void Session::Disconnect(bool bGracefulDisconnect)
{
	if (bGracefulDisconnect)
	{
		Remove();
	}
	else
	{
		m_bDisconnectOrdered = true;
	}
}

}


