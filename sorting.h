#ifndef SORTING_H
#define SORTING_H


#include <vector>
#include "packet_info.h"
#include <algorithm> // std::sort
#include <iostream>
#include "packet_config.h"
// #include "globals.h"


class Sorting
{
    public:
        Sorting();
        ~Sorting();

        void search_most_traffic(/*const std::unordered_map<std::string, Packet>& packet_table*/);
        void create_most_traffic_array(Packet packet);
        void sort_most_traffic(char sort);
        void clear_top_connections();

        std::vector<Packet>& getTopConnections();

    private:
        std::vector<Packet> top_connections;
};

#endif // SORTING_H