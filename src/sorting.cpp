/**
 * @file sorting.cpp
 * @author Tomáš Barták (xbarta51)
 */

#include "sorting.h"
#include "globals.h"

/**
 * @brief Constructor for a new Sorting object
 * 
 */
Sorting::Sorting() {}

/**
 * @brief Destructor for the Sorting object
 * 
 */
Sorting::~Sorting() {}


/**
 * @brief Function that goes through the packet table to find the top 10 connections with most traffic.
 * 
 */
void Sorting::search_most_traffic()
{
    for (auto &packet : packet_config.getPacketTable())
    {
        create_most_traffic_array(packet.second);
    }
}

/**
 * @brief Function that handles the current packet passed and adds it to the table of top connections, if needed.
 * 
 * @param packet packet that is currently processed.
 */
void Sorting::create_most_traffic_array(Packet packet)
{
    // If the table is empty, add the first packet
    if (top_connections.empty())
    {
        top_connections.push_back(packet);
    }
    else
    {
        // If the table is not full yet, add the packet
        if (top_connections.size() < 10)
        {
            top_connections.push_back(packet);
        }
        else
        {
            // If the table is full, check if the packet has more traffic than the smallest packet in the table, if so, replace it
            for (auto &current_packet : top_connections)
            {
                if (packet.getLength() > current_packet.getLength())
                {
                    current_packet = packet;
                    break;
                }
            }
        }
    }
}

/**
 * @brief Function that sorts the top 10 connections based on the sort argument (either bytes or packets).
 * 
 * @param sort_by argument that is used to sort the top connections.
 */
void Sorting::sort_most_traffic(char sort_by)
{
    if (sort_by == 'b')
    {
        std::sort(top_connections.begin(), top_connections.end(), [](Packet a, Packet b) { return ((a.getRx() + a.getTx()) > (b.getRx() + b.getTx())); });
    }
    else if (sort_by == 'p')
    {
        std::sort(top_connections.begin(), top_connections.end(), [](Packet a, Packet b) { return ((a.getRxPacketCount() +  a.getTxPacketCount()) > (b.getRxPacketCount() + b.getTxPacketCount())); });
    }
    else
    {
        std::cerr << "Invalid argument for sorting the top connections." << std::endl;
        exit(EXIT_FAILURE);
    }
}

/**
 * @brief Function that clears the table of top connections.
 * 
 */
void Sorting::clear_top_connections()
{
    top_connections.clear();
}


/**
 * @brief Getter for the table of top connections.
 * 
 * @return table of top connections.
 */
std::vector<Packet>& Sorting::getTopConnections()
{
    return top_connections;
}