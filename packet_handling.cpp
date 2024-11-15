/**
 * @file packet_handling.cpp
 * @author Tomáš Barták (xbarta51)
 */

#include "packet_handling.h"
#include <memory> // std::unique_ptr
#include "utility.h"
#include "globals.h"

/**
 * @brief Constructor for a new Packet Handling object.
 * 
 */
PacketHandling::PacketHandling() {}

/**
 * @brief Destructor for the Packet Handling object.
 * 
 */
PacketHandling::~PacketHandling() {}

/**
 * @brief Function that handles each packet captured.
 * 
 * @param user_data contains the local IP addresses needed for differentiating between incoming and outgoing traffic.
 * @param pkthdr header of the packet.
 * @param packet packet that is captured.
 * 
 * Some parts of the code used from: https://vichargrave.github.io/programming/develop-a-packet-sniffer-with-libpcap/
 */
void PacketHandling::packet_handler(u_char *user_data, const struct pcap_pkthdr* pkthdr, const u_char *packet) {

    if(!user_data) {
        std::cerr << "Error: No user data provided." << std::endl;
        return;
    }

    // Convert user_data to local_ips
    std::vector<std::string> *local_ips = reinterpret_cast<std::vector<std::string>*>(user_data);

    if(!local_ips) {
        std::cerr << "Error: No local IPs provided." << std::endl;
        return;
    }

    // Create a new packet
    std::unique_ptr<Packet> newPacket = std::make_unique<Packet>();

    // Save packet length (in bits)
    newPacket->setLength(pkthdr->len * 8);

    // Extract the Ethernet header
    const struct ether_header *eth_header = (struct ether_header*)packet;

    // Check if the packet is an IPv4 packet
    if (ntohs(eth_header->ether_type) == ETHERTYPE_IP) {
        // Extract the IP header
        const struct ip *ip_header = (struct ip*)(packet + sizeof(struct ether_header)); // IP header is after ethernet header

        // Get and save source and destination IP addresses
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
            // Extract the TCP header and store the ports
            const struct tcphdr* tcp_header = (struct tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
            newPacket->setSrcPort(ntohs(tcp_header->th_sport));
            newPacket->setDstPort(ntohs(tcp_header->th_dport));
        } else if (ip_header->ip_p == IPPROTO_UDP) {
            // Extract the UDP header and store the ports
            const struct udphdr* udp_header = (struct udphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
            newPacket->setSrcPort(ntohs(udp_header->uh_sport));
            newPacket->setDstPort(ntohs(udp_header->uh_dport));
        }
    }
    else if (ntohs(eth_header->ether_type) == ETHERTYPE_IPV6) { // TODO: needs to be tested, seems to be working though
        // Extract the IPv6 header
        const struct ip6_hdr *ip6_header = (struct ip6_hdr*)(packet + sizeof(struct ether_header)); // IPv6 header is after ethernet header
        
        // Get and save source and destination IPv6 addresses
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
            // Extract the TCP header and store the ports
            const struct tcphdr* tcp_header = (struct tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr));
            newPacket->setSrcPort(ntohs(tcp_header->th_sport));
            newPacket->setDstPort(ntohs(tcp_header->th_dport));
        } else if (ip6_header->ip6_nxt == IPPROTO_UDP) {
            // Extract the UDP header and store the ports
            const struct udphdr* udp_header = (struct udphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip6_hdr));
            newPacket->setSrcPort(ntohs(udp_header->uh_sport));
            newPacket->setDstPort(ntohs(udp_header->uh_dport));
        }
    }

        // Differentiate between incoming and outgoing traffic and store the Rx or Tx value
        packet_config.rx_tx(*newPacket, *local_ips);


        // Add packet to the hash map, if it is communicating with the local machine (Rx or Tx is not 0) // TODO: accept packets that are not communicating with the local machine
        if (newPacket->getRx() == 0 && newPacket->getTx() == 0)
        {
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
}

/**
 * @brief Function that will handle packet capture until the user stops the program.
 * 
 * @param config to access the interface name and time
 */
void PacketHandling::packet_capture(Config &config)
{
    std::cout << "Packet Capture Starts..." << std::endl;

    std::string interface = config.getInterface();

    // Get local IP addresses
    Utility utility;
    std::vector<std::string> local_ips = utility.get_local_ips(/*interface*/);

    // Opens the ncurses interface that is displayed in the terminal
    Display display;
    display.print_packets(config.getTime()); // show the header straight away before the first packet is captured and the screen is refreshing automatically
    
    // loop for capturing packets on interface defined by global_handle, looping infinitely until the user stops the program, callback function is packet_handler
    if (pcap_loop(global_handle, 0, PacketHandling::packet_handler, reinterpret_cast<u_char *>(&local_ips)) == -1) {
        std::cerr << "Error capturing packets: " << pcap_geterr(global_handle) << std::endl;
    }
}