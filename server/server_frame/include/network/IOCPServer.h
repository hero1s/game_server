
#pragma once

#include <unordered_map>
#include <pthread.h>
#include <string.h>
#include "unistd.h"
#include <functional>
#include "NetworkObject.h"

namespace Network {
    class IoHandler;

    using AcceptAllocFunc = std::function<NetworkObject *()>;
    using AcceptFreeFunc = std::function<void(NetworkObject *)>;

    struct stIOHANDLER_DESC {
        uint32_t ioHandlerKey;
        uint32_t maxAcceptSession;      // ������������
        uint32_t maxConnectBuffSize;    // ��������BuffSize
        uint32_t sendBufferSize;        // ���ͻ���
        uint32_t recvBufferSize;        // ���ܻ���
        uint32_t timeOut;               // ��ʱ�Ͽ�(��)
        uint16_t maxPacketSize;         // ������
        uint16_t maxHeadSize;           // ����ͷ����
        AcceptAllocFunc allocFunc;
        AcceptFreeFunc freeFunc;
        bool webSocket;                 // �Ƿ�websocket

        stIOHANDLER_DESC() {
            memset(this, 0, sizeof(stIOHANDLER_DESC));
            maxAcceptSession = 50000;
            maxHeadSize = 32;
            allocFunc = nullptr;
            freeFunc = [](NetworkObject *p) { delete p; };
            webSocket = false;
        }
    };

    class IOCPServer {
        friend void *send_thread(void *param);

    public:
        IOCPServer();

        virtual ~IOCPServer();

        bool AddIoHandler(stIOHANDLER_DESC &lpDesc);

        bool StartListen(uint32_t dwIoHandlerKey, const char *pIP, uint16_t wPort);

        void Update();

        void Shutdown();

        uint32_t Connect(uint32_t dwIoHandlerKey, NetworkObject *pNetworkObject, const char *pszIP, uint16_t wPort);

        int IsListening(uint32_t dwIoHandlerKey);

        uint32_t GetNumberOfConnections(uint32_t dwIoHandlerKey);

        //������
        void AddWhiteListIP(uint32_t dwIoHandlerKey, uint32_t ip);

        void ClearWhiteListIP(uint32_t dwIoHandlerKey);

        bool IsWhiteIP(uint32_t dwIoHandlerKey, uint32_t ip);

    private:
        void CreateIoHandler(stIOHANDLER_DESC &lpDesc);

        void CreateSendThread();

        pthread_t m_hSendThread;
        int m_bShutdown;
        std::unordered_map<uint32_t, IoHandler *> m_mapIoHandlers;
    };

    uint32_t getNetWorkTime();

}


