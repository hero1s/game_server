
#pragma once

#include<string>
#include <any>

namespace Network {

    class Session;

    class NetworkObject {
        friend class Session;

        friend class IoHandler;

    public:
        NetworkObject();

        virtual ~NetworkObject();

        //true 立即关闭 false 发送完关闭
        void Disconnect(bool bGracefulDisconnect = true);

        bool Send(uint8_t *pMsg, uint16_t wSize);

        void Redirect(NetworkObject *pNetworkObject);

        std::string GetIP();

        uint32_t GetIPNumber();

        uint32_t GetUID();

        void SetUID(uint32_t uid);

        const std::any &GetContext() const { return context; }

        void SetContext(const std::any &context) { this->context = context; }

        inline uint16_t MaxTickPacket() { return m_maxTickPacket; };
    public:
        virtual uint16_t GetHeadLen() = 0;

        virtual uint16_t GetPacketLen(const uint8_t *pData, uint16_t wLen) = 0;

    protected:
        virtual void OnDisconnect();

        virtual int OnRecv(uint8_t *pMsg, uint16_t wSize);

        virtual void OnConnect(bool bSuccess);

        void SetSession(Session *pSession);

        Session *m_pSession;
        std::string m_strIP;
        uint32_t m_ipNumber;
        uint32_t m_uid;
        uint16_t m_maxTickPacket;
        std::any context;
    };
}


	
	
