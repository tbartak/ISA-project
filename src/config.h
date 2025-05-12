/**
 * @file config.h
 * @author Tomáš Barták (xbarta51)
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <iostream>
#include <string>
#include <getopt.h>
#include <memory> // std::unique_ptr

/**
 * @brief class for storing configuration values
 */
class Config
{
    public:
        // Constructor
        Config();

        // Destructor
        ~Config();

        // Methods
        void parse_args(int argc, char *argv[]);

        // Getters
        std::string getInterface() const;
        char getSort() const;
        int getTime() const;

    private:
        std::string interface; // interface variable (-i)
        char sort = 'b'; // default sort is by bytes (-b)
        int time = 1; // default time is 1 second (-t 1) (extra optional argument)
};

#endif // CONFIG_H