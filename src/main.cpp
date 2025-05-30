/**
 * @file main.cpp
 * @author Tomáš Barták (xbarta51)
 */

#include <iostream>
#include "config.h"
#include "utility.h"
#include "network_interface.h"
#include <functional>
#include "packet_handling.h"
#include "globals.h"
#include "display.h"
#include <thread>

/**
 * @brief main function that handles the flow of the program.
 * 
 * @param argc count of arguments passed from the command line
 * @param argv array of arguments passed from the command line
 * @return return value of the program (0 if successful, 1 if not)
 */
int main(int argc, char *argv[])
{
    // creating instances of classes
    Display display;
    Config config;
    NetworkInterface network_interface;
    Utility utility;
    PacketHandling packet_handling;

    config.parse_args(argc, argv); // parse arguments from the command line

    // check selected interface
    if (!network_interface.open_interface(config.getInterface()))
    {
        return 1;
    }

    global_handle = network_interface.getHandle(); // get the handle of the network interface


    // check for Ctrl+C for graceful shutdown
    signal(SIGINT, utility.signal_handler);

    // start another thread that will control displaying and refresh of the screen
    std::thread timer_thread(display.timer, config.getTime(), config.getSort());

    // capture packets
    packet_handling.packet_capture(config);

    // after receiving SIGINT (Ctrl+C), stop the program
    std::cout << "Program stopped by user." << std::endl;
    timer_thread.join(); // close the timer thread
    network_interface.close_interface(); // close the network interface
    utility.clear_packets(); // clear all packets from the packet table

    std::cout << "Program finished. Resources cleaned." << std::endl;
    return 0;
}