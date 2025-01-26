#include "session.h"
#include <nlohmann-json.hpp>
void Session::read(std::string const& origin)
{
    auto self = shared_from_this();
    _socket.async_read_some(boost::asio::buffer(data),
    [self, origin](boost::system::error_code const& e, size_t const& bytesRecieved)
    {
        if(e)
        {
            std::cerr << "Error receiving data: " << e.message() << '\n';
            return;
        }
        if(!self->isValidRequests(bytesRecieved)) return;
        std::make_shared<Cache>(self->_socket, static_cast<boost::asio::io_context&>(self->_socket.get_executor().context()), origin, self->path)->start();
        self->read(origin);
    });
}
bool Session::isValidRequests(size_t bytesRecieved)
{

    std::string requestLine(data.data(), bytesRecieved);

    std::istringstream iss(requestLine);
    std::string method;
    iss >> method >> path;
    
    return true;
}

void Session::write(std::string const& message)
{
    auto self = shared_from_this();
    boost::asio::async_write(_socket, boost::asio::buffer(message),
    [self](boost::system::error_code const& e, size_t const& transferredBytes)
    {
        if(e && e != boost::asio::error::eof)
        {
            std::cerr << "Error while writing: " << e.message();
            return;
        }

    });
}

void Session::start(std::string const& origin)
{
        read(origin);
    
}

