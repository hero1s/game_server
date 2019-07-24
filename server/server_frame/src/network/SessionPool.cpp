#include "Network.h"
#include "TLock.h"
#include "SessionPool.h"
#include "Session.h"
#include "SessionList.h"

#include <algorithm>

namespace Network {
    SessionPool::SessionPool(uint32_t dwSendBufferSize,
            uint32_t dwRecvBufferSize,
            uint32_t dwMaxPacketSize,
            uint16_t maxHeadSize,
            uint32_t dwTimeOutTick,
            bool bAcceptSocket,
            bool bWebSocket)
    {
        m_pList = new SessionList;

        m_dwSendBufferSize = dwSendBufferSize;
        m_dwRecvBufferSize = dwRecvBufferSize;
        m_dwMaxPacketSize = dwMaxPacketSize;
        m_maxHeadSize = maxHeadSize;
        m_dwTimeOutTick = dwTimeOutTick;
        m_bAcceptSocket = bAcceptSocket;
        m_dwCurSize = 0;
        m_webSocket = bWebSocket;

        Create();
    }

    SessionPool::~SessionPool()
    {
        if (m_pList) delete m_pList;
    }

    void SessionPool::Create()
    {
        ++m_dwCurSize;
        auto pSession = new Session(m_dwSendBufferSize, m_dwRecvBufferSize, m_dwMaxPacketSize, m_maxHeadSize,
                m_dwTimeOutTick, m_webSocket);
        if (m_bAcceptSocket) {
            pSession->SetAcceptSocketFlag();
        }
        m_pList->push_back(pSession);
    }

    bool SessionPool::NeedFree()
    {
        if (m_pList->size()>(m_dwCurSize/10))return true;

        return false;
    }

    Session* SessionPool::Alloc()
    {
        m_pList->Lock();
        if (m_pList->empty()) {
            Create();
            if (m_pList->empty()) {
                m_pList->Unlock();
                return NULL;
            }
        }

        Session* pSession = m_pList->front();
        if (NULL!=pSession) {
            pSession->Init();
        }
        m_pList->pop_front();

        m_pList->Unlock();

        return pSession;
    }

    void SessionPool::Free(Session* pSession)
    {
        m_pList->Lock();
        if (NeedFree()) {
            delete pSession;
            m_dwCurSize--;
        }
        else {
            m_pList->push_back(pSession);
        }
        m_pList->Unlock();
    }


}
	
