#include <pcap.h>
#include <iostream>
#include <string>
// #include <memory> // std::unique_ptr

pcap_t *open_interface(std::string &interface)
{
    char errbuf[PCAP_ERRBUF_SIZE];

    pcap_t *handle = pcap_open_live(interface.c_str(), BUFSIZ, 1, 1000, errbuf);
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

        exit(EXIT_FAILURE);
    }

    std::cout << "Interface " << interface << " opened." << std::endl;

    return handle;
}