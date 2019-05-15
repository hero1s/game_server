//
// Created by Administrator on 2018/8/8.
//

#pragma once

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/message.h>
#include "svrlib.h"
#include "network/NetworkObject.h"
#include "protobuf_handle.h"

using namespace Network;

#pragma  pack(1)
//内部数据包头------------------------------------------------------------------------------------------------------------
typedef struct inner_header_t {
    uint8_t route;          // 路由类型
    uint32_t routeID;       // 路由ID
    uint32_t uin;           // uin(服务器内部转发用)
    uint16_t cmd;           // 消息id
    uint16_t datalen;       // 消息数据长度(不包括包头)
} INNERHEAD;

#define INNER_MAX_SIZE             1024*32
#define INNER_HEADER_SIZE          sizeof(inner_header_t)
#define INNER_MAX_DATA_SIZE        (INNER_MAX_SIZE - INNER_HEADER_SIZE)

typedef struct {
    inner_header_t header;
    uint8_t protobuf[INNER_MAX_DATA_SIZE];
} inner_protobuf;

#pragma pack()

class pkg_inner {
public:
    static int GetPacketLen(const uint8_t *pData, uint16_t wLen) {
        if (pData == NULL) {
            return -1;
        }
        if (wLen < INNER_HEADER_SIZE) {
            return -1;
        }
        inner_header_t *head = (inner_header_t *) pData;
        if (head->datalen < INNER_MAX_DATA_SIZE) {
            return head->datalen + INNER_HEADER_SIZE;
        }
        LOG_ERROR("max packet len:{},uid:{},cmd:{}", head->datalen, head->uin, head->cmd);
        return -1;
    }

    static bool
    SendProtobufMsg(NetworkObject *pNetObj, const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin,
                 uint8_t route, uint32_t routeID) {
        if (pNetObj == NULL) {
            return false;
        }
        static string sstr;
        msg->SerializeToString(&sstr);
        return SendBuffMsg(pNetObj, sstr.c_str(), sstr.length(), msg_type, uin, route, routeID);
    }

    static bool
    SendBuffMsg(NetworkObject *pNetObj, const void *msg, uint16_t msg_len, uint16_t msg_type, uint32_t uin,
                     uint8_t route, uint32_t routeID) {
        if (pNetObj == NULL) {
            return false;
        }
        static inner_protobuf pkt;
        memset(&pkt, 0, sizeof(pkt));
        pkt.header.cmd = msg_type;
        pkt.header.uin = uin;
        pkt.header.route = route;
        pkt.header.routeID = routeID;

        if (msg_len >= INNER_MAX_DATA_SIZE) {
            LOG_ERROR("msg length more than max length:{}", msg_len);
            return false;
        }
        memcpy((void *) pkt.protobuf, msg, msg_len);
        pkt.header.datalen = msg_len;

        return pNetObj->Send((uint8_t *) &pkt.header, pkt.header.datalen + INNER_HEADER_SIZE);
    }
};

// 消息处理
class CInnerMsgHanlde : public CProtobufHandleBase<INNERHEAD> {
public:

    int OnHandleClientMsg(NetworkObject *pNetObj, uint8_t *pData, size_t uiDataLen) {
        if (pData == NULL)
            return -1;
        inner_header_t *head = (inner_header_t *) pData;
        if (head->datalen > (uiDataLen - INNER_HEADER_SIZE)) {
            LOG_ERROR("msg length is not right:{}--{}", uiDataLen, head->datalen);
            return -1;
        }
        _pNetObj = pNetObj;
        _head = head;
        _pkt_buf = pData + INNER_HEADER_SIZE;
        _buf_len = head->datalen;
        _cmd = head->cmd;

        return OnRecvClientMsg();
    }
};


