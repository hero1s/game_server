
#pragma once

namespace Network {

    class Session;

    class SessionList;

    class SessionPool {
    public:
        SessionPool(uint32_t dwSize, uint32_t dwSendBufferSize, uint32_t dwRecvBufferSize, uint32_t dwMaxPacketSize,
                    uint32_t dwTimeOutTick, uint32_t dwIndexStart, bool bAcceptSocket,bool bOpenMsgQueue,bool bWebSocket);

        ~SessionPool();

        Session *Alloc();

        void Free(Session *pSession);

        int GetMaxSize();

    private:
        void Create();

        uint32_t m_dwMaxSize;
        uint32_t m_dwSendBufferSize;
        uint32_t m_dwRecvBufferSize;
        uint32_t m_dwMaxPacketSize;
        uint32_t m_dwTimeOutTick;
        uint32_t m_dwIndexStart;
        bool m_bAcceptSocket;
        uint32_t m_dwCurSize;

        bool  m_openMsgQueue;//是否开启消息队列模式
        bool  m_webSocket;	 // 是否websocket

        SessionList *m_pList;
    };

}


	
