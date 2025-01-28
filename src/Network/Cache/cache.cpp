#include "cache.h"
#include "session.h" 

bool Cache::start()
{
    trim();
    if(isCached())
    {
        return true;
    }

    connect();
    
    return false;
}

void Cache::write(std::string const& message, boost::asio::ip::tcp::socket& socket)
{
    auto self = shared_from_this();
    auto messagePtr = std::make_shared<std::string>(message);
    boost::asio::async_write(socket, boost::asio::buffer(*messagePtr),
    [self, &socket, messagePtr](boost::system::error_code e, size_t)
    {
        if(e && e != boost::asio::error::eof)
        {
            return;
        }
        if(&socket == &self->_socket)
        {
            self->readResponse();
        }
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
            return;
        }
        std::cout << "A new request from " << self->_session->_socket.remote_endpoint() << " for " << self->_path  << " has been sucessfully forwarded to: " << self->_origin << '\n';
        self->write(self->requestMessage(), self->_socket);

    });
}


void Cache::readResponse()  
{
    auto self = shared_from_this();
    boost::asio::async_read(_socket, responseBuffer,boost::asio::transfer_all(),
    [self](boost::system::error_code e, size_t)
    {
        if (e && e != boost::asio::error::eof)
        {
            return;
        }
            self->handleResponse(self->responseBuffer);

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
void Cache::store(std::string const& identifier, nlohmann::json const& cachedResponse)
{
    try
    {
        nlohmann::json jsonFile;

        if (std::filesystem::exists(fileName))
        {
            if (std::ifstream(fileName).peek() == std::ifstream::traits_type::eof())
            {
                std::ofstream(fileName) << "{}";
            }
            std::ifstream(fileName) >> jsonFile;
        }

        jsonFile[identifier] = cachedResponse["Response"].get<std::string>();
        std::ofstream(fileName) << jsonFile.dump(4);
    }
    catch (std::exception const& e)
    {
        std::cerr << "Error in caching process: " << e.what() << std::endl;
    }
}

bool Cache::isCached()
{
    std::string identifier = _origin + _path;
    try
    {
        nlohmann::json jsonFile;
        if(!std::filesystem::exists(fileName) || std::ifstream(fileName).peek() == std::ifstream::traits_type::eof())
        {
            return false;
        }
        
        std::ifstream(fileName) >> jsonFile;    
        if(jsonFile.contains(identifier))
        {
            std::string cachedResponse = jsonFile[identifier].get<std::string>();
            size_t headerEnd = cachedResponse.find("\r\n\r\n");
            cachedResponse += "\n\nX-Cache: HIT. ";
            std::cout << "Request has been found in cache.\nReturned the cached response.\n";
            write(cachedResponse, _session->_socket);
            return true;
        }

    }
    catch(std::exception const& e)
    {
        std::cerr << "An error occured while searching for cached responses: " << e.what();
        return false;
    }
    return false;
}

std::string Cache::makeHeaders(size_t messageSize)
{
    std::ostringstream message;
    message << "HTTP/1.1 200 OK\r\n";
    message << "Content-Length: " << messageSize << "\r\n";
    message << "Connection: close\r\n\r\n";
    return message.str();

}

void Cache::changeContentLength(std::string& fullResponse)
{
    size_t headerEnd = fullResponse.find("\r\n\r\n");
    std::string headers = fullResponse.substr(0, headerEnd);
    size_t contentLengthPosition = headers.find("Content-Length:");
    size_t startPosition = contentLengthPosition + std::string("Content-Length:").length();
    size_t endPosition = headers.find("\r\n", startPosition);

    size_t originalContentLength = std::stoul(headers.substr(startPosition, endPosition - startPosition));
    size_t newContentLength = originalContentLength + std::string("\n\nX-Cache: MISS").size();
    fullResponse.replace(contentLengthPosition + 16, endPosition - startPosition, std::to_string(newContentLength));
    fullResponse += "\n\nX-Cache: MISS";
}

void Cache::handleResponse(boost::asio::streambuf& responseBuffer)
{
    std::string fullResponse((std::istreambuf_iterator<char>(&responseBuffer)), std::istreambuf_iterator<char>());
    std::istringstream responseStream(fullResponse);

    std::string httpVersion;
    int statusCode;
    std::string statusMessage;
    responseStream >> httpVersion >> statusCode >> statusMessage;

    if(statusCode == 200)
    {
        nlohmann::json cachedResponse;
        cachedResponse["Response"] = fullResponse.substr(0, fullResponse.size() - std::string("\n\nX-Cache: MISS").size()); 

        std::string identifier = _origin + _path;
        store(identifier, cachedResponse);
        changeContentLength(fullResponse);
        write(fullResponse, _session->_socket);
    }
    else
    {
        write(fullResponse, _session->_socket);
    }
}
