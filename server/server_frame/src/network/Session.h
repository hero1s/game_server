
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

        enum WebSocketOpcode { //�����붨������
            OPCODE_MID = 0x0,//��ʶһ���м����ݰ�
            OPCODE_TXT = 0x1,//��ʶһ��text�������ݰ�
            OPCODE_BIN = 0x2,//��ʶһ��binary�������ݰ�
            //0x3 - 7������
                    OPCODE_CLR = 0x8,//��ʶһ���Ͽ������������ݰ�
            OPCODE_PIN = 0x9,//��ʶһ��ping�������ݰ�
            OPCODE_PON = 0xA,//��ʾһ��pong�������ݰ�
        };

#pragma pack(push, 1)

        struct WebSocketHead {
            uint8_t fin : 1;//��ʶ�Ƿ�Ϊ����Ϣ�����һ�����ݰ�
            uint8_t rsv1 : 1;//����λ1
            uint8_t rsv2 : 1;//����λ2
            uint8_t rsv3 : 1;//����λ3
            uint8_t opcode : 4;//������

            uint8_t mask : 1; //�Ƿ���Ҫ����
            uint8_t len : 7;//����
            union {
                uint16_t v16;//����Ϊ126ʱ
                uint64_t v64;//����Ϊ127ʱ
            } ex_len;
            uint8_t mkey[4];
            uint8_t rh : 1;//head��ȡ���
            uint8_t rl : 1;//len��ȡ���
            uint8_t rk : 1;//mkey��ȡ���
            uint8_t rs : 5;//��չ����
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
        bool shake_hands_;//�Ƿ��Ѿ�����

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

        //������Ϣ
        bool HandleRecvMessage();

        //������Ϣ����Ϣ����
        bool DecodeMsgToQueue();

        //����websocket��Ϣ������
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
        svrlib::LockedQueue<std::shared_ptr<MessageBuffer> > m_QueueMessage;//��Ϣ����
        bool m_openMsgQueue;//�Ƿ�����Ϣ����ģʽ
        bool m_webSocket;     // �Ƿ�websocket
        WebSocketHead ws_head_;//��ͷ
        uint16_t m_wMaxPacketSize;

    };

}

	
 