//
// Created by Administrator on 2018/7/4.
//

#pragma once

#include "NetworkObject.h"
#include "IOCPServer.h"
#include "asio.hpp"
#include <string>


namespace Network {

    class CTcpConnector : public NetworkObject {
        enum {
            emNet_NULL = 0,// 空状态
            emNet_CONNECTED = 1,// 连接状态
            emNet_DISCONNECT = 2,// 断开状态
            emNet_CONNECTING = 3,// 连接中
        };

    public:
        CTcpConnector();

        virtual ~CTcpConnector();

        bool Init(asio::io_context &context,IOCPServer *pServer, uint32_t ioKey, std::string ip, uint32_t port);

        void ShutDown();

        virtual void ConnectorOnDisconnect() = 0;

        virtual void ConnectorOnConnect(bool bSuccess) = 0;

    protected:
        void OnTimer(const std::error_code &err);

        void ReConnect();

        virtual void OnDisconnect() final;

        virtual void OnConnect(bool bSuccess) final;

    private:
        std::shared_ptr<asio::system_timer> m_pTimer = nullptr;
        uint8_t m_netState;
        IOCPServer *m_pServer;
        uint32_t m_ioKey;
        char m_szRemoteIp[128];
        uint32_t m_RemotePort;

    };
};


