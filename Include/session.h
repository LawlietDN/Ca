#pragma once
#include "cache.h"
#include <boost/asio.hpp>
#include <memory>
#include <iostream>

class Session: public std::enable_shared_from_this<Session>
{
private:
    boost::asio::ip::tcp::socket _socket;
    std::array<char, 1024> data;
    enum{buffer = 1024};
    std::string incomingData;
    std::string path;
    void read(std::string const& origin);
    void handleRequests(size_t bytesRecieved, std::string const& origin);
public:
    explicit Session(boost::asio::ip::tcp::socket socket)
    :   _socket(std::move(socket)) {}

    void start(std::string const& origin);
};