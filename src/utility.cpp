/**
 * @file utility.cpp
 * @author Tomáš Barták (xbarta51)
 */

#include "utility.h"

/**
 * @brief Constructor for a new Utility object.
 * 
 */
Utility::Utility() {}

/**
 * @brief Destructor for the Utility object.
 * 
 */
Utility::~Utility() {}

/**
 * @brief Function that gets all local IP addresses of the current machine.
 * @return vector of strings with all local IP addresses.
 * 
 * Based on the code from: https://stackoverflow.com/questions/212528/how-can-i-get-the-ip-address-of-a-linux-machine
 * and on: https://dev.to/fmtweisszwerg/cc-how-to-get-all-interface-addresses-on-the-local-device-3pki
 */
std::vector<std::string> Utility::get_local_ips(/*std::string &interface_name*/) {
    std::vector<std::string> local_ips;
    struct ifaddrs *ifaddr, *ifa;
    char ip[INET6_ADDRSTRLEN]; // enough for both IPv4 and IPv6 addresses

    // Get all network interfaces
    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        return local_ips;
    }

    // Loop through the interfaces and collect IP addresses
    for (ifa = ifaddr; ifa != nullptr; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr != nullptr)
        {
            int family = ifa->ifa_addr->sa_family;

            // Handle IPv4 addresses
            if (family == AF_INET) {
                struct sockaddr_in *sa = (struct sockaddr_in *)ifa->ifa_addr;
                inet_ntop(AF_INET, &(sa->sin_addr), ip, INET_ADDRSTRLEN);
                local_ips.push_back(std::string(ip));
            }
            // Handle IPv6 addresses
            else if (family == AF_INET6) {
                struct sockaddr_in6 *sa = (struct sockaddr_in6 *)ifa->ifa_addr;
                inet_ntop(AF_INET6, &(sa->sin6_addr), ip, INET6_ADDRSTRLEN);
                local_ips.push_back(std::string(ip));
            }
        }
    }

    freeifaddrs(ifaddr);
    return local_ips;
}

/**
 * @brief Function that clears the data transmitted and received.
 * 
 */
void Utility::clear_data()
{
    for (auto &packet : packet_config.getPacketTable())
    {
        packet.second.setLength(0);
        packet.second.setRx(0);
        packet.second.setTx(0);
        packet.second.setRxPacketCount(0);
        packet.second.setTxPacketCount(0);
        packet.second.setPacketCount(0);
    }

    // clear table of top connections
    sorting.clear_top_connections();
}

/**
 * @brief Function that clears the hash map with all packets.
 * 
 */
void Utility::clear_packets()
{
    packet_config.clear_packet_table();
}

/**
 * @brief Function that handles the signal from the user (Ctrl+C) and sets stop flag to true.
 * 
 */
void Utility::signal_handler(int signal)
{
    if (signal == SIGINT)
    {
        stop_flag.store(true);
        pcap_breakloop(global_handle);
    }
}

/**
 * @brief Function that converts the data amount to a more readable format.
 * 
 * @param data_amount data amount that is converted.
 * @return string with the converted data amount.
 * 
 */
std::string Utility::convert_data_amount(double data_amount)
{
    // constants for data amounts
    double K = 1024;
    double M = 1024 * K;
    double G = 1024 * M;

    std::ostringstream data; 
    double result; 
    data << std::fixed << std::setprecision(1); // set precision to 1 decimal place

    if (data_amount < K)
    {
        result = data_amount;
        data << result << " ";
    }
    else if (data_amount >= K && data_amount < M)
    {
        result = data_amount / K;
        data << result << " K";
    }
    else if (data_amount >= M && data_amount < G)
    {
        result = data_amount / M;
        data << result << " M";
    }
    else
    {
        result = data_amount / G;
        data << result << " G";
    }
    return data.str();
}