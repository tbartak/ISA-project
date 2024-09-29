//#include <iostream>
#include "cli_parser.h"

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

    // TODO: Implement the rest of the program

    return 0;
}