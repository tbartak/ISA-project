/**
 * @file packet_config.cpp
 * @author Tomáš Barták (xbarta51)
 */

#include "packet_config.h"

/**
 * @brief Constructor for a new Packet Config object
 * 
 */
PacketConfig::PacketConfig() {}

/**
 * @brief Destructor for the Packet Config object
 * 
 */
PacketConfig::~PacketConfig() {}

/**
 * @brief Getter for a hash map that stores all packets.
 * 
 * @return hash map with all packets captured by the program.
 */
std::unordered_map<std::string, Packet>& PacketConfig::getPacketTable() { return packet_table; }

/**
 * @brief Function that clears the hash map with all packets.
 * 
 */
void PacketConfig::clear_packet_table()
{
    packet_table.clear();
}

/**
 * @brief Function that adds a packet to the hash map.
 * 
 * @param key key of the packet that is used to save it into the hash map
 * @param packet the packet that is stored into the hash map
 */
void PacketConfig::add_packet(std::string key, Packet packet)
{
    packet_table[key].setSrcIp(packet.getSrcIp());
    packet_table[key].setSrcPort(packet.getSrcPort());
    packet_table[key].setDstIp(packet.getDstIp());
    packet_table[key].setDstPort(packet.getDstPort());
    packet_table[key].setProtocol(packet.getProtocol());
    packet_table[key].addLength(packet.getLength()); // add length of the packet to the current length
    packet_table[key].addPacketCount(); // increment packet count
    packet_table[key].addRxPacketCount(packet.getRxPacketCount()); // increment received packet count if its incoming (rx_packet_count will be set to 1, if its incoming and to 0, if its outgoing)
    packet_table[key].addTxPacketCount(packet.getTxPacketCount()); // increment transmitted packet count if its outgoing (tx_packet_count will be set to 1, if its outgoing and to 0, if its incoming)
    packet_table[key].addRx(packet.getRx()); // add received bytes to the current received bytes
    packet_table[key].addTx(packet.getTx()); // add transmitted bytes to the current transmitted bytes
}

/**
 * @brief Function that differentiates between incoming and outgoing traffic.
 * 
 * @param packet that is currently processed
 * @param local_ips all local IP addresses found on current machine
 */
void PacketConfig::rx_tx(Packet &packet, const std::vector<std::string> &local_ips)
{
    if (std::find(local_ips.begin(), local_ips.end(), packet.getDstIp()) != local_ips.end()) // look whether any of the local IPs match the destination IP, then its incoming (Rx)
    {
        // swap the source and destination IP addresses, when the packet is incoming, so it combines the incoming and outgoing traffic into one connection
        std::string temp_src_ip = packet.getSrcIp();
        std::string temp_dst_ip = packet.getDstIp();
        packet.setSrcIp(temp_dst_ip);
        packet.setDstIp(temp_src_ip);
        packet.setRx(packet.getLength());
        packet.setRxPacketCount(1); // set the count of received packets
    }
    else // consider everything else as outgoing (Tx)
    {
        packet.setTx(packet.getLength());
        packet.setTxPacketCount(1); // set the count of transmitted packets
    }
}