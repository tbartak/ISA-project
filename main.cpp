#include <iostream>
#include "argument_parser.h"
#include "utility.h"

int main(int argc, char *argv[])
{
    // TODO: just testing purposes to see if cli_parser works, later remove this
    printf("argc = %d\n", argc);
    for (int i = 0; i < argc; i++)
    {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    std::unique_ptr<Config> config = parse_args(argc, argv);

    // TODO: just testing purposes to see if cli_parser works, later remove this
    std::cout << "Interface: " << config->interface << std::endl;
    std::cout << "Sort: " << config->sort << std::endl;
    std::cout << "Time: " << config->time << std::endl;

    // check selected interface
    pcap_t *handle = open_interface(config->interface);



    pcap_close(handle);

    // TODO: Implement the rest of the program
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