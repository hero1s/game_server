#pragma once

#include <memory>
#include <asio/ip/tcp.hpp>
#include <asio.hpp>

#include "byte_buffer.h"
#include "tcp_callbacks.h"

using asio::ip::tcp;
using asio::ip::address;
namespace NetworkAsio {
    class TCPConn
            : public std::enable_shared_from_this<TCPConn> {
    public:
        enum Type {
            kIncoming = 0, // The type of a TCPConn held by a TCPServer
            kOutgoing = 1, // The type of a TCPConn held by a TCPClient
        };
        enum Status {
            kDisconnected = 0,
            kConnecting = 1,
            kConnected = 2,
            kDisconnecting = 3,
        };

        TCPConn(asio::io_service &service_, tcp::socket &&socket, std::string name);

        ~TCPConn();

        void Close();

        void SetMessageCallback(const MessageCallback &cb) { msg_fn_ = cb; }

        void SetConnCallback(const ConnCallback &cb) { conn_fn_ = cb; }

        void SetHighWaterMarkCallback(const HighWaterMarkCallback &cb, size_t mark);

        void SetCloseCallback(const CloseCallback &cb) { close_fn_ = cb; }

        void OnAttachedToLoop();

        std::string GetLocalAddress() const { return local_ep_.address().to_string(); }

        uint16_t GetLocalPort() const { return local_ep_.port(); }

        std::string GetRemoteAddress() const { return remote_ep_.address().to_string(); }

        uint16_t GetRemotePort() const { return remote_ep_.port(); }

        void SetTCPNoDelay(bool on);

        bool Send(const char *data, size_t sz);

        bool Send(const std::string &msg);

        bool IsConnected() const { return status_ == kConnected; }

        bool IsConnecting() const { return status_ == kConnecting; }

        bool IsDisconnected() const { return status_ == kDisconnected; }

        bool IsDisconnecting() const { return status_ == kDisconnecting; }

        void SetType(Type t) { type_ = t; }

        Type GetType() const { return type_; }

        bool IsIncommingConn() const { return type_ == kIncoming; }

        Status GetStatus() const { return status_; }

        std::string GetName() const { return name_; }

        void SetUID(uint32_t uid) { uid_ = uid; }

        uint32_t GetUID() { return uid_; }

        void Timeout(time_t now);

        static time_t Now();
    protected:
        void HandleClose();

        void HandleError();

        void AsyncRead();

        void AsyncWrite();

        void HandleRead(asio::error_code err, std::size_t trans_bytes);

        void HandleWrite(asio::error_code err, std::size_t trans_bytes);

        bool SendInLoop(const char *data, size_t sz);

    private:
        asio::io_service &io_service_;
        tcp::socket socket_;

        Type type_;
        Status status_;
        std::string name_;
        uint32_t uid_;

        tcp::endpoint local_ep_;
        tcp::endpoint remote_ep_;

        ByteBuffer recv_buffer_;
        time_t recvtime_ = 0;
        uint32_t timeout_ = 0;

        bool async_writing_;
        ByteBuffer write_buffer_;
        ByteBuffer writing_buffer_;

        size_t high_water_mark_; // Default 32MB

        ConnCallback conn_fn_;
        MessageCallback msg_fn_;
        WriteCompleteCallback write_complete_fn_;
        HighWaterMarkCallback high_water_mark_fn_;
        CloseCallback close_fn_;
    };
};
