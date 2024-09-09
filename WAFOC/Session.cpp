#include "Session.h"

// Constructor for the Session class
Session::Session(asio::io_context& io_context)
    : client_socket_(io_context),  // Initialize client socket
    server_socket_(io_context) {}  // Initialize server socket

// Getter method for the client socket
tcp::socket& Session::socket() {
    return client_socket_;
}

// Starts the session by resolving the server address and connecting to it
void Session::start() {
    auto self(shared_from_this()); // Keep session alive during asynchronous operations
    tcp::resolver resolver(client_socket_.get_executor());
    auto endpoints = resolver.resolve("127.0.0.1", "3000");

    // Asynchronously connect to the server
    asio::async_connect(server_socket_, endpoints,
        [this, self](const asio::error_code& error, const tcp::endpoint&) {
            if (!error) {
                // If connection is successful, start reading from the client
                readFromClient();
            }
            else {
                // Handle connection error
                client_socket_.close();
                server_socket_.close();
            }
        });
}

// Asynchronously reads data from the client socket
void Session::readFromClient() {
    auto self(shared_from_this()); // Keep session alive during asynchronous operation

    // Start an asynchronous read operation from the client socket
    client_socket_.async_read_some(asio::buffer(buffer_),
        [this, self](asio::error_code ec, std::size_t length) {
            if (!ec) {
                // Successfully read data from the client, log it
                std::cout << "Client to Server: " << std::string(buffer_.data(), length) << std::endl;
                

                // Start an asynchronous write operation to send data to the server
                writeToServer(length);
            }
            else {
                // Handle error: close the client socket
                client_socket_.close();
                server_socket_.close();
            }
        });
}

// Asynchronously writes data to the server socket
void Session::writeToServer(std::size_t length) {
    auto self(shared_from_this()); // Keep session alive during asynchronous operation

    // Start an asynchronous write operation to the server
    asio::async_write(server_socket_, asio::buffer(buffer_, length),
        [this, self](asio::error_code ec, std::size_t /*bytes_transferred*/) {
            if (!ec) {
                // After writing to the server, start reading from the server
                readFromServer();
            }
            else {
                // Handle error: close the server socket
                server_socket_.close();
                client_socket_.close();
            }
        });
}

// Asynchronously reads data from the server socket
void Session::readFromServer() {
    auto self(shared_from_this()); // Keep session alive during asynchronous operation

    // Start an asynchronous read operation from the server socket
    server_socket_.async_read_some(asio::buffer(buffer_),
        [this, self](asio::error_code ec, std::size_t length) {
            if (!ec) {
                // Successfully read data from the server, log it
                std::cout << "Server to Client: " << std::string(buffer_.data(), length) << std::endl;

                // Start an asynchronous write operation to send data back to the client
                writeToClient(length);
            }
            else {
                // Handle error: close the server socket
                server_socket_.close();
                client_socket_.close();
            }
        });
}

// Asynchronously writes data to the client socket
void Session::writeToClient(std::size_t length) {
    auto self(shared_from_this()); // Keep session alive during asynchronous operation

    // Start an asynchronous write operation to the client
    asio::async_write(client_socket_, asio::buffer(buffer_, length),
        [this, self](asio::error_code ec, std::size_t /*bytes_transferred*/) {
            if (!ec) {
                // After writing back to the client, read from the client again
                readFromClient();
            }
            else {
                // Handle error: close the client socket
                client_socket_.close();
                server_socket_.close();
            }
        });
}
