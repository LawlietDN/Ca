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
template<typename SocketType>
void Cache::write(std::string const& message, SocketType& socket)
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
        
        
            self->readResponse(socket);
        
    });
}

void Cache::connect()
{
    auto self = shared_from_this();
    resolver.async_resolve(_origin, port,
    [self](boost::system::error_code const& e,  boost::asio::ip::tcp::resolver::results_type const& endpoints)
    {
        if(e)
        {
            std::cout << "Failed to resolve: " << e.message();
            return;
        }
        if(self->port == "443")
        {
            self->sslConnect(endpoints);
            return;
        }
        boost::asio::async_connect(self->_socket, endpoints,
        [self](boost::system::error_code const& e,  boost::asio::ip::tcp::endpoint const& chosenEndpoint)
        {
            if(e)
            {
                std::cout << "Failed to connect: " << e.message();
                return;
            }
            std::cout << "A new request from " << self->_session->_socket.remote_endpoint() << " for " << self->_path  << " has been sucessfully forwarded to: " << self->_origin << '\n';
            self->write(self->requestMessage(), self->_socket);
        });
    }); 
   
}

template<typename SocketType>
void Cache::readResponse(SocketType& socket)  
{
    auto self = shared_from_this();
    boost::asio::async_read(socket, responseBuffer,boost::asio::transfer_all(),
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
        port = "443";

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
        return;
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
            _session->write(cachedResponse);
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
        _session->write(fullResponse);
    }
    else
    {
        _session->write(fullResponse);
    }
}


void Cache::sslConnect(boost::asio::ip::tcp::resolver::results_type const& endpoints)
{
    auto self = shared_from_this();
    SSL_set_tlsext_host_name(ssl.native_handle(),_origin.c_str());
    boost::asio::async_connect(ssl.lowest_layer(), endpoints,
    [self](boost::system::error_code const& e, boost::asio::ip::tcp::endpoint const& chosenEndpoint)
    {
        if(e)
        {
            std::cout << "Connection failed: " << e.message();
            return;
        }
        self->ssl.async_handshake(boost::asio::ssl::stream_base::client,
        [self, &chosenEndpoint](boost::system::error_code const& e)
        {
            if(e)
            {
                std::cout << "SSL handshake for: " << chosenEndpoint.address() << " has failed\n";
                return;
            }
        std::cout << self->port;
        std::cout << "A new request from " << self->_session->_socket.remote_endpoint() << " for " << self->_path  << " has been sucessfully forwarded to: " << self->_origin << '\n';
        self->write(self->requestMessage(), self->ssl);
        });
        
    });
}
