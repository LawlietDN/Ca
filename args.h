#pragma once
#include <iostream>
#include <vector>
namespace args
{
    bool check(int argc, char* argv[]);
    bool isArgsValid(std::vector<std::string>& args);
    std::vector<std::string> rawToVector(int argc, char* argv[]);
    bool checkPort(std::string& portArg);
    bool isPortActive(uint16_t const& port);
};