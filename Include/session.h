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
    std::string incomingData;
    std::string path;
    bool isValidRequests(size_t bytesRecieved);

    void read(std::string const& origin);
    void write(std::string const& message);


public:
    explicit Session(boost::asio::ip::tcp::socket socket)
    :   _socket(std::move(socket)) {}

    void start(std::string const& origin);

};