//
// Created by toney on 2019/2/27.
//
#pragma once

#include "NetworkObject.h"
#include "IOCPServer.h"
#include <string>
#include <stdint.h>

namespace Network {

/// websocket
#define SEND_PACKET_MAX 1024*1024*8
#define DEFAULT_TAG 0xCA0FFFFF

    enum WebSocketOpcode { //操作码定义类型
        OPCODE_MID = 0x0,//标识一个中间数据包
        OPCODE_TXT = 0x1,//标识一个text类型数据包
        OPCODE_BIN = 0x2,//标识一个binary类型数据包
        //0x3 - 7：保留
                OPCODE_CLR = 0x8,//标识一个断开连接类型数据包
        OPCODE_PIN = 0x9,//标识一个ping类型数据包
        OPCODE_PON = 0xA,//表示一个pong类型数据包
    };

#pragma pack(push, 1)

    struct WebSocketHead {
        uint8_t fin : 1;//标识是否为此消息的最后一个数据包
        uint8_t rsv1 : 1;//保留位1
        uint8_t rsv2 : 1;//保留位2
        uint8_t rsv3 : 1;//保留位3
        uint8_t opcode : 4;//操作码

        uint8_t mask : 1; //是否需要掩码
        uint8_t len : 7;//长度
        union {
            uint16_t v16;//长度为126时
            uint64_t v64;//长度为127时
        } ex_len;
        uint8_t mkey[4];
        uint8_t rh : 1;//head读取完成
        uint8_t rl : 1;//len读取完成
        uint8_t rk : 1;//mkey读取完成
        uint8_t rs : 5;//扩展保留
        WebSocketHead(void) { reset(); }

        void reset(void) { memset(this, 0, sizeof(WebSocketHead)); }

        inline uint64_t GetLen(void) {
            if (len == 126) {
                return ex_len.v16;
            } else if (len == 127) {
                return ex_len.v64;
            }
            return len;
        }

        inline uint8_t GetLenNeedByte(void) {
            if (len == 126) {
                return 2;
            } else if (len == 127) {
                return 8;
            }
            return 0;
        }
    };

#pragma pack(pop)

    enum ConnStatusDef {
        CS_CONNED,//连接上
        CS_COMMUNICATE,//通信状态
    };

    class CWebSocketClient : public NetworkObject {
    public:
        CWebSocketClient();

        virtual ~CWebSocketClient();

    protected:
        virtual void OnAccept(uint32_t dwNetworkIndex);

        virtual void OnDisconnect();

        virtual int OnRecv(uint8_t *pMsg, uint16_t wSize);

        virtual void OnConnect(bool bSuccess, uint32_t dwNetworkIndex);

        void ShakeHandsHandle(const char* buf, int buflen);
    private:
        bool shake_hands_;//是否已经握手
        WebSocketHead ws_head_;//包头
        uint8_t conn_status_;//连接状态
        uint32_t skey_;//服务端KEY


    };
};