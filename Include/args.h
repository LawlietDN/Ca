#pragma once
#include <iostream>
#include <boost/program_options.hpp>
#include <regex>

namespace args
{
    bool check(int argc, char* argv[], uint16_t& port, std::string& origin);
    bool checkPort(uint16_t const& port, boost::program_options::options_description const& description);
    bool isValidURL(std::string const& url, boost::program_options::options_description const& description);

};