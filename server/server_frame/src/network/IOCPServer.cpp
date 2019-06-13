#include "Network.h"
#include "TLock.h"
#include "network/IOCPServer.h"
#include "IoHandler.h"
#include "SessionList.h"
#include "svrlib.h"

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
    void IOCPServer::Update() {
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

    uint32_t getNetWorkTime() { return getSysTime(); }


}

