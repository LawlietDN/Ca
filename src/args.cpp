#include "args.h"

namespace args
{
    bool isValidURL(std::string const& url)
    {
    const std::regex url_regex(R"((https?)://[^\s/$.?#].[^\s]*)");
    return std::regex_match(url, url_regex);
}
    bool check(int argc, char* argv[], uint16_t& port, std::string& origin)
    {
        boost::program_options::options_description description("Usage:"); 
         description.add_options()
        ("port, p", boost::program_options::value<uint16_t>()->required(), "The port number on which the caching proxy server will run(e.g.,12345).")
        ("origin, p", boost::program_options::value<std::string>()->required(), "The URL of the server to which the requests will be forwarded.(e.g.,http://dummyjson.com).");
        boost::program_options::variables_map variablesMap;

    try
    {

        boost::program_options::store( boost::program_options::parse_command_line(argc, argv, description), variablesMap);
        
        if (!variablesMap.count("port"))
        {
             /** Boost's notiy function validates all required arguments but doesn't enforce an order by default.
             *   Hence why the program kept throwing an error indicating that --origin is required when no
             *   arguments are passed, when in fact --port is required if we follow the required arguments order.*/
            std::cerr << "Error: --port option is required.\n";
            std::cout << description;   
            return false;
        }
        
        boost::program_options::notify(variablesMap);
        if(!isValidURL(origin))
        {
            std::cerr << "Error: Invalid URL\n";
            std::cout << description;
            return false;
        }
       

    }
    catch(boost::program_options::error const& e)
    {
        std::cerr << "Error: " << e.what() << "\n";
        std::cout << description << "\n";
        return false;
    }
    
        port = variablesMap["port"].as<uint16_t>();
        origin = variablesMap["origin"].as<std::string>();
    return true;
    }
       
};