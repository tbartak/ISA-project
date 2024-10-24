/**
 * @file sorting.h
 * @author Tomáš Barták (xbarta51)
 */

#ifndef SORTING_H
#define SORTING_H


#include <vector>
#include "packet_info.h"
#include <algorithm> // std::sort
#include <iostream>
#include "packet_config.h"
// #include "globals.h"

/**
 * @brief Class for sorting packets based on the traffic.
 * 
 */
class Sorting
{
    public:
        // Constructor and destructor
        Sorting();
        ~Sorting();

        // Methods
        void search_most_traffic();
        void create_most_traffic_array(Packet packet);
        void sort_most_traffic(char sort);
        void clear_top_connections();

        // Getters
        std::vector<Packet>& getTopConnections();

    private:
        std::vector<Packet> top_connections;
};

#endif // SORTING_H