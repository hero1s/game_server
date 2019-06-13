
#pragma once

#include "Double_List_T.h"
#include "SocketOpt.h"
#include "TLock.h"
#include "Message.h"
#include <string>
#include "network/IOCPServer.h"

namespace Network {

    class SendBuffer;

    class RecvBuffer;

    class SessionPool;

    class NetworkObject;

    class Session;

    class IoHandler;

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
    class Session : public LinkD_T<Session> {
        friend void *io_thread(void *param);

        friend class SessionPool;

        enum WebSocketOpcode { //操作码定义类型
            OPCODE_MID = 0x0,//标识一个中间数据包
            OPCODE_TXT = 0x1,//标识一个text类型数据包
            OPCODE_BIN = 0x2,//标识一个binary类型数据包
            //0x3 - 7：保留
                    OPCODE_CLR = 0x8,//标识一个断开连接类型数据包
            OPCODE_PIN = 0x9,//标识一个ping类型数据包
            OPCODE_PON = 0xA,//表示一个pong类型数据包
        };

#pragma pack(push, 1)

        struct WebSocketHead {
            uint8_t fin : 1;//标识是否为此消息的最后一个数据包
            uint8_t rsv1 : 1;//保留位1
            uint8_t rsv2 : 1;//保留位2
            uint8_t rsv3 : 1;//保留位3
            uint8_t opcode : 4;//操作码

            uint8_t mask : 1; //是否需要掩码
            uint8_t len : 7;//长度
            union {
                uint16_t v16;//长度为126时
                uint64_t v64;//长度为127时
            } ex_len;
            uint8_t mkey[4];
            uint8_t rh : 1;//head读取完成
            uint8_t rl : 1;//len读取完成
            uint8_t rk : 1;//mkey读取完成
            uint8_t rs : 5;//扩展保留
            WebSocketHead(void) { reset(); }

            void reset(void) { memset(this, 0, sizeof(WebSocketHead)); }

            inline uint64_t GetLen(void) {
                if (len == 126)
                {
                    return ex_len.v16;
                }
                else if (len == 127)
                {
                    return ex_len.v64;
                }
                return len;
            }

            inline uint8_t GetLenNeedByte(void) {
                if (len == 126)
                {
                    return 2;
                }
                else if (len == 127)
                {
                    return 8;
                }
                return 0;
            }
        };

#pragma pack(pop)


    private://websocket
        void ShakeHandsHandle(const char *buf, int buflen);

        bool FindHttpParam(const char *param, const char *buf);

    private:
        bool shake_hands_;//是否已经握手

    public:
        /// dwTimeOut .
        Session(uint32_t dwSendBufferSize, uint32_t dwRecvBufferSize, uint32_t dwMaxPacketSize, uint16_t maxHeadSize, uint32_t dwTimeOut
                , bool openMsgQueue, bool webSocket);

        virtual ~Session();

        bool IsWebSocket() { return m_webSocket; }

        void Init();

        bool Send(uint8_t *pMsg, uint16_t wSize);

        bool OnSend();

        int DoSend(IoHandler *pIoHandler);

        int DoRecv();

        int PreSend(IoHandler *pIoHandler);

        SOCKET CreateSocket();

        bool ProcessRecvdPacket();

        //处理消息
        bool HandleRecvMessage();

        //解码消息到消息队列
        bool DecodeMsgToQueue();

        //解码websocket消息到队列
        bool DecodeWebSocketToQueue();

        void BindNetworkObject(NetworkObject *pNetworkObject);

        void UnbindNetworkObject();

        void OnAccept();

        void OnConnect(bool bSuccess);

        inline void SetSocket(SOCKET socket) {
            m_socket = socket;
        }

        inline void SetSockAddr(SOCKADDR_IN &sockaddr) {
            m_sockaddr = sockaddr;
        }

        inline void CloseSocket() {
            SocketOpt::CloseSocket(m_socket);
            m_socket = INVALID_SOCKET;
        }

        inline NetworkObject *GetNetworkObject() {
            return m_pNetworkObject;
        }

        inline SendBuffer *GetSendBuffer() {
            return m_pSendBuffer;
        }                ///<
        inline RecvBuffer *GetRecvBuffer() {
            return m_pRecvBuffer;
        }                ///<
        inline SOCKET GetSocket() {
            return m_socket;
        }                    ///<
        inline SOCKADDR_IN *GetSockAddr() {
            return &m_sockaddr;
        }        ///<
        inline char *GetIP() {
            return inet_ntoa(m_sockaddr.sin_addr);
        }    ///<
        inline unsigned long GetIPNumber() {
            return m_sockaddr.sin_addr.s_addr;
        }

        inline uint32_t GetIdleTick() const {
            return m_dwTimeOut ? m_dwLastSyncTick + m_dwTimeOut : 0;
        }

        inline uint32_t GetLastSyncTick() {
            return m_dwLastSyncTick;
        }

        inline bool IsOnIdle() {
            return m_dwTimeOut ? GetIdleTick() < getNetWorkTime() : false;
        }

        inline bool IsAcceptSocket() {
            return m_bAcceptSocket;
        }

        inline void SetAcceptSocketFlag() {
            m_bAcceptSocket = true;
        }

        void Remove() {
            __sync_fetch_and_or(&m_bRemove, 1); /*m_bRemove = 1;*/ }

        inline void ResetKillFlag() {
            __sync_fetch_and_and(&m_bRemove, 0); /*m_bRemove = 0;*/ }

        inline int ShouldBeRemoved() {
            return m_bRemove;
        }

        void Disconnect(bool bGracefulDisconnect);

        inline bool HasDisconnectOrdered() {
            return m_bDisconnectOrdered;
        }

    private:
        inline void ResetTimeOut() {
            m_dwLastSyncTick = getNetWorkTime();
        }
        void SetWebSocket(bool webSocket);

        NetworkObject *m_pNetworkObject;
        SendBuffer *m_pSendBuffer;
        RecvBuffer *m_pRecvBuffer;

        SOCKET m_socket;
        SOCKADDR_IN m_sockaddr;

        uint32_t m_dwLastSyncTick;
        volatile int m_bRemove;
        uint32_t m_dwTimeOut;
        bool m_bAcceptSocket;
        bool m_bDisconnectOrdered;

        TLock m_lockRecv;
        TLock m_lockSend;
        int m_bCanSend;
        svrlib::LockedQueue<std::shared_ptr<MessageBuffer> > m_QueueMessage;//消息队列
        bool m_openMsgQueue;//是否开启消息队列模式
        bool m_webSocket;     // 是否websocket
        WebSocketHead ws_head_;//包头
        uint16_t m_wMaxPacketSize;

    };

}

	
 