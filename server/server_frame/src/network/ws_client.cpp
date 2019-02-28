
#include "network/ws_client.h"
#include "crypt/base64.hpp"
#include "crypt/sha1.h"
#include <istream>
#include <iostream>
#include <arpa/inet.h>
#ifdef HAVE_NETINET_IN_H
#include <netinet/in.h>
#endif
using namespace svrlib;

namespace Network {

#define WEB_SOCKET_HANDS_RE "HTTP/1.1 101 Switching Protocols\r\nUpgrade: websocket\r\nConnection: Upgrade\r\nSec-WebSocket-Accept: %s\r\n\r\n"
#define MAGIC_KEY "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"

    CWebSocketClient::CWebSocketClient() {
        shake_hands_ = false;

    }

    CWebSocketClient::~CWebSocketClient() {

    }
    uint16_t CWebSocketClient::GetHeadLen(){
        if(shake_hands_)return 2;
        return 4;
    }
    uint16_t CWebSocketClient::GetPacketLen(const uint8_t* pData, uint16_t wLen){
        if(shake_hands_){
            // 检查扩展位并忽略
            if ((pData[0] & 0x70) != 0x0)
            {
                return -1;
            }
            // fin位: 为1表示已接收完整报文, 为0表示继续监听后续报文
            if ((pData[0] & 0x80) != 0x80)
            {
                return -1;
            }
            // mask位, 为1表示数据被加密
            if ((pData[1] & 0x80) != 0x80)
            {
                return -1;
            }

            // 操作码
            uint16_t payloadLength = 0;
            uint8_t payloadFieldExtraBytes = 0;
            uint8_t opcode = static_cast<uint8_t >(pData[0] & 0x0f);
            if (opcode == WS_TEXT_FRAME)
            {
                // 处理utf-8编码的文本帧
                payloadLength = static_cast<uint16_t >(pData[1] & 0x7f);
                if (payloadLength == 0x7e)
                {
                    uint16_t payloadLength16b = 0;
                    payloadFieldExtraBytes = 2;
                    memcpy(&payloadLength16b, &pData[2], payloadFieldExtraBytes);
                    payloadLength = ntohs(payloadLength16b);
                }
                else if (payloadLength == 0x7f)
                {
                    // 数据过长,暂不支持
                    return -1;
                }
                return 2+payloadFieldExtraBytes+payloadLength;
            }
            else if (opcode == WS_BINARY_FRAME || opcode == WS_PING_FRAME || opcode == WS_PONG_FRAME)
            {
                // 二进制/ping/pong帧暂不处理
                return 2+payloadFieldExtraBytes+payloadLength;
            }
            else if (opcode == WS_CLOSING_FRAME)
            {
                return -1;
            }
            else
            {
                return -1;
            }
        }else{//握手前的包长判断(待实现)

            return wLen;
        }
    }
    uint16_t CWebSocketClient::MaxTickPacket(){
        return 20;
    }

    void CWebSocketClient::OnAccept(uint32_t dwNetworkIndex) {

    }

    void CWebSocketClient::OnDisconnect() {

    }

    int CWebSocketClient::OnRecv(uint8_t *pMsg, uint16_t wSize) {
        if (shake_hands_) {
            //握手成功后处理消息

        } else {
            ShakeHandsHandle((const char*)pMsg, wSize);
        }
        return 0;
    }

    void CWebSocketClient::OnConnect(bool bSuccess, uint32_t dwNetworkIndex) {

    }

    void CWebSocketClient::ShakeHandsHandle(const char *buf, int buflen) {
        char key[512];
        memset(key, 0, 512);
        for (int i = 0; i < buflen; ++i) {
            if (FindHttpParam("Sec-WebSocket-Key", buf + i)) {
                short k = i + 17, ki = 0;
                while (*(buf + k) != '\r' && *(buf + k) != '\n') {
                    if (*(buf + k) == ':' || *(buf + k) == ' ') {
                        ++k;
                        continue;
                    } else {
                        key[ki++] = *(buf + k);
                    }
                    ++k;
                }
                break;
            }
        }
        //MSG_LOG("key:%s...", key);
        memcpy(key + strlen(key), MAGIC_KEY, sizeof(MAGIC_KEY));
        //MSG_LOG("megerkey:%s...", key);
        //求哈希1
        SHA1 sha;
        unsigned int message_digest[5];
        sha.Reset();
        sha << key;
        sha.Result(message_digest);
        for (int i = 0; i < 5; i++) {
            message_digest[i] = htonl(message_digest[i]);
        }

        memset(key, 0, 512);
        base64::encode(key, reinterpret_cast<const char *>(message_digest), 20);
        char http_res[512] = "";
        sprintf(http_res, WEB_SOCKET_HANDS_RE, key);
        Send((uint8_t*)http_res, strlen(http_res));
        //MSG_LOG("res:%s...",http_res);//fkYTdNEVkParesYkrM4S
        shake_hands_ = true;
    }
    bool CWebSocketClient::FindHttpParam(const char * param, const char * buf) {
        while (*param == *buf) {
            if (*(param + 1) == '\0') return true;
            ++param; ++buf;
        }
        return false;
    }
    int CWebSocketClient::wsDecodeFrame(std::string inFrame, std::string &outMessage)
    {
        int ret = WS_OPENING_FRAME;
        const char *frameData = inFrame.c_str();
        const int frameLength = inFrame.size();
        if (frameLength < 2)
        {
            ret = WS_ERROR_FRAME;
        }

        // 检查扩展位并忽略
        if ((frameData[0] & 0x70) != 0x0)
        {
            ret = WS_ERROR_FRAME;
        }

        // fin位: 为1表示已接收完整报文, 为0表示继续监听后续报文
        ret = (frameData[0] & 0x80);
        if ((frameData[0] & 0x80) != 0x80)
        {
            ret = WS_ERROR_FRAME;
        }

        // mask位, 为1表示数据被加密
        if ((frameData[1] & 0x80) != 0x80)
        {
            ret = WS_ERROR_FRAME;
        }

        // 操作码
        uint16_t payloadLength = 0;
        uint8_t payloadFieldExtraBytes = 0;
        uint8_t opcode = static_cast<uint8_t >(frameData[0] & 0x0f);
        if (opcode == WS_TEXT_FRAME)
        {
            // 处理utf-8编码的文本帧
            payloadLength = static_cast<uint16_t >(frameData[1] & 0x7f);
            if (payloadLength == 0x7e)
            {
                uint16_t payloadLength16b = 0;
                payloadFieldExtraBytes = 2;
                memcpy(&payloadLength16b, &frameData[2], payloadFieldExtraBytes);
                payloadLength = ntohs(payloadLength16b);
            }
            else if (payloadLength == 0x7f)
            {
                // 数据过长,暂不支持
                ret = WS_ERROR_FRAME;
            }
        }
        else if (opcode == WS_BINARY_FRAME || opcode == WS_PING_FRAME || opcode == WS_PONG_FRAME)
        {
            // 二进制/ping/pong帧暂不处理
        }
        else if (opcode == WS_CLOSING_FRAME)
        {
            ret = WS_CLOSING_FRAME;
        }
        else
        {
            ret = WS_ERROR_FRAME;
        }

        // 数据解码
        if ((ret != WS_ERROR_FRAME) && (payloadLength > 0))
        {
            // header: 2字节, masking key: 4字节
            const char *maskingKey = &frameData[2 + payloadFieldExtraBytes];
            char *payloadData = new char[payloadLength + 1];
            memset(payloadData, 0, payloadLength + 1);
            memcpy(payloadData, &frameData[2 + payloadFieldExtraBytes + 4], payloadLength);
            for (int i = 0; i < payloadLength; i++)
            {
                payloadData[i] = payloadData[i] ^ maskingKey[i % 4];
            }

            outMessage = payloadData;
            delete[] payloadData;
        }

        return ret;
    }
    int CWebSocketClient::wsEncodeFrame(std::string inMessage, std::string &outFrame, enum WS_FrameType frameType)
    {
        int ret = WS_EMPTY_FRAME;
        const uint32_t messageLength = inMessage.size();
        if (messageLength > 32767)
        {
            // 暂不支持这么长的数据
            return WS_ERROR_FRAME;
        }

        uint8_t payloadFieldExtraBytes = (messageLength <= 0x7d) ? 0 : 2;
        // header: 2字节, mask位设置为0(不加密), 则后面的masking key无须填写, 省略4字节
        uint8_t frameHeaderSize = 2 + payloadFieldExtraBytes;
        uint8_t *frameHeader = new uint8_t[frameHeaderSize];
        memset(frameHeader, 0, frameHeaderSize);
        // fin位为1, 扩展位为0, 操作位为frameType
        frameHeader[0] = static_cast<uint8_t>(0x80 | frameType);

        // 填充数据长度
        if (messageLength <= 0x7d)
        {
            frameHeader[1] = static_cast<uint8_t>(messageLength);
        }
        else
        {
            frameHeader[1] = 0x7e;
            uint16_t len = htons(messageLength);
            memcpy(&frameHeader[2], &len, payloadFieldExtraBytes);
        }

        // 填充数据
        uint32_t frameSize = frameHeaderSize + messageLength;
        char *frame = new char[frameSize + 1];
        memcpy(frame, frameHeader, frameHeaderSize);
        memcpy(frame + frameHeaderSize, inMessage.c_str(), messageLength);
        frame[frameSize] = '\0';
        outFrame = frame;

        delete[] frame;
        delete[] frameHeader;
        return ret;
    }


};