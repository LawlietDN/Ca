#include "cache.h"
bool Cache::start()
{

    trim();
    connect();
    
}

void Cache::write(std::string const& message, boost::asio::ip::tcp::socket& socket)
{
    
    auto self = shared_from_this();
    auto messageCopy = std::make_shared<std::string>(message);
    boost::asio::async_write(socket, boost::asio::buffer(*messageCopy),
    [self, messageCopy](boost::system::error_code e, size_t transfereedBytes)
    {
        if(e && e != boost::asio::error::eof)
        {
            std::cerr << "Error while writing dats: " << e.message();
        }
        std::cout << "aa"; 
        
        self->readResponse();
    });
}

void Cache::connect()
{

    auto endpoints = resolver.resolve(_origin, "80");
    auto self = shared_from_this();
    boost::asio::async_connect(_socket, endpoints,
    [self](boost::system::error_code const& e, boost::asio::ip::tcp::endpoint const& endpoints)
    {
        if(e && e != boost::asio::error::eof)
        {
            std::cerr << "Error while connecting to the origin: " << e.message();
            return;
        }
        
        self->write(self->requestMessage(), self->_socket);
    });
}


void Cache::readResponse()  
{
    auto self = shared_from_this();
    
    boost::asio::async_read(_socket, responseBuffer,boost::asio::transfer_all(),
    [self](boost::system::error_code e, size_t transferredBytes)
    {
        if (e && e != boost::asio::error::eof)
        {
            std::cerr << "Error while reading response: " << e.message();
            return;
        }
            std::istream responseStream(&(self->responseBuffer));
            std::string httpVersion;
            int statusCode;
            std::string statusMessage;
            responseStream >> httpVersion >> statusCode >> statusMessage;

            std::cout << "eher\n";
            if(statusCode == 200)
            {
                std::string fullResponse;
                while(std::getline(responseStream, fullResponse))
                {
                    std::cout << fullResponse << '\n';
                }
            }
            else
            {
                std::cout << "Here\n";
                std::ostringstream message;
                message << "Something is wrong with the provided URL: "
                    << self->_origin << self->_path << " "
                    << statusCode << " " << statusMessage << '\n';
                    std::cout << "ee\n";
                    self->write(message.str(), self->_clientSocket);
                    return;
            }

    });
}

void Cache::trim()
{
    if (_origin.find("https://") == 0)
    {
        _origin = _origin.substr(8);
    }
    else if (_origin.find("http://") == 0)
    {
        _origin = _origin.substr(7);
    }
    size_t slashPos = _origin.find('/');
    if (slashPos != std::string::npos)
    {
        _origin = _origin.substr(0, slashPos);
    }
}

std::string Cache::requestMessage()
{
    std::ostringstream requestMessage;
    requestMessage << "GET " << _path << " HTTP/1.1\r\n"
                   << "Host: " << _origin << "\r\n"
                   << "Accept: */*\r\n"
                   << "Connection: close\r\n\r\n";

    return requestMessage.str(); 
}
