#include <iostream>
//#include <string>
// #include <vector>
#include <getopt.h>
#include <memory> // std::unique_ptr
#include "config.h"

std::unique_ptr<Config> parse_args(int argc, char *argv[])
{
    std::unique_ptr<Config> config = std::make_unique<Config>();
    int opt;
    while ((opt = getopt(argc, argv, "i:s:t:")) != -1) // -i must be defined, -s and -t are optional, if not defined, default values are used (s = 'b', t = 1)
    {
        switch (opt)
        {
        case 'i':
            config->interface = optarg;
            break;
        case 's':
            if (optarg[0] == 'b')
            {
                config->sort = 'b';
            }
            else if (optarg[0] == 'p')
            {
                config->sort = 'p';
            }
            else
            {
                std::cerr << "Invalid argument used after -s. Valid options are 'b' for bytes or 'p' for packets." << std::endl;
                exit(EXIT_FAILURE);
            }
            break;
        case 't':
            config->time = std::stoi(optarg); // save as integer
            if (config->time <= 0) // invalid time
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
    if (config->interface.empty())
    {
        std::cerr << "Network interface (-i) must be specified." << std::endl;
        exit(EXIT_FAILURE);
    }

    return config;
}