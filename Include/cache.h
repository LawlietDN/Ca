#pragma once
#include "session.h"
#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <nlohmann-json.hpp>
#include <fstream>
#include <filesystem>
class Cache: public std::enable_shared_from_this<Cache>
{
private:
    boost::asio::ip::tcp::socket& _clientSocket;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::streambuf responseBuffer;
    std::string _origin;
    std::string _path;
    std::string fileName = "../src/Network/Cache/cache.json";
    std::array<char, 1024> data;
    void write(std::string const& message, boost::asio::ip::tcp::socket& socket);
    void connect();
    void readResponse();
    void trim();
    std::string requestMessage();
    void store(std::string const& identifier, nlohmann::json const& cachedResponse);
    bool isCached();
    std::string makeHeaders(size_t messageSize);
    void changeContentLength(std::string& fullResponse);
public:
    explicit Cache(boost::asio::ip::tcp::socket& clientSocket, boost::asio::io_context& io, std::string const& origin, std::string const& path)
    :   _clientSocket(clientSocket),
        _socket(io),
        resolver(io),
        _origin(origin),
        _path(path) {}
    
    void start();
};