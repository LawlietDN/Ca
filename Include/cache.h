#pragma once
#include "session.h"
#include <iostream>
#include <memory>
#include <boost/asio.hpp>
class Cache: public std::enable_shared_from_this<Cache>
{
private:
    boost::asio::ip::tcp::socket& _clientSocket;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::streambuf responseBuffer;
    std::string _origin;
    std::string _path;
    std::array<char, 1024> data;
    void write(std::string const& message, boost::asio::ip::tcp::socket& socket);
    void connect();
    void readResponse();
    void trim();
    std::string requestMessage();

public:
    explicit Cache(boost::asio::ip::tcp::socket& clientSocket, boost::asio::io_context& io, std::string const& origin, std::string const& path)
    :   _clientSocket(clientSocket),
        _socket(io),
        resolver(io),
        _origin(origin),
        _path(path) {}
    
    bool start();
};