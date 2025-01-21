#pragma once
#include "session.h"

class Server
{
private:
    boost::asio::ip::tcp::acceptor acceptor;
    std::string _origin;
    void accept();

public:
    explicit Server(boost::asio::io_context& io, uint16_t const& port, std::string const& origin, std::string const& address)
    :   _origin(origin),
        acceptor(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(address), port))
        {
            std::cout << "Listening on: " << address << ":" << port << '\n';
            accept();
        }
    std::string getOrigin();
};