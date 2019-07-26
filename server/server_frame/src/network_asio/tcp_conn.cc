
#include "network_asio/tcp_conn.h"
#include "network_asio/message_head.h"
#include <assert.h>

namespace NetworkAsio {
    TCPConn::TCPConn(asio::io_service &service_, tcp::socket &&socket, std::string name)
            : io_service_(service_), socket_(std::move(socket)), type_(kIncoming), status_(kDisconnected), name_(name),
              local_ep_(socket_.local_endpoint()), remote_ep_(socket_.remote_endpoint()), recv_buffer_(),
              async_writing_(false), write_buffer_(), writing_buffer_(), high_water_mark_(32 * 1024 * 1024),
              conn_fn_(DefaultConnectionCallback), msg_fn_(DefaultMessageCallback), write_complete_fn_(nullptr), high_water_mark_fn_(nullptr),
              close_fn_(nullptr) {

    }

    TCPConn::~TCPConn() {
        //assert(status_ == kDisconnected);
        //assert(!socket_.is_open());
    }

    void TCPConn::Close() {
        status_ = kDisconnecting;
        auto c = shared_from_this();

        asio::error_code ec;
        c->socket_.close(ec);
    }

//    bool TCPConn::Update() {
//        if (!socket_.is_open()) {
//            return false;
//        }
//        return true;
//    }


    void TCPConn::HandleClose() {
        if (status_ == kDisconnected) {
            return;
        }

        // close by peer
        if (socket_.is_open()) {
            asio::error_code ec;
            socket_.close(ec);
        }

        status_ = kDisconnecting;

        TCPConnPtr conn(shared_from_this());
        if (conn_fn_) {
            assert(status_ == kDisconnecting);
            conn_fn_(conn);
        }

        if (close_fn_) {
            close_fn_(conn);
        }
        status_ = kDisconnected;
    }

    void TCPConn::HandleError() {
        status_ = kDisconnecting;
        HandleClose();
    }

    void TCPConn::SetHighWaterMarkCallback(const HighWaterMarkCallback &cb, size_t mark) {
        high_water_mark_fn_ = cb;
        high_water_mark_ = mark;
    }

    void TCPConn::OnAttachedToLoop() {
        status_ = kConnected;
        conn_fn_(shared_from_this());
        AsyncRead();
    }

    void TCPConn::SetTCPNoDelay(bool on) {
        tcp::no_delay option(on);
        asio::error_code ec;
        socket_.set_option(option, ec);
        if (ec) {
            // Log
        }
    }

    bool TCPConn::Send(const char *data, size_t sz) {
        return SendInLoop(data, sz);
    }

    bool TCPConn::Send(const std::string &msg) {
        return SendInLoop(msg.c_str(), msg.size());
    }

    bool TCPConn::SendInLoop(const char *data, size_t sz) {
        if (!socket_.is_open()) {
            return false;
        }
        std::shared_ptr<MessageBuffer> msg = CreateMessageWithHeader(data, sz);
        if (!async_writing_) {
            if (write_buffer_.Size() > 0) {
                writing_buffer_.Swap(write_buffer_);
            }
            writing_buffer_.Write(msg->data_, msg->length_);
            AsyncWrite();
        } else {
            write_buffer_.Write(msg->data_, msg->length_);

            size_t buffer_size = write_buffer_.Size() + writing_buffer_.Size();
            if (buffer_size > high_water_mark_) {
                if (high_water_mark_fn_) {
                    high_water_mark_fn_(shared_from_this(), buffer_size);
                }
            }
        }
        return true;
    }

    void TCPConn::AsyncRead() {
        recv_buffer_.Normalize();
        recv_buffer_.EnsureValidSize();
        socket_.async_read_some(asio::buffer(recv_buffer_.WriteBegin(), recv_buffer_.ValidSize()),
                                std::bind(&TCPConn::HandleRead, shared_from_this(), std::placeholders::_1,
                                          std::placeholders::_2));
    }

    void TCPConn::AsyncWrite() {
        if (async_writing_ || writing_buffer_.Size() <= 0) return;
        async_writing_ = true;
        socket_.async_write_some(asio::buffer(writing_buffer_.ReadBegin(), writing_buffer_.Size()),
                                 std::bind(&TCPConn::HandleWrite, shared_from_this(), std::placeholders::_1,
                                           std::placeholders::_2));
    }

    void TCPConn::HandleRead(asio::error_code err, std::size_t trans_bytes) {
        if (err) {
            if (err == asio::error::eof) {
                status_ = kDisconnecting;
                HandleClose();
                return;
            }
            HandleError();
            return;
        }
        recv_buffer_.WriteBytes(trans_bytes);
        if (recv_buffer_.Size() >= sizeof(message_head)) {
            message_head *head = (message_head *) recv_buffer_.ReadBegin();
            if (recv_buffer_.Size() >= head->length_ + sizeof(message_head)) {
                //crc32 to do
                std::shared_ptr<MessageBuffer> msg = CreateMessage(recv_buffer_.ReadBegin() + sizeof(message_head),
                                                                   head->length_);
                recv_buffer_.ReadBytes(head->length_ + sizeof(message_head));

                ByteBuffer buf;
                buf.Write(msg->data_, msg->length_);
                msg_fn_(shared_from_this(), buf);
            }
        }

        AsyncRead();
    }

    void TCPConn::HandleWrite(asio::error_code err, std::size_t trans_bytes) {
        async_writing_ = false;

        if (err) {
            HandleError();
            return;
        }

        writing_buffer_.ReadBytes(trans_bytes);
        if (writing_buffer_.Size() > 0) {
            AsyncWrite();
            return;
        }
        writing_buffer_.Normalize();

        if (write_buffer_.Size() > 0) {
            writing_buffer_.Swap(write_buffer_);
            AsyncWrite();
            return;
        }

    }

};
