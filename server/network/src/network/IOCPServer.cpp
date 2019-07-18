#include "Network.h"
#include "TLock.h"
#include "network/IOCPServer.h"
#include "IoHandler.h"
#include "SessionList.h"
#include "utility/comm_macro.h"
#include <time.h>

namespace Network {
//=============================================================================================================================
/**
	@remarks
	@param	param
			IOCP 
*/
//=============================================================================================================================
    void *send_thread(void *param) {
        IOCPServer *pIOCPServer = (IOCPServer *) param;
        while (!pIOCPServer->m_bShutdown)
        {
            usleep(1000 * 5);//5ms
            for (auto &it : pIOCPServer->m_mapIoHandlers)
            {
                it.second->ProcessSend();
            }
        }

        pthread_exit(0);

    }

    IOCPServer::IOCPServer() {
        m_bShutdown = FALSE;
        m_hSendThread = 0;
        m_lastTick = 0;
        m_wheelTime = 0;
        m_wheelPrecision = 10;
    }

    IOCPServer::~IOCPServer() {
        if (!m_bShutdown) Shutdown();
    }

    bool IOCPServer::AddIoHandler(stIOHANDLER_DESC &lpDesc) {
        if (m_mapIoHandlers.find(lpDesc.ioHandlerKey) != m_mapIoHandlers.end())
        {
            LOG_ERROR("error io key is exist");
            return false;
        }
        CreateIoHandler(lpDesc);
        CreateSendThread();
        return true;
    }

    void IOCPServer::CreateIoHandler(stIOHANDLER_DESC &lpDesc) {
        IoHandler *pIoHandler = new IoHandler;

        pIoHandler->Init(this, lpDesc);

        m_mapIoHandlers.insert(make_pair(pIoHandler->GetKey(), pIoHandler));
    }

    void IOCPServer::CreateSendThread() {
        if (m_hSendThread == 0)
        {
            pthread_create(&m_hSendThread, NULL, send_thread, (void *) this);
        }
    }

    void IOCPServer::Shutdown() {
        m_bShutdown = TRUE;

        // Send .
        pthread_cancel(m_hSendThread);
        pthread_join(m_hSendThread, NULL);

        // IoHandler.
        IoHandler *pIoHandler;

        for (auto &it : m_mapIoHandlers)
        {
            pIoHandler = it.second;
            pIoHandler->Shutdown();
            delete pIoHandler;
        }
        m_mapIoHandlers.clear();
    }

//=============================================================================================================================
/**
	@remarks
			Accept Connect
*/
//=============================================================================================================================
    void IOCPServer::Update(uint64_t curTime) {
        getNetWorkTime(true);
        if (m_lastTick == 0) {
            m_lastTick = curTime;
        }
        int64_t delta = curTime - m_lastTick;
        if (delta > 0) {
            m_wheelTime += delta;
            m_timers.advance(m_wheelTime/m_wheelPrecision);
            m_wheelTime = m_wheelTime%m_wheelPrecision;
            m_lastTick = curTime;
        }

        for (auto &it : m_mapIoHandlers)
        {
            it.second->Update();
        }
    }

    bool IOCPServer::StartListen(uint32_t dwIoHandlerKey, const char *pIP, uint16_t wPort) {
        auto it = m_mapIoHandlers.find(dwIoHandlerKey);
        if (it == m_mapIoHandlers.end())
        {
            LOG_ERROR("iohandler is null :{}", dwIoHandlerKey);
            return false;
        }
        return it->second->StartListen(pIP, wPort);
    }

//=============================================================================================================================
/**
	@remarks
			- connect 
	@param	dwIoHandlerKey
			- OnConnect( int )
	@param	szIP
	@param	wPort
	@retval	uint32_t
*/
//=============================================================================================================================
    uint32_t IOCPServer::Connect(uint32_t dwIoHandlerKey, NetworkObject *pNetworkObject, const char *pszIP,
                                 uint16_t wPort) {
        if (pNetworkObject == NULL) return 0;

        auto it = m_mapIoHandlers.find(dwIoHandlerKey);
        if (it == m_mapIoHandlers.end())
        {
            LOG_ERROR("iohandler is null :{}", dwIoHandlerKey);
            return 0;
        }
        return it->second->Connect(pNetworkObject, pszIP, wPort);
    }

    int IOCPServer::IsListening(uint32_t dwIoHandlerKey) {
        auto it = m_mapIoHandlers.find(dwIoHandlerKey);
        if (it == m_mapIoHandlers.end())
        {
            LOG_ERROR("iohandler is null :{}", dwIoHandlerKey);
            return 0;
        }
        return it->second->IsListening();
    }

    uint32_t IOCPServer::GetNumberOfConnections(uint32_t dwIoHandlerKey) {
        auto it = m_mapIoHandlers.find(dwIoHandlerKey);
        if (it == m_mapIoHandlers.end())
        {
            LOG_ERROR("iohandler is null :{}", dwIoHandlerKey);
            return 0;
        }
        return it->second->GetNumberOfConnections();
    }

    void IOCPServer::AddWhiteListIP(uint32_t dwIoHandlerKey, uint32_t ip) {
        auto it = m_mapIoHandlers.find(dwIoHandlerKey);
        if (it == m_mapIoHandlers.end())
        {
            LOG_ERROR("iohandler is null :{}", dwIoHandlerKey);
            return;
        }
        return it->second->AddWhiteListIP(ip);
    }

    void IOCPServer::ClearWhiteListIP(uint32_t dwIoHandlerKey) {
        auto it = m_mapIoHandlers.find(dwIoHandlerKey);
        if (it == m_mapIoHandlers.end())
        {
            LOG_ERROR("iohandler is null :{}", dwIoHandlerKey);
            return;
        }
        return it->second->ClearWhiteListIP();
    }

    bool IOCPServer::IsWhiteIP(uint32_t dwIoHandlerKey, uint32_t ip) {
        auto it = m_mapIoHandlers.find(dwIoHandlerKey);
        if (it == m_mapIoHandlers.end())
        {
            LOG_ERROR("iohandler is null :{}", dwIoHandlerKey);
            return false;
        }
        return it->second->IsWhiteIP(ip);
    }

    void IOCPServer::schedule(TimerEventInterface *event, uint64_t delta) {
        m_timers.schedule(event, delta/m_wheelPrecision);
    }

    void IOCPServer::schedule_in_range(TimerEventInterface *event, uint64_t start, uint64_t end) {
        m_timers.schedule_in_range(event, start/m_wheelPrecision, end/m_wheelPrecision);
    }

    uint32_t getNetWorkTime(bool flush) {
        static uint32_t s_cur_time = 0;
        if(!s_cur_time || flush)
            s_cur_time = ::time(NULL);
        return s_cur_time;
    }


}

