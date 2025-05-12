/**
 * @file packet_handling.h
 * @author Tomáš Barták (xbarta51)
 */

#ifndef PACKET_HANDLING_H
#define PACKET_HANDLING_H

#include <string>
#include "packet_info.h"
#include <vector>
#include <unordered_map>
#include <algorithm> // std::find
#include <pcap.h>
#include <netinet/ip6.h> // struct ip6_hdr
#include <netinet/udp.h> // struct udphdr
#include <netinet/tcp.h> // struct tcphdr
#include <netdb.h> // getprotobynumber
#include <iostream>
#include "config.h"
#include "packet_config.h"
#include <netinet/ether.h> // struct ether_header
#include <netinet/ip.h> // struct ip
#include "display.h"
#include <memory> // std::unique_ptr
#include "utility.h"
#include "globals.h"

/**
 * @brief Class for handling packets.
 * 
 */
class PacketHandling 
{
    public:
        // Constructor and destructor
        PacketHandling();
        ~PacketHandling();

        // Methods
        void packet_capture(Config &config);
        static void packet_handler(u_char *user_data, const struct pcap_pkthdr* pkthdr, const u_char *packet);      
};

#endif // PACKET_HANDLING_H