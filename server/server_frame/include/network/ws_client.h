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

    enum WebSocketOpcode { //�����붨������
        OPCODE_MID = 0x0,//��ʶһ���м����ݰ�
        OPCODE_TXT = 0x1,//��ʶһ��text�������ݰ�
        OPCODE_BIN = 0x2,//��ʶһ��binary�������ݰ�
        //0x3 - 7������
                OPCODE_CLR = 0x8,//��ʶһ���Ͽ������������ݰ�
        OPCODE_PIN = 0x9,//��ʶһ��ping�������ݰ�
        OPCODE_PON = 0xA,//��ʾһ��pong�������ݰ�
    };

#pragma pack(push, 1)

    struct WebSocketHead {
        uint8_t fin : 1;//��ʶ�Ƿ�Ϊ����Ϣ�����һ�����ݰ�
        uint8_t rsv1 : 1;//����λ1
        uint8_t rsv2 : 1;//����λ2
        uint8_t rsv3 : 1;//����λ3
        uint8_t opcode : 4;//������

        uint8_t mask : 1; //�Ƿ���Ҫ����
        uint8_t len : 7;//����
        union {
            uint16_t v16;//����Ϊ126ʱ
            uint64_t v64;//����Ϊ127ʱ
        } ex_len;
        uint8_t mkey[4];
        uint8_t rh : 1;//head��ȡ���
        uint8_t rl : 1;//len��ȡ���
        uint8_t rk : 1;//mkey��ȡ���
        uint8_t rs : 5;//��չ����
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
        CS_CONNED,//������
        CS_COMMUNICATE,//ͨ��״̬
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
        bool shake_hands_;//�Ƿ��Ѿ�����
        WebSocketHead ws_head_;//��ͷ
        uint8_t conn_status_;//����״̬
        uint32_t skey_;//�����KEY


    };
};