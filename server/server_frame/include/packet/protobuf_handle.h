//
// Created by toney on 2019/2/1.
//
#pragma once

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/message.h>
#include "svrlib.h"
#include "network/tcp_conn.h"

using namespace Network;

using handFunc = function<int()>;

#ifndef PARSE_MSG
#define PARSE_MSG(msg)                   \
    if(ParseMsg(msg)<0){                 \
        LOG_DEBUG("PARSE_MSG fail");     \
        return -1;                       \
    }
#endif // PARSE_MSG

// ��Ϣ����
template<typename Head>
class CProtobufHandleBase {
public:
    using handBase = CProtobufHandleBase<Head>;
    //  �յ��ͻ�����Ϣʱ�ص�
    virtual int OnRecvClientMsg() {
        auto it = m_handlers.find(_cmd);
        if (it != m_handlers.end()) {
            return it->second();
        }
        for(auto p:m_subHands){
            auto ret = p->OnDispatchMsg(_connPtr,_pkt_buf,_buf_len,_head);
            if(ret <= 0)return ret;
        }

        return 1;
    }

    //  �����Ϣӳ��
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
    void RegisterSubHandle(shared_ptr<handBase> hand){
        m_subHands.emplace_back(hand);
    }


    int OnDispatchMsg(const TCPConnPtr& connPtr, const uint8_t *pkt_buf,uint16_t buf_len,Head * head){
        _connPtr = connPtr;
        _head = head;
        _pkt_buf = pkt_buf;
        _buf_len = buf_len;
        _cmd = head->cmd;
        return OnRecvClientMsg();
    }

protected:
    std::unordered_map<uint32_t, handFunc> m_handlers;
    TCPConnPtr _connPtr;
    const uint8_t *_pkt_buf;
    uint16_t _buf_len;
    uint32_t _cmd;
    Head * _head;
private:
    std::vector<shared_ptr<handBase>> m_subHands;

};

