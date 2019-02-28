
#pragma once

#include "Double_List_T.h"
#include "SocketOpt.h"
#include "TLock.h"
#include "Message.h"
#include <string>

namespace Network
{

class SendBuffer;
class RecvBuffer;
class SessionPool;
class NetworkObject;
class Session;
class IoHandler;
class CMessageDecode;

//=============================================================================================================================
/**
	@remarks
	@par
			- OnAccept:		accept
			- OnDisConnect:	
			- OnRecv:		
			- OnConnect:	connect
			- OnUpdate:		IO Update
	@note

*/
//=============================================================================================================================
class Session : public LinkD_T<Session>
{
	friend void* io_thread(void* param);
	friend class SessionPool;

    enum WS_FrameType {
        WS_EMPTY_FRAME = 0xF0,
        WS_ERROR_FRAME = 0xF1,
        WS_TEXT_FRAME = 0x01,
        WS_BINARY_FRAME = 0x02,
        WS_PING_FRAME = 0x09,
        WS_PONG_FRAME = 0x0A,
        WS_OPENING_FRAME = 0xF3,
        WS_CLOSING_FRAME = 0x08
    };
private://websocket
    void ShakeHandsHandle(const char *buf, int buflen);

    bool FindHttpParam(const char *param, const char *buf);

    int wsDecodeFrame(std::string inFrame, std::string &outMessage);

    int wsEncodeFrame(std::string inMessage, std::string &outFrame, enum WS_FrameType frameType);

    int32_t GetWebSocketPacketLen(const uint8_t* pData, uint16_t wLen);

private:
    bool shake_hands_;//是否已经握手


public:
	/// dwTimeOut .
	Session(uint32_t dwSendBufferSize, uint32_t dwRecvBufferSize, uint32_t dwMaxPacketSize, uint32_t dwTimeOut);
	virtual ~Session();

	void SetOpenMsgQueue(bool openMsgQueue);
	void SetWebSocket(bool webSocket);

	void Init();
	bool Send(uint8_t* pMsg, uint16_t wSize);

	bool OnSend();
	int DoSend(IoHandler* pIoHandler);
	int DoRecv();
	int PreSend(IoHandler* pIoHandler);
	SOCKET CreateSocket();
	bool ProcessRecvdPacket();

	//处理消息
	bool HandleRecvMessage();
	//解码消息到消息队列
	bool DecodeMsgToQueue();
	//解码websocket消息到队列
	bool DecodeWebSocketToQueue();

	void BindNetworkObject(NetworkObject* pNetworkObject);
	void UnbindNetworkObject();
	void OnAccept();
	void OnConnect(bool bSuccess);

	inline void SetSocket(SOCKET socket)
	{
		m_socket = socket;
	}
	inline void SetSockAddr(SOCKADDR_IN& sockaddr)
	{
		m_sockaddr = sockaddr;
	}
	inline void CloseSocket()
	{
		SocketOpt::CloseSocket(m_socket);
		m_socket = INVALID_SOCKET;
	}
	inline NetworkObject* GetNetworkObject()
	{
		return m_pNetworkObject;
	}
	inline SendBuffer* GetSendBuffer()
	{
		return m_pSendBuffer;
	}                ///<
	inline RecvBuffer* GetRecvBuffer()
	{
		return m_pRecvBuffer;
	}                ///<
	inline SOCKET GetSocket()
	{
		return m_socket;
	}                    ///<
	inline SOCKADDR_IN* GetSockAddr()
	{
		return &m_sockaddr;
	}        ///<
	inline char* GetIP()
	{
		return inet_ntoa(m_sockaddr.sin_addr);
	}    ///<
	inline unsigned long GetIPNumber()
	{
		return m_sockaddr.sin_addr.s_addr;
	}

	inline uint32_t GetIdleTick() const
	{
		return m_dwTimeOut ? m_dwLastSyncTick + m_dwTimeOut : 0;
	}
	inline uint32_t GetLastSyncTick()
	{
		return m_dwLastSyncTick;
	}
	inline bool IsOnIdle()
	{
		return m_dwTimeOut ? GetIdleTick() < time(NULL) : false;
	}
	inline uint32_t GetIndex()
	{
		return m_dwIndex;
	}
	inline bool IsAcceptSocket()
	{
		return m_bAcceptSocket;
	}
	inline void SetAcceptSocketFlag()
	{
		m_bAcceptSocket = true;
	}
	void Remove()
	{
		__sync_fetch_and_or(&m_bRemove, 1); /*m_bRemove = 1;*/ }
	inline void ResetKillFlag()
	{
		__sync_fetch_and_and(&m_bRemove, 0); /*m_bRemove = 0;*/ }
	inline int ShouldBeRemoved()
	{
		return m_bRemove;
	}
	void Disconnect(bool bGracefulDisconnect);
	inline bool HasDisconnectOrdered()
	{
		return m_bDisconnectOrdered;
	}

private:
	void SetIndex(uint32_t index)
	{
		m_dwIndex = index;
	}
	inline void ResetTimeOut()
	{
		m_dwLastSyncTick = time(NULL);
	}

	NetworkObject* m_pNetworkObject;
	SendBuffer   * m_pSendBuffer;
	RecvBuffer   * m_pRecvBuffer;

	SOCKET      m_socket;
	SOCKADDR_IN m_sockaddr;

	uint32_t     m_dwLastSyncTick;
	volatile int m_bRemove;
	uint32_t     m_dwTimeOut;
	uint32_t     m_dwIndex;
	bool         m_bAcceptSocket;
	bool         m_bDisconnectOrdered;

	TLock m_lockRecv;
	TLock m_lockSend;
	int   m_bCanSend;
	svrlib::LockedQueue<std::shared_ptr<CMessage> > m_QueueMessage;//消息队列
	bool  m_openMsgQueue;//是否开启消息队列模式
	bool  m_webSocket;	 // 是否websocket
	uint16_t   m_wMaxPacketSize;

};

}

	
 