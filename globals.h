#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include "packet_config.h"
// #include "sorting.h"
#include <pcap.h>

extern PacketConfig packet_config;

// extern Sorting sorting;

// extern std::atomic<bool> stop_flag;

extern pcap_t *global_handle;

#endif // GLOBALS_H