
#include "network/ws_client.h"

namespace Network {

    CWebSocketClient::CWebSocketClient() {

    }

    CWebSocketClient::~CWebSocketClient() {

    }

    void CWebSocketClient::OnAccept(uint32_t dwNetworkIndex) {

    }

    void CWebSocketClient::OnDisconnect() {

    }

    int CWebSocketClient::OnRecv(uint8_t *pMsg, uint16_t wSize) {
        return 0;
    }

    void CWebSocketClient::OnConnect(bool bSuccess, uint32_t dwNetworkIndex) {

    }

    void CWebSocketClient::ShakeHandsHandle(const char *buf, int buflen) {

    }
};