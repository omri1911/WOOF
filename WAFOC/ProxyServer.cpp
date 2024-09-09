#include "ProxyServer.h"

ProxyServer::ProxyServer(asio::io_context& io_context, short port)
    : io_context_(io_context),
    acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
    startAccept();
}

void ProxyServer::startAccept() {
    std::shared_ptr<Session> new_session = std::make_shared<Session>(io_context_);

    acceptor_.async_accept(new_session->socket(),
        [this, new_session](const asio::error_code& error) {
            handleAccept(new_session, error);
        });
}

void ProxyServer::handleAccept(std::shared_ptr<Session> new_session, const asio::error_code& error) {
    if (!error) {
        new_session->start();
    }
    startAccept();
}
