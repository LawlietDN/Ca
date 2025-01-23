#include "args.h"

namespace args
{
    bool isValidURL(std::string const& url, boost::program_options::options_description const& description)
    {
     const std::regex urlRregex(R"((https?)://[^\s/$.?#].[^\s]*)");
     if(std::regex_match(url, urlRregex)) return true;

     std::cerr << "Invalid URL\n";
     std::cerr << description;
     return false;
    }


    bool checkPort(uint16_t const& port, boost::program_options::options_description const& description)
    {
        if(port < 1024 || port > 65535)
        {
            std::cerr << "Error: Port numebr must be between 1024 and 65535\n";
            std::cerr << description;
            return false;
        }
        return true;
    }


    bool check(int argc, char* argv[], uint16_t& port, std::string& origin)
    {
         boost::program_options::options_description description("Usage"); 
         description.add_options()
        ("port, --p", boost::program_options::value<uint16_t>(&port)->required(), "The port number on which the proxy server will run(e.g., 12345).")
        ("origin, --o", boost::program_options::value<std::string>(&origin)->required(), "The URL of the server to which the requests will be forwarded.(e.g., https://example.com).")
        ("clear-cache", "To clear the cached responses from the server");
        boost::program_options::variables_map variablesMap;

    try
    {

        boost::program_options::store(boost::program_options::parse_command_line(argc, argv, description), variablesMap);
        // if(variablesMap.count("clear-cache"))
        // {
        //     std::cout << "Clearing the cache JSON...\n"; //Logic being implemented soon.
        //     return true;
        // }
        if (!variablesMap.count("port"))
        {
             /** Boost's notiy function validates all required arguments but doesn't enforce an order by default.
             *   Hence why the program kept throwing an error indicating that --origin is required when no
             *   arguments are passed, when in fact --port is required if we follow the required arguments order.
             *   Hence why we need to do a manual check for the port argument to avoid confusion. */
            std::cerr << "Error: --port option is required.\n";
            std::cerr << description;   
            return false;
        }
        
        boost::program_options::notify(variablesMap);
      

    }
    catch(boost::program_options::error const& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        std::cerr << description << "\n";
        return false;
    }
        if(!isValidURL(origin, description)) return false;
        if(!checkPort(port, description)) return false;
    return true;
    }
       
};