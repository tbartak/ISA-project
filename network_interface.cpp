/**
 * @file network_interface.cpp
 * @author Tomáš Barták (xbarta51)
 */

#include "network_interface.h"
#include <cstring>

/**
 * @brief Constructor for a new Network Interface object
 * 
 */
NetworkInterface::NetworkInterface() : handle(nullptr) // initialize handle to nullptr
{
    char errbuf[PCAP_ERRBUF_SIZE];
    memset(errbuf, 0, PCAP_ERRBUF_SIZE);
}

/**
 * @brief Destructor for the Network Interface object
 * 
 */
NetworkInterface::~NetworkInterface()
{
    if (handle != nullptr) // close the interface, if it is still open
    {
        close_interface();
    }
}

// function that will open the selected interface and exit with an error if the interface is not available
/**
 * @brief Function that will open the selected interface and exit with an error, if the interface is not available.
 * 
 * @param interface name of the interface that is supposed to be opened
 * @return true, if the interface is opened successfully
 * @return false, if something went wrong (no interface available, wrong interface name, ...)
 */
bool NetworkInterface::open_interface(const std::string &interface)
{
    char errbuf[PCAP_ERRBUF_SIZE];

    handle = pcap_open_live(interface.c_str(), BUFSIZ, 0, 1000, errbuf); // try to open the interface, promiscuous mode is disabled, timeout is 1000ms
    if (handle == NULL)
    {
        std::cerr << "Couldn't open interface " << interface << ": " << errbuf << std::endl;
        
        // prints all available interfaces, if the selected interface is not available
        pcap_if_t *all_interfaces;
        if (pcap_findalldevs(&all_interfaces, errbuf) == -1)
        {
            std::cerr << "Couldn't find any interfaces: " << errbuf << std::endl;
            exit(EXIT_FAILURE);
        }
        std::cout << "Available interfaces:" << std::endl;
        for (pcap_if_t *i = all_interfaces; i != NULL; i = i->next) // list all available interfaces
        {
            std::cout << i->name << std::endl;
        }
        pcap_freealldevs(all_interfaces); // free the memory allocated by pcap_findalldevs

        return false;
    }

    std::cout << "Interface " << interface << " opened." << std::endl;

    return true;
}

/**
 * @brief Function that will close the selected interface.
 * 
 */
void NetworkInterface::close_interface()
{
    pcap_close(handle);
    handle = nullptr;
    std::cout << "Interface closed" << std::endl;
}

/**
 * @brief Getter method for the handle of the network interface.
 * 
 * @return handle of the network interface
 */
pcap_t *NetworkInterface::getHandle() const
{
    return handle;
}