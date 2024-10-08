#ifndef PACKET_CONFIG_H
#define PACKET_CONFIG_H

#include <string>
#include "packet_info.h"
#include <vector>
#include <unordered_map>
#include <algorithm> // std::find

class PacketConfig {
    public:
        PacketConfig();

        void rx_tx(Packet &packet, const std::vector<std::string> &local_ips);
        void add_packet(std::string key, Packet packet);
        void clear_packet_table();

        // getter for packet_table
        std::unordered_map<std::string, Packet>& getPacketTable();

    private:
        std::unordered_map<std::string, Packet> packet_table; // all packets will be stored using a hash map
};

#endif // PACKET_CONFIG_H