#ifndef CLIENT_SOCKET_GUARD
#define CLIENT_SOCKET_GUARD

#include <boost/asio.hpp>

#include <string>
#include <deque>
#include <memory>

namespace sockets
{
    class Client_socket
    {
        public:
            Client_socket ( Client_socket&& ) = default;
            Client_socket ( std::string address, std::string service );
            Client_socket (
                    std::unique_ptr < boost::asio::ip::tcp::socket >&& socket,
                    std::shared_ptr < boost::asio::io_service > io_service
                    );

            // I don't want this to be copyable, because of boost not allowing it as well
            Client_socket& operator= ( Client_socket&& ) = default;
            Client_socket& operator= ( const Client_socket& ) = delete;

            std::string read_line (); // Read until '\n'
            void send ( std::string message );
            void close ();

            virtual ~Client_socket () = default;

        private:
            std::shared_ptr < boost::asio::io_service > io_service;
            std::unique_ptr < boost::asio::ip::tcp::socket > socket;

            // Attributes for Client_socket::read_line
            std::deque < std::string > next_lines;
            std::string line_rest;
    };
}

#endif //CLIENT_SOCKET_GUARD
