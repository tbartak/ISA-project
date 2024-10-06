#include <iostream>
//#include <string>
// #include <vector>
#include <getopt.h>
#include <memory> // std::unique_ptr
#include "config.h"

Config::Config() : interface(""), sort('b'), time(1) {}

std::string Config::getInterface() const
{
    return interface;
}

char Config::getSort() const
{
    return sort;
}

int Config::getTime() const
{
    return time;
}

// function that will parse arguments entered using the command line
void Config::parse_args(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "i:s:t:")) != -1) // -i must be defined, -s and -t are optional, if not defined, default values are used (s = 'b', t = 1)
    {
        switch (opt)
        {
        case 'i':
            interface = optarg;
            break;
        case 's':
            if (optarg[0] == 'b')
            {
                sort = 'b';
            }
            else if (optarg[0] == 'p')
            {
                sort = 'p';
            }
            else
            {
                std::cerr << "Invalid argument used after -s. Valid options are 'b' for bytes or 'p' for packets." << std::endl;
                exit(EXIT_FAILURE);
            }
            break;
        case 't':
            time = std::stoi(optarg); // save as integer
            if (time <= 0) // invalid time
            {
                std::cerr << "Invalid argument used after -t. Valid options are only numbers bigger than 0." << std::endl;
                exit(EXIT_FAILURE);
            }
            break;
        default:
            std::cerr << "Usage: " << argv[0] << " -i <interface> [-s <b|p>] [-t <time>]" << std::endl;
            exit(EXIT_FAILURE);
        }
    }

    // Interface must be specified
    if (interface.empty())
    {
        std::cerr << "Network interface (-i) must be specified." << std::endl;
        exit(EXIT_FAILURE);
    }
}