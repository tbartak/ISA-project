#include <iostream>
#include "argument_parser.h"
#include "utility.h"

int main(int argc, char *argv[])
{
    std::unique_ptr<Config> config = parse_args(argc, argv);

    // check selected interface
    pcap_t *handle = open_interface(config->interface);

    // TODO: need to handle timer that will refresh the screen every second (or the specified time)

    // capture packets
    packet_capture(handle);

    // after graceful shutdown
    close_interface(handle);
    clear_packets();

    // TODO: Implement the rest of the program
    // close gracefully (Ctrl+C)
    // print results (SRC IP:port, DST IP:port, proto, Rx, Tx) using ncurses (first just basic info)

    // refresh every 1 second

    // sort method (bytes or packets)

    // time interval (extra)

    // check for memory leaks

    // tests (unit tests, integration tests) + merlin testing (valgrind, ...)

    // documentation (doxygen)

    return 0;
}