//
// Created by Administrator on 2018/8/8.
//

#pragma once

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/message.h>
#include "svrlib.h"
#include "network/NetworkObject.h"

using namespace svrlib;
using namespace Network;

#pragma  pack(1)
//�ڲ����ݰ�ͷ------------------------------------------------------------------------------------------------------------
typedef struct inner_header_t {
    uint8_t route;         // ·������
    uint32_t routeMain;     // ��·��
    uint32_t routeSub;      // ��·��
    uint32_t uin;           // uin(�������ڲ�ת����)
    uint16_t cmd;           // ��Ϣid
    uint16_t datalen;       // ��Ϣ���ݳ���(��������ͷ)
} INNERHEAD;

#define INNER_MAX_SIZE             1024*32
#define INNER_HEADER_SIZE          sizeof(inner_header_t)
#define INNER_MAX_DATA_SIZE        (INNER_MAX_SIZE - INNER_HEADER_SIZE)

typedef struct {
    inner_header_t header;
    uint8_t protobuf[INNER_MAX_DATA_SIZE];
} inner_protobuf;

#pragma pack()

int GetInnerPacketLen(const uint8_t *pData, uint16_t wLen);

bool SendInnerMsg(NetworkObject *pNetObj, const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin,
                  uint8_t route = 0, uint32_t routeMain = 0, uint32_t routeSub = 0);

bool SendInnerMsg(NetworkObject *pNetObj, const void *msg, uint16_t msg_len, uint16_t msg_type, uint32_t uin,
                  uint8_t route = 0, uint32_t routeMain = 0, uint32_t routeSub = 0);

// ��Ϣ����
class CInnerMsgHanlde {
public:
    int OnHandleClientMsg(NetworkObject *pNetObj, uint8_t *pData, size_t uiDataLen);

    //  �յ��ͻ�����Ϣʱ�ص�
    virtual int OnRecvClientMsg(NetworkObject *pNetObj, const uint8_t *pkt_buf, uint16_t buf_len, INNERHEAD *head) = 0;

    //  �����Ϣӳ��
    template<class TYPE, class F>
    void bind_handler(TYPE *m, int key, F f) {
        m_handlers[key] = std::bind(f, m, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                                    std::placeholders::_4);
    }

protected:
    std::unordered_map<int, function<int(NetworkObject *pNetObj, const uint8_t *pkt_buf, uint16_t buf_len,
                                         INNERHEAD *head)>> m_handlers;
};

// ��Ϣ����
class CInnerMsgDecode : public CMessageDecode {
public:
    virtual uint32_t GetHeadLen() {
        return INNER_HEADER_SIZE;
    };

    virtual uint32_t GetPacketLen(const uint8_t *pData, uint16_t wLen) {
        return GetInnerPacketLen(pData, wLen);
    };

    virtual uint32_t MaxTickPacket() {
        return 1000;
    }
};

