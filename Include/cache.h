#pragma once
#include <iostream>
#include <memory>
#include <boost/asio.hpp>
#include <nlohmann/json.hpp>
#include <fstream>
#include <filesystem>
#include <boost/asio/ssl.hpp>
class Session;

class Cache: public std::enable_shared_from_this<Cache>
{
private:
    std::shared_ptr<Session> _session;
    boost::asio::ip::tcp::socket _socket;
    boost::asio::ssl::context sslContext{boost::asio::ssl::context::tls_client};
    boost::asio::ssl::stream<boost::asio::ip::tcp::socket> tls;
    boost::asio::ip::tcp::resolver resolver;
    boost::asio::streambuf responseBuffer;
    std::string _origin;
    std::string _path;
    std::string port = "80";
    template<typename SocketType>
    void write(std::string const& message,  SocketType& socket);
    void connect();
    template<typename SocketType>
    void readResponse(SocketType& socket);
    void trim();
    std::string requestMessage();
    void store(std::string const& identifier, nlohmann::json const& cachedResponse);
    bool isCached();
    std::string makeHeaders(size_t messageSize);
    void changeContentLength(std::string& fullResponse);
    void handleResponse(boost::asio::streambuf& responseBuffer);
    void sslConnect(boost::asio::ip::tcp::resolver::results_type const& endpoints);

public:
    explicit Cache(std::shared_ptr<Session> session, boost::asio::io_context& io, std::string const& origin, std::string const& path)
    :   _session(session),
        _socket(io),
        tls(io,sslContext),
        resolver(io),
        _origin(origin),
        _path(path)
        {
            sslContext.set_default_verify_paths();
            sslContext.set_verify_mode(boost::asio::ssl::verify_peer);
        }
    inline static std::string fileName = "../src/Network/Cache/cache.json";
    bool start();
};