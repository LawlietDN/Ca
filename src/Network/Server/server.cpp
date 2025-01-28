#include "server.h"

void Server::accept()
{
    
    acceptor.async_accept(
        [this](boost::system::error_code const& e, boost::asio::ip::tcp::socket socket)
        {
            if(e && e != boost::asio::error::eof)
            {
                std::cerr << "\nError occured while trying to accept connectiosn: " << e.message() << '\n';
                return;
            }
            std::make_shared<Session>(std::move(socket))->start(getOrigin());
            accept();
        }
    );
}

std::string Server::getOrigin()
{
    return _origin;
}


