#include "args.h"



int main(int argc, char *argv[])
{
    uint16_t port = 0;
    std::string origin;
    if(!args::check(argc, argv, port, origin))
    {
        return 1;
    }
    std::cout << "Port: " << port << "\nOrigin: " << origin;
    
    
    return 0;
   
}