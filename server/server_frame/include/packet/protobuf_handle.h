//
// Created by toney on 2019/2/1.
//
#pragma once

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/message.h>
#include "svrlib.h"
#include "network/NetworkObject.h"

using handFunc = function<int()>;

#ifndef PARSE_MSG
#define PARSE_MSG(msg)                   \
    if(ParseMsg(msg)<0)return -1;
#endif // PARSE_MSG

// 消息处理
template<typename Head>
class CProtobufHandleBase {
public:
    //  收到客户端消息时回调
    virtual int OnRecvClientMsg() {
        auto it = m_handlers.find(_cmd);
        if (it != m_handlers.end()) {
            return it->second();
        }
        return 1;
    }

    //  添加消息映射
    template<class TYPE, class F>
    void bind_handler(TYPE *m, uint32_t key, F f) {
        m_handlers[key] = std::bind(f, m);
    }

    template<class MSGTYPE>
    int ParseMsg(MSGTYPE &msg) {
        if (!msg.ParseFromArray(_pkt_buf, _buf_len)) {
            LOG_ERROR("message unpack fail {}", msg.GetTypeName());
            return -1;
        }
        return 0;
    }
    int OnDispatchMsg(NetworkObject *pNetObj, const uint8_t *pkt_buf,uint16_t buf_len,Head * head){
        _pNetObj = pNetObj;
        _head = head;
        _pkt_buf = pkt_buf;
        _buf_len = buf_len;
        _cmd = head->cmd;
        return OnRecvClientMsg();
    }

protected:
    std::unordered_map<uint32_t, handFunc> m_handlers;
    NetworkObject *_pNetObj;
    const uint8_t *_pkt_buf;
    uint16_t _buf_len;
    uint32_t _cmd;
    Head * _head;
};

