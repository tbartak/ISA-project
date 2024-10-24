/**
 * @file utility.cpp
 * @author Tomáš Barták (xbarta51)
 */

#include <pcap.h>
#include <iostream>
#include <string>
#include "utility.h"
#include <netinet/ether.h> // struct ether_header
#include <netinet/ip.h> // struct ip
#include <netinet/tcp.h> // struct tcphdr
#include <netinet/udp.h> // struct udphdr
#include <arpa/inet.h> // inet_ntop
#include "packet_info.h"
#include "network_interface.h"
#include "packet_config.h"
#include "globals.h"

// columns widths for ncurses
#define SRC_IP_COL 0
#define DST_IP_COL 35
#define PROTOCOL_COL 75
#define LENGTH_COL 90
#define RX_COL 105
#define TX_COL 120
#define PACKET_COUNT_COL 135
#define TIMESTAMP_COL 150

std::atomic<bool> stop_flag(false);

// Function to get all local IP addresses
std::vector<std::string> get_local_ips(/*std::string &interface_name*/) {
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
        if (ifa->ifa_addr == nullptr) {
            continue;
        }        

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

    freeifaddrs(ifaddr);
    return local_ips;
}



// function to clear data transmitted and received
void clear_data()
{
    for (auto &packet : packet_config.getPacketTable())
    {
        packet.second.setLength(0);
        packet.second.setRx(0);
        packet.second.setTx(0);
        packet.second.setPacketCount(0);
    }

    // clear table of top connections
    sorting.clear_top_connections();
}

// function to clear the hash map
void clear_packets()
{
    packet_config.clear_packet_table();
}

// function that looks for Ctrl+C and sets the stop flag to true
void signal_handler(int signal)
{
    if (signal == SIGINT)
    {
        // std::cout << "\nSIGINT received, stopping capture and cleaning up..." << std::endl;
        // stop_flag = true;
        stop_flag.store(true);
        pcap_breakloop(global_handle);
    }
}

// function that will shutdown the program gracefully
void shutdown(std::thread &timer_thread)
{
    NetworkInterface network_interface;
    timer_thread.join();
    network_interface.close_interface();
    clear_packets();
}

// function that converts the data amount to more readable format
std::string convert_data_amount(double data_amount)
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