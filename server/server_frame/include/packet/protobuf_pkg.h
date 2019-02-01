
#pragma once

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/message.h>
#include "svrlib.h"
#include "network/NetworkObject.h"
#include "zlib.h"
#include "network/IOCPServer.h"
#include "protobuf_handle.h"

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

int GetProtobufPacketLen(const uint8_t *pData, uint16_t wLen);

bool SendProtobufMsg(NetworkObject *pNetObj, const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin,
                     uint8_t compress = 0, uint8_t crypt = 0);

bool SendProtobufMsg(NetworkObject *pNetObj, const void *msg, uint16_t msg_len, uint16_t msg_type, uint32_t uin,
                     uint8_t compress = 0, uint8_t crypt = 0);

void SetEncryptKey(string key);

// 消息处理
class CProtobufMsgHanlde : public CProtobufHandleBase {
public:
    int OnHandleClientMsg(NetworkObject *pNetObj, uint8_t *pData, size_t uiDataLen);


protected:
    PACKETHEAD * _head;
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






