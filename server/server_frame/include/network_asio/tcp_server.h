
#pragma once

#include <string>
#include <cstdint>
#include <memory>
#include <unordered_map>
#include <asio/ip/tcp.hpp>
#include <asio.hpp>
#include "tcp_callbacks.h"

using asio::ip::tcp;
namespace NetworkAsio {
    class TCPServer {
    public:
        TCPServer(asio::io_service &service_, const std::string &bind_ip, uint16_t port, const std::string &name);

        ~TCPServer();

        bool Start();

        void Stop();

        void SetConnectionCallback(const ConnCallback &cb) { conn_fn_ = cb; }

        void SetMessageCallback(const MessageCallback &cb) { msg_fn_ = cb; }

        const std::string Address() const {
            return acceptor_.local_endpoint().address().to_string();
        }

        void StopInLoop();

        void RemoveConn(const TCPConnPtr &conn);

    protected:
        void AsyncAccept();

        void HandleNewConn(tcp::socket &&socket);

    private:
        asio::io_service &io_service_;  // the listening loop
        tcp::acceptor acceptor_;
        tcp::socket accept_socket_; // default accept this socket

        bool closed_;
        const std::string name_;
        ConnCallback conn_fn_;
        MessageCallback msg_fn_;

        // always in the listening loop thread
        uint64_t next_conn_id_;
        typedef std::unordered_map<std::string, TCPConnPtr> ConnectionMap;
        ConnectionMap conns_;
    };
};

