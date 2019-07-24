
#pragma once

namespace Network {

    class Session;

    class SessionList;

    class SessionPool {
    public:
        SessionPool(uint32_t dwSendBufferSize, uint32_t dwRecvBufferSize, uint32_t dwMaxPacketSize,uint16_t maxHeadSize,
                    uint32_t dwTimeOutTick, bool bAcceptSocket,bool bWebSocket);

        ~SessionPool();

        Session *Alloc();

        void Free(Session *pSession);

    private:
        void Create();
        bool NeedFree();

        uint32_t m_dwSendBufferSize;
        uint32_t m_dwRecvBufferSize;
        uint32_t m_dwMaxPacketSize;
        uint16_t m_maxHeadSize;
        uint32_t m_dwTimeOutTick;
        bool m_bAcceptSocket;
        uint32_t m_dwCurSize;

        bool  m_webSocket;	 // ÊÇ·ñwebsocket

        SessionList *m_pList;
    };

}


	
