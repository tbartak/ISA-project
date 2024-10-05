#include <iostream>
#include "argument_parser.h"
#include "utility.h"

int main(int argc, char *argv[])
{
    
    initscr();
    cbreak(); // disable line buffering
    noecho(); // disable echoing of characters

    std::unique_ptr<Config> config = parse_args(argc, argv);

    // check selected interface
    pcap_t *handle = open_interface(config->interface);

    // check for Ctrl+C
    signal(SIGINT, signal_handler);

    // start another thread that will control refresh of the screen
    std::thread timer_thread(timer, config->time, config->sort);

    // capture packets
    packet_capture(handle, config);

    // after receiving SIGINT, stop the program
    std::cout << "Program stopped by user." << std::endl;
    timer_thread.join();
    close_interface(handle);
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