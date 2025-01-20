#include "../Include/server.h"

void Server::accept()
{
    
    acceptor.async_accept(
        [this](boost::system::error_code const& e, boost::asio::ip::tcp::socket socket)
        {
            if(e)
            {
                std::cerr << "\nError occured while trying to accept connectiosn: " << e.message();
            }
            std::cout << "\nA connection has been accepted from: " << socket.remote_endpoint() << '\n';
            std::make_shared<Session>(std::move(socket))->start();
            accept();
        }
    );
}

