
#pragma once

#include <memory>
#include <functional>
#include "byte_buffer.h"

namespace Network {
    class TCPConn;

    typedef std::shared_ptr<TCPConn> TCPConnPtr;

    typedef std::function<void(const TCPConnPtr &)> ConnCallback;

    typedef std::function<void(const TCPConnPtr &)> CloseCallback;
    typedef std::function<void(const TCPConnPtr &)> WriteCompleteCallback;

    typedef std::function<void(const TCPConnPtr &, size_t)> HighWaterMarkCallback;

    typedef std::function<void(const TCPConnPtr &, ByteBuffer &)> MessageCallback;

    void DefaultConnectionCallback(const TCPConnPtr & connPtr);

    void DefaultMessageCallback(const TCPConnPtr & connPtr, ByteBuffer &buffer);

};
