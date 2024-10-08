#include <iostream>
#include "config.h"
#include "utility.h"
#include "network_interface.h"


int main(int argc, char *argv[])
{
    initscr();
    cbreak(); // disable line buffering
    noecho(); // disable echoing of characters

    Config config;
    config.parse_args(argc, argv);

    NetworkInterface network_interface;

    // check selected interface
    if (!network_interface.open_interface(config.getInterface()))
    {
        endwin();
        return 1;
    }


    // check for Ctrl+C
    signal(SIGINT, signal_handler);

    // start another thread that will control refresh of the screen
    std::thread timer_thread(timer, config.getTime(), config.getSort());

    // capture packets
    packet_capture(network_interface.getHandle(), config);

    // after receiving SIGINT, stop the program
    std::cout << "Program stopped by user." << std::endl;
    timer_thread.join();
    network_interface.close_interface();
    clear_packets();
    // shutdown();

    endwin();
    
    

    std::cout << "Program finished. Resources cleaned." << std::endl;

    // TODO: Implement the rest of the program
    // close gracefully (Ctrl+C)

    // check for memory leaks

    // tests (unit tests, integration tests) + merlin testing (valgrind, ...)

    // documentation (doxygen)

    return 0;
}