#include "../Include/args.h"
#include "../Include/server.h"

int main(int argc, char *argv[])
{
    uint16_t port = 0;
    std::string origin;
    std::string address = "127.0.0.1";
    if(!args::check(argc, argv, port, origin)) return 1;
    
    std::cout << "Port: " << port << "\nOrigin: " << origin << '\n';
    try
    {
        boost::asio::io_context io;
        Server Server(io, port, origin, address);
        io.run();
    }
    catch(std::exception const& e)
    {
        std::cerr << "Error: " << e.what();
    }
    return 0;
   
}