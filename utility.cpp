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


PacketConfig packet_config;

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

pcap_t *global_handle;

std::vector<Packet> top_connections;

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
    top_connections.clear();
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
//     for (auto &packet : packet_table)
//     {
//         if (packet.second.src_port == -1 || packet.second.dst_port == -1)
//         {
//             std::cout << "Source IP: " << packet.second.src_ip << " Destination IP: " << packet.second.dst_ip << " Protocol: \n" << packet.second.protocol << " Length: " << packet.second.length << " Bytes " << " RX: " << packet.second.rx << " Bytes " << " TX: " << packet.second.tx << " Bytes " << "Count of packets: " << packet.second.packet_count << " Timestamp: " << packet.second.timestamp << std::endl;
//         }
//         else
//         {
//         std::cout << "Source IP: " << packet.second.src_ip << " Source Port: " << packet.second.src_port << " Destination IP: " << packet.second.dst_ip << " Destination Port: " << packet.second.dst_port << " Protocol: \n" << packet.second.protocol << " Length: " << packet.second.length << " Bytes " << " RX: " << packet.second.rx << " Bytes " << " TX: " << packet.second.tx << " Bytes " << "Count of packets: " << packet.second.packet_count << " Timestamp: " << packet.second.timestamp << std::endl;
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


    for (auto &packet : top_connections)
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
        search_most_traffic();
        sort_most_traffic(top_connections, sort);
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

// function that will search for the top 10 connections with most traffic
void search_most_traffic()
{
    auto packet_table = packet_config.getPacketTable();
    for (auto &packet : packet_table)
    {
        create_most_traffic_array(top_connections, packet.second);
    }
}

// function that will create a table of top 10 connections with most traffic
void create_most_traffic_array(std::vector<Packet> &top_connections, Packet packet)
{
    // if the packet doesnt have any traffic, dont add it to the table (no rx or tx) // this will prevent packets that are not communicating with the local machine to be displayed
    // TODO: in case we want to save the packets that are not communicating with the local machine to the structure, but not the table, use this instead of the return in packet_handler
    // if (packet.rx == 0 && packet.tx == 0)
    // {
    //     return;
    // }
    
    // if the table is empty, add the first packet
    if (top_connections.empty())
    {
        top_connections.push_back(packet);
    }
    else
    {
        // if the table is not full yet, add the packet
        if (top_connections.size() < 10)
        {
            top_connections.push_back(packet);
        }
        else
        {
            // if the table is full, check if the packet has more traffic than the smallest packet in the table
            for (auto &current_packet : top_connections)
            {
                if (packet.getLength() > current_packet.getLength())
                {
                    current_packet = packet;
                    break;
                }
            }
        }
    }
}

// function that will sort the top 10 connections with most traffic based on the -s argument (bytes or packets)
void sort_most_traffic(std::vector<Packet> &top_connections, char sort_by)
{
    if (sort_by == 'b')
    {
        std::sort(top_connections.begin(), top_connections.end(), [](Packet a, Packet b) { return ((a.getRx() + a.getTx()) > (b.getRx() + b.getTx())); });
    }
    else if (sort_by == 'p')
    {
        std::sort(top_connections.begin(), top_connections.end(), [](Packet a, Packet b) { return a.getPacketCount() > b.getPacketCount(); });
    }
    else
    {
        std::cerr << "Invalid argument for sorting the top connections." << std::endl;
        exit(EXIT_FAILURE);
    }
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




// function that will handle each packet captured and print/save results into a structure
void packet_handler(u_char *user_data, const struct pcap_pkthdr* pkthdr, const u_char *packet) {

    // convert user_data to local_ips
    std::vector<std::string> *local_ips = reinterpret_cast<std::vector<std::string>*>(user_data);

    // // create a new packet
    std::unique_ptr<Packet> newPacket = std::make_unique<Packet>();

    // Packet length 
    newPacket->setLength(pkthdr->len);

    // Extract the Ethernet header
    const struct ether_header *eth_header = (struct ether_header*)packet;

    // Check if the packet is an IPv4 packet
    if (ntohs(eth_header->ether_type) == ETHERTYPE_IP) {
        // Extract the IP header
        const struct ip *ip_header = (struct ip*)(packet + sizeof(struct ether_header)); // IP header is after ethernet header

        // Get source and destination IP addresses
        newPacket->setSrcIp(inet_ntoa(ip_header->ip_src));
        newPacket->setDstIp(inet_ntoa(ip_header->ip_dst));

        // Check protocol
        struct protoent *packet_protocol = getprotobynumber(ip_header->ip_p);
        if (packet_protocol)
        {
            newPacket->setProtocol(packet_protocol->p_name);
        }
        else
        {
            newPacket->setProtocol("Unknown");
        }
        
        if (ip_header->ip_p == IPPROTO_TCP) {
            // Extract the TCP header
            const struct tcphdr* tcp_header = (struct tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
            newPacket->setSrcPort(ntohs(tcp_header->th_sport));
            newPacket->setDstPort(ntohs(tcp_header->th_dport));
        } else if (ip_header->ip_p == IPPROTO_UDP) {
            const struct udphdr* udp_header = (struct udphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
            newPacket->setSrcPort(ntohs(udp_header->uh_sport));
            newPacket->setDstPort(ntohs(udp_header->uh_dport));
        }
        // TODO: rest of the protocols with specified ports in headers
    }
    else if (ntohs(eth_header->ether_type) == ETHERTYPE_IPV6) { // TODO: needs to be tested, seems to be working though
        // Extract the IPv6 header
        const struct ip6_hdr *ip6_header = (struct ip6_hdr*)(packet + sizeof(struct ether_header)); // IPv6 header is after ethernet header, which is 14 bytes
        
        char src_ip[INET6_ADDRSTRLEN];
        char dst_ip[INET6_ADDRSTRLEN];
        
        inet_ntop(AF_INET6, &(ip6_header->ip6_src), src_ip, INET6_ADDRSTRLEN);
        inet_ntop(AF_INET6, &(ip6_header->ip6_dst), dst_ip, INET6_ADDRSTRLEN);

        newPacket->setSrcIp(src_ip);
        newPacket->setDstIp(dst_ip);

        // Check protocol
        struct protoent *packet_protocol = getprotobynumber(ip6_header->ip6_nxt);
        if (packet_protocol)
        {
            newPacket->setProtocol(packet_protocol->p_name);
        }
        else
        {
            newPacket->setProtocol("Unknown");
        }

        // Check protocol
        if (ip6_header->ip6_nxt == IPPROTO_TCP) {
            // Extract the TCP header
            // const struct tcphdr* tcp_header = (struct tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
            const struct tcphdr* tcp_header = (struct tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr));
            newPacket->setSrcPort(ntohs(tcp_header->th_sport));
            newPacket->setDstPort(ntohs(tcp_header->th_dport));
        } else if (ip6_header->ip6_nxt == IPPROTO_UDP) {
            // const struct udphdr* udp_header = (struct udphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
            const struct udphdr* udp_header = (struct udphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr));
            newPacket->setSrcPort(ntohs(udp_header->uh_sport));
            newPacket->setDstPort(ntohs(udp_header->uh_dport));
        }
        // TODO: rest of protocols with specified ports in headers
    }

        // differentiate between incoming and outgoing traffic
        packet_config.rx_tx(*newPacket, *local_ips);


        // add packet to the hash map // TODO: include ports for more specific key for the exact communication, but ports are now integers and also only optional, so they arent included all the time, but only for protocols like TCP/UDP, ...
        // only add the packets if they communicate with the local machine, that means that rx_tx function has set rx or tx to a value
        if (newPacket->getRx() == 0 && newPacket->getTx() == 0)
        {
            // TODO: testing purposes, if the packet is not communicating with the local machine, show the packet, but dont add it to the table
            // std::cout << "Source IP: " << newPacket->getSrcIp() << " Source Port: " << newPacket->getSrcPort() << " Destination IP: " << newPacket->getDstIp() << " Destination Port: " << newPacket->getDstPort() << " Protocol: " << newPacket->getProtocol() << " Length: " << newPacket->getLength() << " Bytes " << "Count of packets: " << newPacket->getPacketCount() << std::endl;
            return;
        }
        if (newPacket->getSrcPort() == -1 && newPacket->getDstPort() == -1)
        {
            packet_config.add_packet(newPacket->getSrcIp() + newPacket->getDstIp() + newPacket->getProtocol(), *newPacket);
        }
        else
        {
            packet_config.add_packet(newPacket->getSrcIp() + std::to_string(newPacket->getSrcPort()) + newPacket->getDstIp() + std::to_string(newPacket->getDstPort()) + newPacket->getProtocol(), *newPacket);
        }

        // print current packet // TODO: for testing purposes, will be removed later
        // if (newPacket->src_port == -1 && newPacket->dst_port == -1)
        // {
        //     print_packet(newPacket->src_ip /*+ newPacket->src_port*/ + newPacket->dst_ip /*+ newPacket->dst_port*/ + newPacket->protocol);
        // }
        // else
        // {
        //     print_packet(newPacket->src_ip + std::to_string(newPacket->src_port) + newPacket->dst_ip + std::to_string(newPacket->dst_port) + newPacket->protocol);
        // }
}

// function that will capture packets
void packet_capture(pcap_t *handle, Config &config)
{
    std::cout << "Packet Capture Starts..." << std::endl;

    global_handle = handle;

    std::string interface = config.getInterface();

    std::vector<std::string> local_ips = get_local_ips(/*interface*/);

    // opens the interface that is displayed in the terminal
    print_packets(config.getTime());

    // Pass the local_ips vector to the packet_handler as user data
    if (pcap_loop(handle, 0, packet_handler, (u_char *)&local_ips) == -1) {
        std::cerr << "Error capturing packets: " << pcap_geterr(handle) << std::endl;
        return;
    }
}