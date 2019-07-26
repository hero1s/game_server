//
// Created by yinguohua on 2019/7/26.
//
#include "network_asio/tcp_callbacks.h"
#include "network_asio/tcp_conn.h"
#include "utility/comm_macro.h"

namespace NetworkAsio {

    inline void DefaultConnectionCallback(const TCPConnPtr & connPtr) {
        LOG_DEBUG("connect ev:{} from:{}:{}",connPtr->GetName(),connPtr->GetRemoteAddress(),connPtr->GetRemotePort());
    }

    inline void DefaultMessageCallback(const TCPConnPtr & connPtr, ByteBuffer &buffer) {
        LOG_DEBUG("recv {},msg from:{},{},size:{}",connPtr->GetName(),connPtr->GetRemoteAddress(),connPtr->GetRemotePort(),buffer.Size());
    }

};

