#pragma once
#include <boost/asio.hpp>
#include <memory>
#include <iostream>

class Cache;

class Session: public std::enable_shared_from_this<Session>
{
    friend class Cache;
private:
    boost::asio::ip::tcp::socket _socket;
    std::array<char, 1024> data;
    std::string incomingData;
    std::string path;
    void storePath(size_t bytesRecieved);

    void read(std::string const& origin);
    void write(std::string const& message);


public:
    explicit Session(boost::asio::ip::tcp::socket socket)
    :   _socket(std::move(socket)) {}

    void start(std::string const& origin);

};