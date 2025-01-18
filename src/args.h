#pragma once
#include <iostream>
#include <vector>
#include <boost/program_options.hpp>
#include <regex>
namespace args
{
    bool check(int argc, char* argv[], uint16_t& port, std::string& origin);
    bool checkPort(std::string& portArg);
    bool isPortActive(uint16_t const& port);
    bool isValidURL(std::string const& url);

};