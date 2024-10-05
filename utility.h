#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <string>
// #include <memory> // std::unique_ptr
#include <pcap.h>
#include <unordered_map>
#include <chrono> // std::chrono::system_clock
#include <thread> // std::this_thread::sleep_for
#include <signal.h> // signal handling
#include "config.h"
#include <atomic>
#include <ncurses.h>
#include <algorithm> // std::sort
#include <vector>
#include <netinet/ip6.h> // struct ip6_hdr
#include <ifaddrs.h> // getifaddrs


// structure for individual packets
struct sPacket {
    std::string src_ip;
    int src_port = -1;
    std::string dst_ip;
    int dst_port = -1;
    std::string protocol;
    int length = 0;
    int packet_count = 0;
    int rx = 0;
    int tx = 0;
    std::string timestamp;
};

extern std::atomic<bool> stop_flag;  // Declare the stop flag as external

std::vector<std::string> get_local_ips(/*std::string &interface_name*/);
void add_packet(std::string key, sPacket packet);
void clear_data();
void clear_packets();
void print_packets();
void print_packet(std::string key);
void timer(int time, char sort);
void signal_handler(int signal);
void shutdown();
void search_most_traffic();
void create_most_traffic_array(std::vector<sPacket> &top_connections, sPacket packet);
void sort_most_traffic(std::vector<sPacket> &top_connections, char sort);
pcap_t *open_interface(std::string &interface);
void close_interface(pcap_t *handle);
void packet_capture(pcap_t *handle, std::unique_ptr<Config> &config);
void packet_handler(struct pcap_pkthdr* pkthdr, const u_char *packet, const std::vector<std::string> &local_ips);

#endif // UTILITY_H