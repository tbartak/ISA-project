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
// #include "globals.h"


// PacketConfig packet_config;

Sorting sorting;

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

// // function to print all packets in the hash map // TODO: for testing purposes, will be removed later
// void print_packets(int time)
// {
//     (void)time; // unused variable
//     for (auto &packet : packet_config.getPacketTable())
//     {
//         if (packet.second.getSrcPort() == -1 || packet.second.getDstPort() == -1)
//         {
//             std::cout << "Source IP: " << packet.second.getSrcIp() << " Destination IP: " << packet.second.getDstIp() << " Protocol: \n" << packet.second.getProtocol() << " Length: " << packet.second.getLength() << " Bytes " << " RX: " << packet.second.getRx() << " Bytes " << " TX: " << packet.second.getTx() << " Bytes " << "Count of packets: " << packet.second.getPacketCount() << std::endl;
//         }
//         else
//         {
//         std::cout << "Source IP: " << packet.second.getSrcIp() << " Source Port: " << packet.second.getSrcPort() << " Destination IP: " << packet.second.getDstIp() << " Destination Port: " << packet.second.getDstPort() << " Protocol: \n" << packet.second.getProtocol() << " Length: " << packet.second.getLength() << " Bytes " << " RX: " << packet.second.getRx() << " Bytes " << " TX: " << packet.second.getTx() << " Bytes " << "Count of packets: " << packet.second.getPacketCount() << std::endl;
//         }
//     }
// }

// function to print all packets in the hash map using ncurses
void print_packets(int time)
{
    clear(); // clear the screen

    double interval = (double)time; // interval in seconds

    int row = 0;

    // header of the table
    mvprintw(row, SRC_IP_COL, "Src IP:port");
    mvprintw(row, DST_IP_COL, "Dst IP:port");
    mvprintw(row, PROTOCOL_COL, "Proto");
    mvprintw(row, LENGTH_COL, "Length");
    mvprintw(row, RX_COL, "RX");
    mvprintw(row, TX_COL, "TX");
    mvprintw(row, PACKET_COUNT_COL, "Packet Count");

    row++;

    // Sorting sorting;
    for (auto &packet : sorting.getTopConnections())
    {        
        // print only packets that have some transmission in the last second // TODO: not needed anymore, since the packets that are not communicating with the local machine are not added to the table
        if (packet.getLength() != 0)
        {      
            if (packet.getSrcPort() == -1 || packet.getDstPort() == -1)
            {
                mvprintw(row, SRC_IP_COL, "%s", packet.getSrcIp().c_str());
                mvprintw(row, DST_IP_COL, "%s", packet.getDstIp().c_str());
                mvprintw(row, PROTOCOL_COL, "%s", packet.getProtocol().c_str());
                mvprintw(row, LENGTH_COL, "%sB/s", convert_data_amount(packet.getLength() / interval).c_str());
                mvprintw(row, RX_COL, "%sB/s", convert_data_amount(packet.getRx() / interval).c_str());
                mvprintw(row, TX_COL, "%sB/s", convert_data_amount(packet.getTx() / interval).c_str());
                mvprintw(row, PACKET_COUNT_COL, "%sp/s", convert_data_amount(packet.getPacketCount() / interval).c_str());
            }
            else
            {
                // add port to the IP address of the source and destination
                mvprintw(row, SRC_IP_COL, "%s:%d", packet.getSrcIp().c_str(), packet.getSrcPort());
                mvprintw(row, DST_IP_COL, "%s:%d", packet.getDstIp().c_str(), packet.getDstPort());
                mvprintw(row, PROTOCOL_COL, "%s", packet.getProtocol().c_str());
                mvprintw(row, LENGTH_COL, "%sB/s", convert_data_amount(packet.getLength() / interval).c_str());
                mvprintw(row, RX_COL, "%sB/s", convert_data_amount(packet.getRx() / interval).c_str());
                mvprintw(row, TX_COL, "%sB/s", convert_data_amount(packet.getTx() / interval).c_str());
                mvprintw(row, PACKET_COUNT_COL, "%sp/s", convert_data_amount(packet.getPacketCount() / interval).c_str());
            }
            row++;
        }
    }
    mvprintw(20, 0, "Press Ctrl+C to stop the program.");
    refresh();
}

// TODO: debug function for printing a single packet
void print_packet(std::string key) 
{
    const auto &packet_table = packet_config.getPacketTable(); // TODO: ended here
    auto packet = packet_table.at(key);
    std::cout << "Source IP: " << packet.getSrcIp() << " Source Port: " << packet.getSrcPort() << " Destination IP: " << packet.getDstIp() << " Destination Port: " << packet.getDstPort() << " Protocol: " << packet.getProtocol() << " Length: " << packet.getLength() << " Bytes " << " RX: " << packet.getRx() << " Bytes " << " TX: " << packet.getTx() << " Bytes " << "Count of packets: " << packet.getPacketCount() << std::endl;
}

// timer function that will be called every second (or other time specified by -t) to refresh the screen
void timer(int time, char sort)
{
    while (!stop_flag.load())
    {
        // TODO: finish timer function
        // wait for the specified time
        // could optimize, lets say that it will always sleep for 1 second, but if the time was set to 10 seconds, it will repeat the sleep 9 more times
        for (int i = 0; i < time; i++)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (stop_flag.load()) // if the stop flag is set, stop the timer -> close the program
            {
                return;
            }
        }
        // std::this_thread::sleep_for(std::chrono::seconds(time)); // when closing using Ctrl+C, this will still wait for the period to finish, even though it wont receive any more data from the packets
        // Sorting sorting;
        sorting.search_most_traffic();
        sorting.sort_most_traffic(sort);
        print_packets(time); // to calculate the bandwidth, we pass the time set by the user to the function
        clear_data(); // TODO: or clear_packets()? to clear all data of the packets? could be better for memory management
    }
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