
#pragma once

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/message.h>
#include "svrlib.h"
#include "network/NetworkObject.h"
#include "zlib.h"
#include "network/IOCPServer.h"
#include "protobuf_handle.h"

using namespace Network;


#pragma  pack(1)
//对外数据包头
typedef struct packet_header_t {
    uint32_t uin;           // uin(服务器内部转发用)
    uint16_t cmd;           // 消息id
    uint16_t datalen;       // 消息数据长度(不包括包头)
} PACKETHEAD;

#define PACKET_MAX_SIZE             1024*8
#define PACKET_HEADER_SIZE          sizeof(packet_header_t)
#define PACKET_MAX_DATA_SIZE        (PACKET_MAX_SIZE - PACKET_HEADER_SIZE)

typedef struct {
    packet_header_t header;
    uint8_t protobuf[PACKET_MAX_DATA_SIZE];
} packet_protobuf;
#pragma pack()

class pkg_client {
public:
    static int GetProtobufPacketLen(const uint8_t *pData, uint16_t wLen) {
        if (pData == NULL) {
            return -1;
        }
        if (wLen < PACKET_HEADER_SIZE) {
            return -1;
        }
        packet_header_t *head = (packet_header_t *) pData;
        if (head->datalen < PACKET_MAX_DATA_SIZE) {
            return head->datalen + PACKET_HEADER_SIZE;
        }
        LOG_ERROR("max packet len:{},uid:{},cmd:{}", head->datalen, head->uin, head->cmd);
        return -1;
    }

    static bool
    SendProtobufMsg(NetworkObject *pNetObj, const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin) {
        if (pNetObj == NULL) {
            return false;
        }
        static string sstr;
        msg->SerializeToString(&sstr);
        return SendBuffMsg(pNetObj, sstr.c_str(), sstr.length(), msg_type, uin);
    }

    static bool
    SendBuffMsg(NetworkObject *pNetObj, const void *msg, uint16_t msg_len, uint16_t msg_type, uint32_t uin) {
        if (pNetObj == NULL) {
            return false;
        }
        static packet_protobuf pkt;
        memset(&pkt, 0, sizeof(pkt));
        pkt.header.cmd = msg_type;
        pkt.header.uin = uin;

        if (msg_len >= PACKET_MAX_DATA_SIZE) {
            LOG_ERROR("msg length more than max length:{}", msg_len);
            return false;
        }
        memcpy((void *) pkt.protobuf, msg, msg_len);
        pkt.header.datalen = msg_len;

        //LOG_DEBUG("Socket Send Msg To Client-cmd:%d--len:%d",pkt.header.cmd,pkt.header.datalen);
        return pNetObj->Send((uint8_t *) &pkt.header, pkt.header.datalen + PACKET_HEADER_SIZE);
    }
};

// 消息处理
class CProtobufMsgHanlde : public CProtobufHandleBase<PACKETHEAD> {
public:
    int OnHandleClientMsg(NetworkObject *pNetObj, uint8_t *pData, size_t uiDataLen) {
        if (pData == NULL)
            return -1;
        packet_header_t *head = (packet_header_t *) pData;
        if (head->datalen > (uiDataLen - PACKET_HEADER_SIZE)) {
            LOG_ERROR("msg length is not right:{}--{}", uiDataLen, head->datalen);
            return -1;
        }
        _pNetObj = pNetObj;
        _head = head;
        _pkt_buf = pData + PACKET_HEADER_SIZE;
        _buf_len = head->datalen;
        _cmd = head->cmd;

        return OnRecvClientMsg();
    }
};







