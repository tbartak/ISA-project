#include <iostream>
#include "argument_parser.h"
#include "utility.h"

int main(int argc, char *argv[])
{
    std::unique_ptr<Config> config = parse_args(argc, argv);

    // check selected interface
    pcap_t *handle = open_interface(config->interface);

    // capture packets
    packet_capture(handle);

    close_interface(handle);

    // TODO: Implement the rest of the program
    // close gracefully (Ctrl+C)
    // check packets
    // create a structure for results and save them into them
    // print results (SRC IP:port, DST IP:port, proto, Rx, Tx) using ncurses (first just basic info)

    // refresh every 1 second

    // sort method (bytes or packets)

    // time interval (extra)

    // check for memory leaks

    // tests (unit tests, integration tests) + merlin testing (valgrind, ...)

    // documentation (doxygen)

    return 0;
}