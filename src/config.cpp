/**
 * @file config.cpp
 * @author Tomáš Barták (xbarta51)
 */

#include "config.h"

/**
 * @brief Constructor for a Config object with default values.
 * @return Config object with default values.
 */
Config::Config() : interface(""), sort('b'), time(1) {}

/**
 * @brief Destructor for the Config object.
 */
Config::~Config() {}

/**
 * @brief Getter method for interface.
 * 
 * @return name of the selected interface.
 */
std::string Config::getInterface() const
{
    return interface;
}

/**
 * @brief Getter method for sort.
 * 
 * @return sort type ('b' for bytes, 'p' for packets).
 */
char Config::getSort() const
{
    return sort;
}

/**
 * @brief Getter method for time interval for refreshing statistics.
 * 
 * @return time interval set by the user (in seconds).
 */
int Config::getTime() const
{
    return time;
}

/**
 * @brief function that handles the parsing of arguments from the command line.
 * 
 * @param argc count of arguments passed from the command line.
 * @param argv array of arguments passed from the command line.
 */
void Config::parse_args(int argc, char *argv[])
{
    int opt;
    while ((opt = getopt(argc, argv, "hi:s:t:")) != -1) // -i must be defined, -s and -t are optional, if not defined, default values are used (s = 'b', t = 1)
    {
        switch (opt)
        {
        case 'h': // help
            std::cout << "Usage: " << argv[0] << " -i <interface> [-s <b|p>] [-t <time>]" << std::endl;
            exit(EXIT_SUCCESS);
            break;
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
            else // invalid sort option
            {
                std::cerr << "Invalid argument used after -s. Valid options are either 'b' for bytes or 'p' for packets." << std::endl;
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
        default: // invalid argument
            std::cerr << "Usage: " << argv[0] << " -i <interface> [-s <b|p>] [-t <time>]" << std::endl;
            exit(EXIT_FAILURE);
        }
    }
}