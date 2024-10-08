#include "network_interface.h"
#include <cstring>

// constructor
NetworkInterface::NetworkInterface() : handle(nullptr) 
{
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);
}

// destructor
NetworkInterface::~NetworkInterface()
{
    if (handle != nullptr)
    {
        close_interface();
    }
}

// function that will open the selected interface and exit with an error if the interface is not available
bool NetworkInterface::open_interface(const std::string &interface)
{
    char errbuf[PCAP_ERRBUF_SIZE];

    handle = pcap_open_live(interface.c_str(), BUFSIZ, 0, 1000, errbuf); // promiscuous mode is disabled, timeout is 1000ms
    if (handle == NULL)
    {
        std::cerr << "Couldn't open interface " << interface << ": " << errbuf << std::endl;
        
        // prints all available interfaces
        pcap_if_t *all_interfaces;
        if (pcap_findalldevs(&all_interfaces, errbuf) == -1)
        {
            std::cerr << "Couldn't find any interfaces: " << errbuf << std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout << "Available interfaces:" << std::endl;
        for (pcap_if_t *i = all_interfaces; i != NULL; i = i->next)
        {
            std::cout << i->name << std::endl;
        }
        pcap_freealldevs(all_interfaces);

        return false;
    }

    std::cout << "Interface " << interface << " opened." << std::endl;

    return true;
}

// function that will close the selected interface (TODO: later needs to be called after pressing Ctrl+C, so that no allocated memory is left behind)
void NetworkInterface::close_interface()
{
    pcap_close(handle);
    handle = nullptr;
    std::cout << "Interface closed" << std::endl;
}

// getter for handle
pcap_t *NetworkInterface::getHandle() const
{
    return handle;
}