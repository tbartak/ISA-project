/**
 * @file globals.h
 * @author Tomáš Barták (xbarta51)
 */

#ifndef GLOBALS_H
#define GLOBALS_H

#include "sorting.h"
#include <iostream>
#include <atomic>
#include "packet_config.h"
#include <pcap.h>

extern PacketConfig packet_config;

extern Sorting sorting;

extern std::atomic<bool> stop_flag;

extern pcap_t *global_handle;

#endif // GLOBALS_H