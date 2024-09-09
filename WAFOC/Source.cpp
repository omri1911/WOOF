#include <asio.hpp>           // ASIO library for networking functionalities
#include <iostream>           // Iostream for logging and debug output
#include "Session.h"          // Include the Session class header

using asio::ip::tcp;          // Simplify the use of ASIO's TCP namespace

// Server class to handle incoming client connections
class Server {
public:
    // Constructor that initializes the server with an io_context and a listening port
    Server(asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)), io_context_(io_context) {
        startAccept();        // Start accepting client connections
    }

private:
    // Start an asynchronous accept operation to handle incoming client connections
    void startAccept() {
        // Create a new session to handle the client connection
        auto new_session = std::make_shared<Session>(io_context_); // Pass io_context_ to match constructor

        // Start an asynchronous accept operation
        acceptor_.async_accept(new_session->socket(),
            [this, new_session](const asio::error_code& error) {
                if (!error) {
                    // If accept is successful, start the session
                    new_session->start();
                }

                // Continue accepting new connections
                startAccept();
            });
    }

    tcp::acceptor acceptor_;  // Acceptor to listen for incoming client connections
    asio::io_context& io_context_; // Store a reference to io_context
};

int main() {
    try {
        asio::io_context io_context;  // Create an I/O context to handle asynchronous operations

        Server server(io_context, 8080);  // Create a server listening on port 8080

        io_context.run();  // Run the I/O context to start handling asynchronous operations
    }
    catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << "\n";  // Catch and display any exceptions
    }

    return 0;
}
