
#include "Network.h"
#include "Session.h"
#include "SessionPool.h"
#include "SendBuffer.h"
#include "RecvBuffer.h"
#include "network/NetworkObject.h"
#include "IoHandler.h"
#include "svrlib.h"

using namespace std;

namespace Network
{
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
	m_bCanSend           = TRUE;
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
	if (m_pSendBuffer->Write(pMsg, wSize) == FALSE)
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
		shared_ptr<CMessage> message(new CMessage(pPacket, iPacketLen));
		m_QueueMessage.push(message);
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
int Session::OnSend()
{
	LOG_DEBUG("[Session::OnSend]");

	m_lockSend.Lock();
	m_bCanSend = TRUE;
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
		if (m_pSendBuffer->GetSendParam(&buf, len) == FALSE)
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
			m_bCanSend = FALSE;
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


