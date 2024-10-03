#include <pcap.h>
#include <iostream>
#include <string>
#include "utility.h"
#include <netinet/ether.h> // struct ether_header
#include <netinet/ip.h> // struct ip
#include <netinet/tcp.h> // struct tcphdr
#include <netinet/udp.h> // struct udphdr
#include <arpa/inet.h> // inet_ntop

// #include <memory> // std::unique_ptr

std::atomic<bool> stop_flag(false);

// all packets will be stored using a hash map
std::unordered_map<std::string, sPacket> packet_table;

// function to add a packet to the hash map
void add_packet(std::string key, sPacket packet)
{
    // if the packet is not already in the hash map, add it, if it is, update the values
    packet_table[key].src_ip = packet.src_ip;
    packet_table[key].src_port = packet.src_port;
    packet_table[key].dst_ip = packet.dst_ip;
    packet_table[key].dst_port = packet.dst_port;
    packet_table[key].protocol = packet.protocol;
    packet_table[key].length += packet.length;
    packet_table[key].packet_count += 1;
    packet_table[key].rx += packet.rx;
    packet_table[key].tx += packet.tx;
    packet_table[key].timestamp = packet.timestamp;
}

// function to clear data transmitted and received
void clear_data()
{
    for (auto &packet : packet_table)
    {
        packet.second.length = 0;
        packet.second.rx = 0;
        packet.second.tx = 0;
    }
}

// function to clear the hash map
void clear_packets()
{
    packet_table.clear();
}

// function to print all packets in the hash map
void print_packets()
{
    for (auto &packet : packet_table)
    {
        if (packet.second.src_port == -1 || packet.second.dst_port == -1)
        {
            std::cout << "Source IP: " << packet.second.src_ip << " Destination IP: " << packet.second.dst_ip << " Protocol: \n" << packet.second.protocol << " Length: " << packet.second.length << " Bytes " << " RX: " << packet.second.rx << " Bytes " << " TX: " << packet.second.tx << " Bytes " << "Count of packets: " << packet.second.packet_count << " Timestamp: " << packet.second.timestamp << std::endl;
        }
        else
        {
        std::cout << "Source IP: " << packet.second.src_ip << " Source Port: " << packet.second.src_port << " Destination IP: " << packet.second.dst_ip << " Destination Port: " << packet.second.dst_port << " Protocol: \n" << packet.second.protocol << " Length: " << packet.second.length << " Bytes " << " RX: " << packet.second.rx << " Bytes " << " TX: " << packet.second.tx << " Bytes " << "Count of packets: " << packet.second.packet_count << " Timestamp: " << packet.second.timestamp << std::endl;
        }
    }
}

// TODO: debug function for printing a single packet
void print_packet(std::string key) 
{
    std::cout << "Source IP: " << packet_table[key].src_ip << " Source Port: " << packet_table[key].src_port << " Destination IP: " << packet_table[key].dst_ip << " Destination Port: " << packet_table[key].dst_port << " Protocol: " << packet_table[key].protocol << " Length: " << packet_table[key].length << " Bytes " << " RX: " << packet_table[key].rx << " Bytes " << " TX: " << packet_table[key].tx << " Bytes " << "Count of packets: " << packet_table[key].packet_count << " Timestamp: " << packet_table[key].timestamp << std::endl;
}

// timer function that will be called every second (or other time specified by -t) to refresh the screen
void timer(int time)
{
    while (!stop_flag.load())
    {
        // TODO: finish timer function
        // wait for the specified time
        std::this_thread::sleep_for(std::chrono::seconds(time));
        // clear screen before printing new data
        std::cout << "-------------------REFRESH HAPPENS NOW!------------------------" << std::endl;
        print_packets();
        // clear_data();
        std::cout << "---------------------------------------------------------------" << std::endl;
    }
}

// function that looks for Ctrl+C and sets the stop flag to true
void signal_handler(int signal)
{
    if (signal == SIGINT)
    {
        std::cout << "\nSIGINT received, stopping capture and cleaning up..." << std::endl;
        stop_flag = true;
    }
}

// function that will shutdown the program gracefully
void shutdown(pcap_t *handle, std::thread &timer_thread)
{
    timer_thread.join();
    close_interface(handle);
    clear_packets();
}


// function that will open the selected interface and exit with an error if the interface is not available
pcap_t *open_interface(std::string &interface)
{
    char errbuf[PCAP_ERRBUF_SIZE];

    pcap_t *handle = pcap_open_live(interface.c_str(), BUFSIZ, 1, 1000, errbuf);
    if (handle == NULL)
    {
        std::cerr << "Couldn't open interface " << interface << ": " << errbuf << std::endl;
        
        // prints all available interfaces
        pcap_if_t *all_interfaces;
        if (pcap_findalldevs(&all_interfaces, errbuf) == -1)
        {
            std::cerr << "Couldn't find any interfaces: " << errbuf << std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout << "Available interfaces:" << std::endl;
        for (pcap_if_t *i = all_interfaces; i != NULL; i = i->next)
        {
            std::cout << i->name << std::endl;
        }
        pcap_freealldevs(all_interfaces);

        exit(EXIT_FAILURE);
    }

    std::cout << "Interface " << interface << " opened." << std::endl;

    return handle;
}

// function that will close the selected interface (TODO: later needs to be called after pressing Ctrl+C, so that no allocated memory is left behind)
void close_interface(pcap_t *handle)
{
    pcap_close(handle);
    std::cout << "Interface closed" << std::endl;
}

// function that will handle each packet captured and print/save results into a structure
void packet_handler(struct pcap_pkthdr* pkthdr, const u_char *packet) {
    std::cout << "Got a packet" << std::endl;

    sPacket *newPacket;

    // create a new packet
    newPacket = new sPacket;

    // Packet length 
    newPacket->length = pkthdr->len;
    std::cout << "Packet length: " << newPacket->length << std::endl;

    // TODO: rx and tx (will depend based on if its incoming or outgoing packet)
    newPacket->rx = 0;
    newPacket->tx = 0;


        // Extract the IP header
        const struct ip *ip_header = (struct ip*)(packet + 14); // IP header is after ethernet header, which is 14 bytes

        // Get source and destination IP addresses
        newPacket->src_ip = inet_ntoa(ip_header->ip_src); // TODO: only works for IPv4, need support for IPv6
        newPacket->dst_ip = inet_ntoa(ip_header->ip_dst); // TODO: only works for IPv4, need support for IPv6

        // Check protocol
        if (ip_header->ip_p == IPPROTO_TCP) {
            // Extract the TCP header
            const struct tcphdr* tcp_header = (struct tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
            newPacket->protocol = "TCP";
            newPacket->src_port = ntohs(tcp_header->th_sport);
            newPacket->dst_port = ntohs(tcp_header->th_dport);
        } else if (ip_header->ip_p == IPPROTO_UDP) {
            const struct udphdr* udp_header = (struct udphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
            newPacket->protocol = "UDP";
            newPacket->src_port = ntohs(udp_header->uh_sport);
            newPacket->dst_port = ntohs(udp_header->uh_dport);
        } 
        
        // TODO: rest of the supported protocols
        else {
            // TODO: for testing purposes, if its unknown protocol, we wont be able to output anything, but for other currently unsupported protocols like ICMP, we will output the correct protocol name
            newPacket->protocol = "Unknown/Unsupported Protocol";
        }

        // Timestamp
        newPacket->timestamp = ctime((const time_t*)&pkthdr->ts.tv_sec);

        // add packet to the hash map // TODO: include ports for more specific key for the exact communication, but ports are now integers and also only optional, so they arent included all the time, but only for protocols like TCP/UDP, ...
        if (newPacket->src_port == -1 && newPacket->dst_port == -1)
        {
            add_packet(newPacket->src_ip /*+ newPacket->src_port*/ + newPacket->dst_ip /*+ newPacket->dst_port*/ + newPacket->protocol, *newPacket);
        }
        else
        {
            add_packet(newPacket->src_ip + std::to_string(newPacket->src_port) + newPacket->dst_ip + std::to_string(newPacket->dst_port) + newPacket->protocol, *newPacket);
        }

        // print current packet
        if (newPacket->src_port == -1 && newPacket->dst_port == -1)
        {
            print_packet(newPacket->src_ip /*+ newPacket->src_port*/ + newPacket->dst_ip /*+ newPacket->dst_port*/ + newPacket->protocol);
        }
        else
        {
            print_packet(newPacket->src_ip + std::to_string(newPacket->src_port) + newPacket->dst_ip + std::to_string(newPacket->dst_port) + newPacket->protocol);
        }

}

// function that will capture packets
void packet_capture(pcap_t *handle)
{
    std::cout << "Packet Capture Starts..." << std::endl;

    struct pcap_pkthdr header;
    const u_char *packet;

    // will capture packets until the program is terminated
    while (!stop_flag.load())
    {
        // blocking/non-blocking?
        packet = pcap_next(handle, &header);
        // packet will be NULL if no packet is captured
        if (packet == NULL)
        {
            continue;
        }

        // signal to shutdown the program received // TODO: but it is sometimes delayed, so it will still capture some packets after the signal is received
        if (stop_flag.load())
        {
            return;
            // shutdown();
        }

        packet_handler(&header, packet);


    }
}