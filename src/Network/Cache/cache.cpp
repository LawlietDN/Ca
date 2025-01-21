#include "cache.h"

void Cache::forward()
{
    std::cout << "Path: " << _path << "\nOrigin: " << _origin << '\n';
    std::string requestMessage;
    std::ostringstream requestMsg;
    requestMsg << "GET " << _path << " HTTP/1.1\r\n";
    requestMsg << "Host: " << _origin << "\r\n";
    requestMsg << "User-Agent: MyCache/1.0\r\n"; 
    requestMsg << "Accept: " << "*/*\r\n";
    requestMsg << "Connection: close\r\n\r\n";
    requestMessage = requestMsg.str();
    auto self = shared_from_this();
    std::cout << "Sending\n";
    boost::asio::async_write(_socket, boost::asio::buffer(requestMessage),
    [self](boost::system::error_code e, size_t transfereedBytes)
    {
        if(e)
        {
            std::cerr << "Error while forwarding request: " << e.message();
        }
        self->readResponse();
    });
}

void Cache::connect()
{
    auto endpoints = resolver.resolve(_origin, "80");
    auto self = shared_from_this();
    boost::asio::async_connect(_socket, endpoints,
    [self](boost::system::error_code e, boost::asio::ip::tcp::endpoint const& endpoints)
    {
        if(e)
        {
            std::cerr << "Error while connecting to the origin: " << e.message();
            return;
        }
        self->forward();
    });
}
void Cache::start()
{
    if (_origin.find("https://"))
    {
        _origin = _origin.substr(8);
    } else if (_origin.find("http://"))
    {
        _origin = _origin.substr(7);
    }
     size_t slashPos = _origin.find('/');
    if (slashPos != std::string::npos)
    {
        _origin = _origin.substr(0, slashPos);
    }
    connect();
    
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
            responseStream >> httpVersion >> statusCode;
            std::getline(responseStream, statusMessage);    

            if(statusCode == 200)
            {
                std::string fullResponse((std::istreambuf_iterator<char>(&self->responseBuffer)),std::istreambuf_iterator<char>());
        
                std::cout << "Response: " << fullResponse << '\n';
            }
            else
            {
                std::cout << "Status: " << self->_origin << self->_path << " " << statusCode << " " << statusMessage << '\n';
                return;
            }

    });
}