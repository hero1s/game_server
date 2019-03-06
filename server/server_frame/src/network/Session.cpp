
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
#include "helper/bufferStream.h"
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
		svrlib::base64::encode(key, reinterpret_cast<const char *>(message_digest), 20);
		char http_res[512] = "";
		sprintf(http_res, WEB_SOCKET_HANDS_RE, key);
		m_pSendBuffer->Write((uint8_t*)http_res, strlen(http_res));
		shake_hands_ = true;
		LOG_DEBUG("shake hand success,res:{}...",http_res);//fkYTdNEVkParesYkrM4S
	}
	bool Session::FindHttpParam(const char * param, const char * buf) {
		while (*param == *buf) {
			if (*(param + 1) == '\0') return true;
			++param; ++buf;
		}
		return false;
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
	ws_head_.reset();
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
	shake_hands_ 	 = false;
	ws_head_.reset();
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

	shake_hands_ 	 = false;
	ws_head_.reset();

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
		auto stream = CBufferStream::buildStream();
		stream.write_((uint8_t)0x82);//写头部
		//写长度
		if (wSize >= 126) {//7位放不下
			if (wSize <= 0xFFFF) {//16位放
				stream.write_((uint8_t)126);
				stream.write_((uint16_t)htons((u_short)wSize));
			} else {//64位放
				stream.write_((uint8_t)127);
				//stream.write_((uint64_t)OrderSwap64(wSize));
			}
		} else {
			stream.write_((uint8_t)wSize);
		}
		//写数据
		stream.write(wSize,pMsg);
		if (m_pSendBuffer->Write((uint8_t*)stream.getBuffer(), stream.getPosition()) == false)
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

	uint8_t* pPacket = nullptr;
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
		uint8_t* pPacket = nullptr;
		if(!shake_hands_){
			pPacket = m_pRecvBuffer->GetFirstPacketPtr(m_pRecvBuffer->GetRecvDataLen());
			ShakeHandsHandle((const char*)pPacket,m_pRecvBuffer->GetRecvDataLen());
			if(shake_hands_){
				m_pRecvBuffer->Clear();
				LOG_DEBUG("握手成功，开始接受数据");
				return true;
			}else{
				LOG_DEBUG("握手失败，等待或者断开");
				return true;
			}
		}
		while (m_QueueMessage.size() < m_pNetworkObject->MaxTickPacket()) {
			//读取websocket固定包头
			if (!ws_head_.rh) {
				//这个包不够一个头部的大小
				if (m_pRecvBuffer->GetRecvDataLen() < 2) {
					break;
				}
				//读取
				uint8_t head = 0;
				m_pRecvBuffer->ReadBuf(&head,1);
				ws_head_.fin = head >> 7;
				ws_head_.opcode = head & 0xF;
				m_pRecvBuffer->ReadBuf(&head,1);
				ws_head_.len = head & 0x7F;
				ws_head_.mask = head >> 7;
				ws_head_.rh = 1;//标记头部读取完成
			}
			//读取长度
			if (!ws_head_.rl) {
				uint8_t nsize = ws_head_.GetLenNeedByte();
				if (nsize) {
					//这个包不够一个长度
					if (m_pRecvBuffer->GetRecvDataLen() < nsize) {
						break;
					}
					if (nsize == 2) {
						m_pRecvBuffer->ReadBuf(&ws_head_.ex_len.v16, sizeof(ws_head_.ex_len.v16));
						ws_head_.ex_len.v16 = ntohs(ws_head_.ex_len.v16);
					} else {
						m_pRecvBuffer->ReadBuf(&ws_head_.ex_len.v64, sizeof(ws_head_.ex_len.v64));
						ws_head_.ex_len.v64 = ntohl((u_long)ws_head_.ex_len.v64);
					}
				}
				ws_head_.rl = 1;
			}
			//读取MKEY
			if (!ws_head_.rk) {
				if (ws_head_.mask) {
					//这个包不够一个key
					if (m_pRecvBuffer->GetRecvDataLen() < 4) {
						break;
					}
					m_pRecvBuffer->ReadBuf(&ws_head_.mkey[0],1);
					m_pRecvBuffer->ReadBuf(&ws_head_.mkey[1],1);
					m_pRecvBuffer->ReadBuf(&ws_head_.mkey[2],1);
					m_pRecvBuffer->ReadBuf(&ws_head_.mkey[3],1);
				}
				ws_head_.rk = 1;
			}
			//读取数据段
			uint64_t data_len = ws_head_.GetLen();
			if (m_pRecvBuffer->GetRecvDataLen() < data_len) {
				break;
			}
			if (ws_head_.mask) {
				pPacket = m_pRecvBuffer->GetFirstPacketPtr(data_len);
				for (size_t i = 0; i < data_len; ++i) {
					pPacket[i] = pPacket[i] ^ ws_head_.mkey[i % 4];
				}
			}
			//放入消息队列
			auto message = std::make_shared<CMessage>(pPacket, data_len);
			m_QueueMessage.push(message);
			//移除缓存
			m_pRecvBuffer->RemoveFirstPacket(data_len);
			LOG_DEBUG("收到web消息:{}--len:{}",string((char*)(message->Data()),message->Length()),data_len);
			if (ws_head_.opcode == OPCODE_CLR) {
				LOG_DEBUG("websocket closed");
				return false;
			}
			ws_head_.reset();
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


