#include <asio.hpp>
#include "network_asio/tcp_client.h"
#include "network_asio/tcp_conn.h"
#include <assert.h>

namespace NetworkAsio {
    static const int default_max_reconnect_time = 5000;

    TCPClient::TCPClient(asio::io_service &service_, const std::string &remote_addr, uint16_t port,
                         const std::string &name)
            : io_service_(service_), socket_(io_service_), remote_addr_(remote_addr), remote_port_(port), name_(name),
              reconnect_timer_(io_service_), auto_reconnect_(false), reconnect_interval_(3), reconnecting_times_(0),
              conn_(), conn_fn_(nullptr), msg_fn_(nullptr) {

    }

    TCPClient::~TCPClient() {
        auto_reconnect_.store(false);
        TCPConnPtr c = GetTCPConn();
        if (c) {
            // Most of the cases, the conn_ is at disconnected status at this time.
            // But some times, the user application layer will call TCPClient::Close()
            // and delete TCPClient object immediately, that will make conn_ to be at disconnecting status.
            assert(c->IsDisconnected() || c->IsDisconnecting());
            if (c->IsDisconnecting()) {
                // the reference count includes :
                //  - this
                //  - c
                //  - A disconnecting callback which hold a shared_ptr of TCPConn
                assert(c.use_count() >= 3);
                c->SetCloseCallback(CloseCallback());
            }
        }
        conn_.reset();
    }

    void TCPClient::Connect() {
        tcp::resolver resolver(io_service_);
        auto endpoint_iterator = resolver.resolve({remote_addr_, std::to_string(remote_port_)});
        asio::async_connect(socket_, endpoint_iterator,
                            [this](const asio::error_code &ec, const tcp::endpoint &endpoint) {
                                if (!ec) {
                                    reconnecting_times_ = 0;
                                    this->HandleConn(std::move(socket_));
                                } else {
                                    if (auto_reconnect_ && reconnecting_times_ < default_max_reconnect_time) {
                                        reconnect_timer_.async_wait(std::bind(&TCPClient::Reconnect, this));
                                        reconnect_timer_.expires_from_now(std::chrono::seconds() * reconnect_interval_);
                                    } else {
                                        ///
                                    }
                                }
                            });
    }

    void TCPClient::Reconnect() {
        ++reconnecting_times_;
        Connect();
    }

    void TCPClient::Disconnect() {
        DisconnectInLoop();
    }

    void TCPClient::DisconnectInLoop() {
        auto_reconnect_.store(false);
        if (conn_) {
            assert(!conn_->IsDisconnected() && !conn_->IsDisconnecting());
            conn_->Close();
        }
    }

    void TCPClient::SetConnCallback(const ConnCallback &cb) {
        conn_fn_ = cb;
        if (conn_) {
            conn_->SetConnCallback(conn_fn_);
        }
    }

    void TCPClient::HandleConn(tcp::socket &&socket) {
        if (!socket.is_open()) {
            conn_fn_(TCPConnPtr(new TCPConn(io_service_, std::move(socket), name_)));
            return;
        }

        TCPConnPtr c = TCPConnPtr(new TCPConn(io_service_, std::move(socket), name_));
        c->SetType(TCPConn::kOutgoing);
        c->SetMessageCallback(msg_fn_);
        c->SetConnCallback(conn_fn_);
        c->SetCloseCallback(std::bind(&TCPClient::OnRemoveConn, this, std::placeholders::_1));

        conn_ = c;

        c->OnAttachedToLoop();
    }

    void TCPClient::OnRemoveConn(const TCPConnPtr &c) {
        if (!conn_)
            return;
        assert(c.get() == conn_.get());
        conn_.reset();
        if (auto_reconnect_.load()) {
            Reconnect();
        }
    }

    TCPConnPtr TCPClient::GetTCPConn() const {
        return conn_;
    }
};