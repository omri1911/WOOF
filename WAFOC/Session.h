#ifndef SESSION_H
#define SESSION_H

#include <asio.hpp>    // ASIO library header for networking functionalities
#include <array>       // Array library for static array usage
#include <iostream>    // Iostream for logging and debug output
#include <memory>      // Memory library for std::enable_shared_from_this

using asio::ip::tcp;   // Simplify the use of ASIO's TCP namespace

// Session class to handle a single connection between client and server
class Session : public std::enable_shared_from_this<Session> {
public:
    // Constructor that initializes the session with an io_context
    Session(asio::io_context& io_context);

    // Getter for client socket
    tcp::socket& socket();

    // Start the session and begin communication between client and server
    void start();

private:
    // Asynchronously read data from the client
    void readFromClient();

    // Asynchronously write data to the server
    void writeToServer(std::size_t length);

    // Asynchronously read data from the server
    void readFromServer();

    // Asynchronously write data to the client
    void writeToClient(std::size_t length);

    tcp::socket client_socket_;          // Socket for communication with the client
    tcp::socket server_socket_;          // Socket for communication with the server
    std::array<char, 1024> buffer_;      // Buffer to store data being transferred
};

#endif // SESSION_H
