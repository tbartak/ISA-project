/**
 * @file globals.cpp
 * @author Tomáš Barták (xbarta51)
 */

#include "globals.h"

PacketConfig packet_config;

Sorting sorting;

pcap_t *global_handle = nullptr;

std::atomic<bool> stop_flag(false); // stop flag for stopping the program

