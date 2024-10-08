#include "packet_config.h"

// constructor
PacketConfig::PacketConfig() {}

// getter for packet_table
std::unordered_map<std::string, Packet>& PacketConfig::getPacketTable() { return packet_table; }

// function to clear the hash map
void PacketConfig::clear_packet_table()
{
    packet_table.clear();
}

// function to add a packet to the hash map
void PacketConfig::add_packet(std::string key, Packet packet)
{
    // if the packet is not already in the hash map, add it, if it is, update the values
    packet_table[key].setSrcIp(packet.getSrcIp());
    packet_table[key].setSrcPort(packet.getSrcPort());
    packet_table[key].setDstIp(packet.getDstIp());
    packet_table[key].setDstPort(packet.getDstPort());
    packet_table[key].setProtocol(packet.getProtocol());
    packet_table[key].addLength(packet.getLength());
    packet_table[key].addPacketCount(); // increment packet count
    packet_table[key].addRx(packet.getRx());
    packet_table[key].addTx(packet.getTx());
}

// function that will differentiate between incoming and outgoing traffic
void PacketConfig::rx_tx(Packet &packet, const std::vector<std::string> &local_ips)
{
    if (std::find(local_ips.begin(), local_ips.end(), packet.getSrcIp()) != local_ips.end())
    {
        // packet.rx = packet.length;
        packet.setRx(packet.getLength());
    }
    else if (std::find(local_ips.begin(), local_ips.end(), packet.getDstIp()) != local_ips.end())
    {
        // when the packet is outgoing we gotta swap the source and destination IPs so it is displayed correctly, combined into one line
        // std::swap(packet.src_ip, packet.dst_ip);
        std::string temp_src_ip = packet.getSrcIp();
        std::string temp_dst_ip = packet.getDstIp();
        packet.setSrcIp(temp_dst_ip);
        packet.setDstIp(temp_src_ip);
        // packet.tx = packet.length;
        packet.setTx(packet.getLength());
    }
    else // if the packet is neither incoming nor outgoing, only happens if the local IP is not found
    {
        // mvprintw(10, 0, "Packet is neither incoming nor outgoing.");
        // // std::cerr << "Packet is neither incoming nor outgoing." << std::endl;
        // // print all local IPs
        // for (auto &ip : local_ips)
        // {
        //     mvprintw(11, 0, "%s", ip.c_str());
        //     // std::cerr << ip << std::endl;
        // }
        // mvprintw(12, 0, "Source IP: %s Destination IP: %s", packet.src_ip.c_str(), packet.dst_ip.c_str());
        // std::cerr << "Source IP: " << packet.src_ip << " Destination IP: " << packet.dst_ip << std::endl;
        // std::cerr << "Packet is neither incoming nor outgoing." << std::endl;
        // exit(EXIT_FAILURE); 
        return;
    }   
}