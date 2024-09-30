#ifndef ARGUMENT_PARSER_H
#define ARGUMENT_PARSER_H

#include <memory> // std::unique_ptr
#include "config.h"

std::unique_ptr<Config> parse_args(int argc, char *argv[]);

#endif // ARGUMENT_PARSER_H