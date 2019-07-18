//
// Created by Administrator on 2018/7/4.
//

#pragma once

#include "NetworkObject.h"
#include "IOCPServer.h"
#include <string>

namespace Network {

    class CTcpConnector : public NetworkObject {
        enum {
            emNet_NULL = 0,// ��״̬
            emNet_CONNECTED = 1,// ����״̬
            emNet_DISCONNECT = 2,// �Ͽ�״̬
            emNet_CONNECTING = 3,// ������
        };

    public:
        CTcpConnector();

        virtual ~CTcpConnector();

        bool Init(IOCPServer *pServer, uint32_t ioKey, std::string ip, uint32_t port);

        void ShutDown();

        virtual void ConnectorOnDisconnect() = 0;

        virtual void ConnectorOnConnect(bool bSuccess) = 0;

    protected:
        void OnTimer();

        void ReConnect();

        virtual void OnDisconnect() final;

        virtual void OnConnect(bool bSuccess) final;

    private:
        MemberTimerEvent<CTcpConnector, &CTcpConnector::OnTimer> m_timer;
        uint8_t m_netState;
        IOCPServer *m_pServer;
        uint32_t m_ioKey;
        char m_szRemoteIp[128];
        uint32_t m_RemotePort;

    };
};


