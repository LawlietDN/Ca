#pragma once
#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <nlohmann-json.hpp>
#include <fstream>
#include <filesystem>
class Session;

class Cache: public std::enable_shared_from_this<Cache>
{
private:
    std::shared_ptr<Session> _session;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::streambuf responseBuffer;
    std::string _origin;
    std::string _path;
    void write(std::string const& message, boost::asio::ip::tcp::socket& socket);
    void connect();
    void readResponse();
    void trim();
    std::string requestMessage();
    void store(std::string const& identifier, nlohmann::json const& cachedResponse);
    bool isCached();
    std::string makeHeaders(size_t messageSize);
    void changeContentLength(std::string& fullResponse);
    void handleResponse(boost::asio::streambuf& responseBuffer);

public:
    explicit Cache(std::shared_ptr<Session> session, boost::asio::io_context& io, std::string const& origin, std::string const& path)
    :   _session(session),
        _socket(io),
        resolver(io),
        _origin(origin),
        _path(path) {}
    inline static std::string fileName = "../src/Network/Cache/cache.json";
    bool start();
};