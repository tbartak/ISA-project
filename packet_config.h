/**
 * @file packet_config.h
 * @author Tomáš Barták (xbarta51)
 */

#ifndef PACKET_CONFIG_H
#define PACKET_CONFIG_H

#include <string>
#include "packet_info.h"
#include <vector>
#include <unordered_map>
#include <algorithm> // std::find

/**
 * @brief Class for configuration and storing packets.
 * 
 */
class PacketConfig {
    public:
        // Constructor
        PacketConfig();

        // Methods
        void rx_tx(Packet &packet, const std::vector<std::string> &local_ips);
        void add_packet(std::string key, Packet packet);
        void clear_packet_table();

        // Getter
        std::unordered_map<std::string, Packet>& getPacketTable();

    private:
        std::unordered_map<std::string, Packet> packet_table; // all captured packets will be stored in a hash map
};

#endif // PACKET_CONFIG_H