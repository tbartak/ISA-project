/**
 * @file network_interface.h
 * @author Tomáš Barták (xbarta51)
 */

#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include <iostream>
#include <string>
#include <pcap.h>
#include <cstring>


/**
 * @brief Class for handling network interfaces.
 * 
 */
class NetworkInterface
{
    public:
        // Constructor and destructor
        NetworkInterface();
        ~NetworkInterface();

        // Methods
        bool open_interface(const std::string &interface);
        void close_interface();
        
        // Getter
        pcap_t *getHandle() const;

    private:
        pcap_t *handle;

};

#endif // NETWORK_INTERFACE_H