#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include <iostream>
#include <string>
#include <pcap.h>

class NetworkInterface
{
    public:
        // constructor and destructor
        NetworkInterface();
        ~NetworkInterface();

        bool open_interface(const std::string &interface);
        void close_interface();
        
        pcap_t *getHandle() const;

    private:
        pcap_t *handle;

};

#endif // NETWORK_INTERFACE_H