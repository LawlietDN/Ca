#include "args.h"

namespace args
{
    bool check(int argc, char* argv[])
    {
        std::vector<std::string> argsVector = rawToVector(argc, argv);
        if(argc < 2)
        {
            std::cout << "No arguments provided.\n";
            std::cout << "Usage:\nca --port [PORT] --origin [URL]\n";
            return false;
        }
        // else if(argc < 4)
        // {
        //     std::cout << "Some arguments are missing\n";
        //     std::cout << "Usage:\nca --port [PORT] --origin [URL]\n";
        //     return false;
        // }
 
        return isArgsValid(argsVector);
    }
    
    bool isArgsValid(std::vector<std::string>& args)
        {
            for(int i = 0; i < args.size(); i++)
            {
                if(args[i] == "--port")
                {
                    if(!checkPort(args[i + 1]))
                    {
                        return false;
                    }
                }
            }
            return true;
        }

    std::vector<std::string> rawToVector(int argc, char* argv[])
    {
        std::vector<std::string> args;
        for(int i = 0; i < argc; i++)
        {
            args.emplace_back(argv[i]);
        }
        return args;
    }

    bool checkPort(std::string& portArg)
    {
        uint16_t port = 0;
        try
        {
            int portNum = std::stoi(portArg);
            if(portNum < 0 || portNum > 65535)
            {
                std::cout << "Port number is out of valid range.\nValid Range: 0-65535\n";
                return false;
            }
            port = portNum;
        }
        catch(std::exception const& e)
        {
            std::cout << "Invalid port number.\n";
            return false;
        }
            isPortActive(port);
        return true;
    }

        bool isPortActive(uint16_t const& port)
        {

        }
};