#ifndef PROXY_SERVER_H
#define PROXY_SERVER_H

#include <asio.hpp>
#include "Session.h"

using asio::ip::tcp;

class ProxyServer {
public:
    ProxyServer(asio::io_context& io_context, short port);
    void startAccept();

private:
    void handleAccept(std::shared_ptr<Session> new_session, const asio::error_code& error);

    asio::io_context& io_context_;
    tcp::acceptor acceptor_;
};

#endif // PROXY_SERVER_H
