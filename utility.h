#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <string>
// #include <memory> // std::unique_ptr
#include <pcap.h>

pcap_t *open_interface(std::string &interface);
void close_interface(pcap_t *handle);
void packet_capture(pcap_t *handle);
void packet_handler();

#endif // UTILITY_H