#include "session.h"
#include "cache.h"
void Session::read(std::string const& origin)
{
    auto self = shared_from_this();
    _socket.async_read_some(boost::asio::buffer(data),
    [self, origin](boost::system::error_code const& e, size_t const& bytesRecieved)
    {
        if(e && e != boost::asio::error::eof)
        {
            return;
        }
        self->storePath(bytesRecieved);
        std::make_shared<Cache>(self, static_cast<boost::asio::io_context&>(self->_socket.get_executor().context()), origin, self->path)->start();
    });
}
void Session::storePath(size_t bytesRecieved)
{

    std::string requestLine(data.data(), bytesRecieved);

    std::istringstream iss(requestLine);
    std::string method;
    iss >> method >> path;
    
}

void Session::write(std::string const& message)
{
    auto self = shared_from_this();
    boost::asio::async_write(_socket, boost::asio::buffer(message),
    [self](boost::system::error_code const& e, size_t const& transferredBytes)
    {
        if(e && e != boost::asio::error::eof)
        {
            return;
        }

    });
}

void Session::start(std::string const& origin)
{
        read(origin);
    
}

