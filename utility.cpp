#include <pcap.h>
#include <iostream>
#include <string>
// #include <memory> // std::unique_ptr

// function that will open the selected interface and exit with an error if the interface is not available
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

// function that will close the selected interface (TODO: later needs to be called after pressing Ctrl+C, so that no allocated memory is left behind)
void close_interface(pcap_t *handle)
{
    pcap_close(handle);
    std::cout << "Interface closed" << std::endl;
}

// function that will handle each packet captured and print/save results into a structure
void packet_handler()
{
    std::cout << "Got a packet" << std::endl;
}

// function that will capture packets
void packet_capture(pcap_t *handle)
{
    std::cout << "Packet Capture Starts..." << std::endl;

    struct pcap_pkthdr header;
    const u_char *packet;

    // will capture packets until the program is terminated
    while (true)
    {
        packet = pcap_next(handle, &header);
        // packet will be NULL if no packet is captured
        if (packet == NULL)
        {
            continue;
        }

        packet_handler();
    }
}