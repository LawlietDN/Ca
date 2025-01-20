#include "session.h"

class Server
{
private:
    boost::asio::io_context& io;
    boost::asio::ip::tcp::acceptor acceptor;
    void accept();

public:
    explicit Server(boost::asio::io_context& ioContext, uint16_t const& port, std::string const& origin, std::string const& address)
    :   io(ioContext),
        acceptor(io, boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(address), port))
        {
            std::cout << "Listening on: " << address << ":" << port << '\n';
            accept();
        }

};