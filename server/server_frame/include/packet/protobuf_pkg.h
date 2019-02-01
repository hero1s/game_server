
#pragma once

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/message.h>
#include "svrlib.h"
#include "network/NetworkObject.h"
#include "zlib.h"
#include "network/IOCPServer.h"

using namespace svrlib;
using namespace Network;

#pragma  pack(1)
//对外数据包头
typedef struct packet_header_t {
    uint32_t uin;           // uin(服务器内部转发用)
    uint16_t cmd;           // 消息id
    uint8_t encrypt;       // 加密
    uint8_t compress;      // 压缩
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

#ifndef PARSE_MSG_FROM_ARRAY
#define PARSE_MSG_FROM_ARRAY(msg)                   \
    if(!pkt_buf)                                    \
        return -1;                                  \
    if (!msg.ParseFromArray(pkt_buf, buf_len)) {    \
        LOG_ERROR("message unpack fail {}",msg.GetTypeName()); \
        return -1;                                  \
    }

#endif // PARSE_MSG_FROM_ARRAY

template<class MSGTYPE>
int ParseMsgFromArray(MSGTYPE &msg,const uint8_t *pkt_buf, uint16_t buf_len) {
    if (!msg.ParseFromArray(pkt_buf, buf_len)) {
        LOG_ERROR("message unpack fail {}",msg.GetTypeName());
        return -1;
    }
    return 0;
}

int GetProtobufPacketLen(const uint8_t *pData, uint16_t wLen);

bool SendProtobufMsg(NetworkObject *pNetObj, const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin,
                     uint8_t compress = 0, uint8_t crypt = 0);

bool SendProtobufMsg(NetworkObject *pNetObj, const void *msg, uint16_t msg_len, uint16_t msg_type, uint32_t uin,
                     uint8_t compress = 0, uint8_t crypt = 0);

void SetEncryptKey(string key);

// 消息处理
class CProtobufMsgHanlde {
public:
    int OnHandleClientMsg(NetworkObject *pNetObj, uint8_t *pData, size_t uiDataLen);

    //  收到客户端消息时回调
    virtual int OnRecvClientMsg(NetworkObject *pNetObj, const uint8_t *pkt_buf, uint16_t buf_len, PACKETHEAD *head) = 0;

    //  添加消息映射
    template<class TYPE, class F>
    void bind_handler(TYPE *m, int key, F f) {
        m_handlers[key] = std::bind(f, m, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3,
                                    std::placeholders::_4);
    }

protected:
    std::unordered_map<int, function<int(NetworkObject *pNetObj, const uint8_t *pkt_buf, uint16_t buf_len,
                                         PACKETHEAD *head)>> m_handlers;
};

// 消息解码
class CClientMsgDecode : public CMessageDecode {
public:
    virtual uint32_t GetHeadLen() {
        return PACKET_HEADER_SIZE;
    };

    virtual uint32_t GetPacketLen(const uint8_t *pData, uint16_t wLen) {
        return GetProtobufPacketLen(pData, wLen);
    };

    virtual uint32_t MaxTickPacket() {
        return 20;
    }
};

#define DUMP_PROTO_MSG_INFO(msg) \
do { \
    LOG_INFO("Dump {}{}{}", (msg).GetTypeName(), ":\n", (msg).DebugString()); \
} while (0)

#define DUMP_PROTO_MSG_WARNING(msg) \
do { \
    LOG_WARNING("Dump {}{}{}", (msg).GetTypeName(), ":\n", (msg).DebugString()); \
} while (0)

#define DUMP_PROTO_MSG_ERROR(msg) \
do { \
    LOG_ERROR("Dump {}{}{}", (msg).GetTypeName(), ":\n", (msg).DebugString()); \
} while (0)






