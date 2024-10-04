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

    signal(SIGINT, signal_handler);

    // TODO: need to handle timer that will refresh the screen every second (or the specified time)
    std::thread timer_thread(timer, config->time, config->sort);

    // capture packets
    packet_capture(handle);

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

    // sort method (bytes or packets)

    // check for memory leaks

    // tests (unit tests, integration tests) + merlin testing (valgrind, ...)

    // documentation (doxygen)

    return 0;
}