#include <iostream>
#include "config.h"
#include "utility.h"
#include "network_interface.h"
// #include "display.h"
#include <functional>
#include "packet_handling.h"
#include "globals.h"
#include "display.h"


int main(int argc, char *argv[])
{
    Display display;

    Config config;
    config.parse_args(argc, argv);

    NetworkInterface network_interface;

    // check selected interface
    if (!network_interface.open_interface(config.getInterface()))
    {
        return 1;
    }

    global_handle = network_interface.getHandle();


    // check for Ctrl+C
    signal(SIGINT, signal_handler);

    // start another thread that will control refresh of the screen
    std::thread timer_thread(display.timer, config.getTime(), config.getSort());

    // capture packets
    PacketHandling packet_handling;
    packet_handling.packet_capture(config);

    // after receiving SIGINT, stop the program
    std::cout << "Program stopped by user." << std::endl;
    timer_thread.join();
    network_interface.close_interface();
    clear_packets();
    // shutdown();

    std::cout << "Program finished. Resources cleaned." << std::endl;

    // TODO: Implement the rest of the program
    // close gracefully (Ctrl+C)

    // check for memory leaks

    // tests (unit tests, integration tests) + merlin testing (valgrind, ...)

    // documentation (doxygen)

    return 0;
}