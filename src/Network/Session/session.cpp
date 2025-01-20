#include "../Include/session.h"

void Session::read()
{
    std::shared_ptr<Session> self = shared_from_this();
    _socket.async_read_some(boost::asio::buffer(data, buffer),
    [self](boost::system::error_code e, size_t bytesRecieved)
    {
        if(e)
        {
            std::cerr << "Error receiving data: " << e.message();
            return;
        }
        self->handleRequests(bytesRecieved);
        std::cout << "\nPath:" << self->path << '\n';
        std::cout << self->incomingData;
        self->read();
    });
}
void Session::handleRequests(size_t bytesRecieved)
{
        incomingData.append(data.data(), bytesRecieved);
        auto position = incomingData.find("\r\n");

        if(position != std::string::npos)
        {
        std::string requestLine = incomingData.substr(0, position);
        incomingData.erase(0, position + 2);

        std::istringstream iss(requestLine);
        std::string method;
        iss >> method >> path;
        }
        
        if(path == "/")
        {
           std::cerr << "No path has been specified\n";
           return;
        }
}



void Session::start()
{
    read();
}