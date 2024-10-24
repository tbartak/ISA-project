/**
 * @file utility.h
 * @author Tomáš Barták (xbarta51)
 */

#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <string>
#include <memory> // std::unique_ptr
#include <pcap.h>
#include <unordered_map>
#include <chrono> // std::chrono::system_clock
#include <thread> // std::this_thread::sleep_for
#include <signal.h> // signal handling
// #include "sorting.h"
#include "config.h"
#include <atomic>
#include <ncurses.h>
#include <algorithm> // std::sort
#include <vector>
#include <netinet/ip6.h> // struct ip6_hdr
#include <ifaddrs.h> // getifaddrs
#include <netdb.h> // getprotobynumber
#include <cmath> // std::round
#include <sstream> // std::stringstream
#include <iomanip> // std::setprecision
#include "packet_info.h"
#include "globals.h"

extern std::atomic<bool> stop_flag;  // Declare the stop flag as external

std::vector<std::string> get_local_ips(/*std::string &interface_name*/);
void clear_data();
void clear_packets();
void signal_handler(int signal);
void shutdown(std::thread &timer_thread);
std::string convert_data_amount(double data_amount);

#endif // UTILITY_H