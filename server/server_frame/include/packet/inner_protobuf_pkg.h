//
// Created by Administrator on 2018/8/8.
//

#pragma once

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/message.h>
#include "svrlib.h"
#include "network/NetworkObject.h"
#include "protobuf_handle.h"

using namespace svrlib;
using namespace Network;

#pragma  pack(1)
//内部数据包头------------------------------------------------------------------------------------------------------------
typedef struct inner_header_t {
    uint8_t route;         // 路由类型
    uint32_t routeMain;     // 主路由
    uint32_t routeSub;      // 子路由
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

int GetInnerPacketLen(const uint8_t *pData, uint16_t wLen);

bool SendInnerMsg(NetworkObject *pNetObj, const google::protobuf::Message *msg, uint16_t msg_type, uint32_t uin,
                  uint8_t route = 0, uint32_t routeMain = 0, uint32_t routeSub = 0);

bool SendInnerMsg(NetworkObject *pNetObj, const void *msg, uint16_t msg_len, uint16_t msg_type, uint32_t uin,
                  uint8_t route = 0, uint32_t routeMain = 0, uint32_t routeSub = 0);

// 消息处理
class CInnerMsgHanlde : public CProtobufHandleBase {
public:
    int OnHandleClientMsg(NetworkObject *pNetObj, uint8_t *pData, size_t uiDataLen);


protected:
    INNERHEAD * _head;
};


