#include "session.h"

void Session::read(std::string const& origin)
{
    std::cout << "ddddk\n";

    auto self = shared_from_this();
    _socket.async_read_some(boost::asio::buffer(data),
    [self, origin](boost::system::error_code e, size_t bytesRecieved)
    {
        if(e)
        {
            std::cerr << "Error receiving data: " << e.message();
            return;
        }
        std::cout << "ddddk\n";
        if(!self->isValidRequests(bytesRecieved)) return;
        std::cout << "eee\n";
        std::make_shared<Cache>(static_cast<boost::asio::io_context&>(self->_socket.get_executor().context()), origin, self->path)->start();
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
    [self](boost::system::error_code e, size_t transferredBytes)
    {
        if(e)
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

