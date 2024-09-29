#include <iostream>
#include <string>
//#include <vector>
//#include <getopt.h>

struct Config
{
    std::string interface; // interface variable (-i)
    char sort = 'b'; // default sort is by bytes (-b)
    int time = 1; // default time is 1 second (-t 1) (extra optional argument)
};
