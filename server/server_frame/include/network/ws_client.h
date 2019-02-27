//
// Created by toney on 2019/2/27.
//
#pragma once

#include "NetworkObject.h"
#include "IOCPServer.h"
#include <string>
#include <stdint.h>

namespace Network {
    enum WS_FrameType {
        WS_EMPTY_FRAME = 0xF0,
        WS_ERROR_FRAME = 0xF1,
        WS_TEXT_FRAME = 0x01,
        WS_BINARY_FRAME = 0x02,
        WS_PING_FRAME = 0x09,
        WS_PONG_FRAME = 0x0A,
        WS_OPENING_FRAME = 0xF3,
        WS_CLOSING_FRAME = 0x08
    };

    class CWebSocketClient : public NetworkObject {
    public:
        CWebSocketClient();

        virtual ~CWebSocketClient();

        virtual uint16_t GetHeadLen();
        virtual uint16_t GetPacketLen(const uint8_t* pData, uint16_t wLen);
        virtual uint16_t MaxTickPacket();
    protected:
        virtual void OnAccept(uint32_t dwNetworkIndex);

        virtual void OnDisconnect();

        virtual int OnRecv(uint8_t *pMsg, uint16_t wSize);

        virtual void OnConnect(bool bSuccess, uint32_t dwNetworkIndex);

    protected:
        void ShakeHandsHandle(const char *buf, int buflen);

        bool FindHttpParam(const char *param, const char *buf);

        int wsDecodeFrame(std::string inFrame, std::string &outMessage);

        int wsEncodeFrame(std::string inMessage, std::string &outFrame, enum WS_FrameType frameType);

    private:
        bool shake_hands_;//是否已经握手

    };
};