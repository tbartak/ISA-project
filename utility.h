#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <string>
// #include <memory> // std::unique_ptr
#include <pcap.h>
#include <unordered_map>

// structure for individual packets
struct sPacket {
    std::string src_ip;
    int src_port = -1;
    std::string dst_ip;
    int dst_port = -1;
    std::string protocol;
    int length;
    int rx;
    int tx;
    std::string timestamp;
};

void add_packet(std::string key, sPacket packet);
void clear_data();
void clear_packets();
void print_packets();
void timer(int time);
pcap_t *open_interface(std::string &interface);
void close_interface(pcap_t *handle);
void packet_capture(pcap_t *handle);
void packet_handler();

#endif // UTILITY_H