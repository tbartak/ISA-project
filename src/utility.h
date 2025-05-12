/**
 * @file utility.h
 * @author Tomáš Barták (xbarta51)
 */

#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <string>
#include <signal.h> // signal handling
#include "config.h"
#include <vector>
#include <netinet/ip6.h> // struct ip6_hdr
#include <ifaddrs.h> // getifaddrs
#include <sstream> // std::stringstream
#include <iomanip> // std::setprecision
#include "packet_info.h"
#include "globals.h"
#include <arpa/inet.h> // inet_ntop
#include "network_interface.h"
#include "packet_config.h"

/**
 * @brief Class for Utility methods.
 * 
 */
class Utility {
    public:
        // Constructor
        Utility();

        //Destructor
        ~Utility();

        // Methods
        static std::vector<std::string> get_local_ips();
        static void clear_data();
        static void clear_packets();
        static void signal_handler(int signal);
        static std::string convert_data_amount(double data_amount);
};

#endif // UTILITY_H