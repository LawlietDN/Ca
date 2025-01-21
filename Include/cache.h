#include <iostream>
#include <memory>
#include <boost/asio.hpp>
class Cache: public std::enable_shared_from_this<Cache>
{
private:
    boost::asio::ip::tcp::socket _socket;
    boost::asio::ip::tcp::resolver resolver;
     boost::asio::streambuf responseBuffer;
    std::string _origin;
    std::string _path;
    void forward();
    void connect();
    void readResponse();
public:
    explicit Cache( boost::asio::io_context& io, std::string const& origin, std::string const& path)
    :   _socket(io),
        resolver(io),
        _origin(origin),
        _path(path) {}
    
    void start();
};