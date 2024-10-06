#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>
//#include <vector>
//#include <getopt.h>

class Config
{
    public:
        Config();

        std::string getInterface() const;
        char getSort() const;
        int getTime() const;

        void parse_args(int argc, char *argv[]);


    private:
        std::string interface; // interface variable (-i)
        char sort = 'b'; // default sort is by bytes (-b)
        int time = 1; // default time is 1 second (-t 1) (extra optional argument)
};

#endif // CONFIG_H