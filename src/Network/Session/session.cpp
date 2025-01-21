#include "session.h"

void Session::read(std::string const& origin)
{
    auto self = shared_from_this();
    _socket.async_read_some(boost::asio::buffer(data),
    [self, origin](boost::system::error_code e, size_t bytesRecieved)
    {
        if(e)
        {
            std::cerr << "Error receiving data: " << e.message();
            return;
        }
        if(!self->isValidRequests(bytesRecieved)) return;
        std::make_shared<Cache>(static_cast<boost::asio::io_context&>(self->_socket.get_executor().context()), origin, self->path)->start();
        self->read(origin);
    });
}
bool Session::isValidRequests(size_t bytesRecieved)
{

    std::string requestLine(data.data(), bytesRecieved);

    std::istringstream iss(requestLine);
    std::string method;
    iss >> method >> path;

    if (path == "/")
    {
        std::cerr << "No path has been specified\n";
        return false;
    }
    return true;
}



void Session::start(std::string const& origin)
{
    read(origin);
}