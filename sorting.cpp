#include "sorting.h"
#include "globals.h"

Sorting::Sorting() {}

Sorting::~Sorting() {}


// function that will search for the top 10 connections with most traffic
void Sorting::search_most_traffic(/*const std::unordered_map<std::string, Packet>& packet_table*/)
{
    for (auto &packet : packet_config.getPacketTable())
    {
        create_most_traffic_array(packet.second);
    }
}

// function that will create a table of top 10 connections with most traffic
void Sorting::create_most_traffic_array(Packet packet)
{
    // if the packet doesnt have any traffic, dont add it to the table (no rx or tx) // this will prevent packets that are not communicating with the local machine to be displayed
    // TODO: in case we want to save the packets that are not communicating with the local machine to the structure, but not the table, use this instead of the return in packet_handler
    // if (packet.rx == 0 && packet.tx == 0)
    // {
    //     return;
    // }
    
    // if the table is empty, add the first packet
    if (top_connections.empty())
    {
        top_connections.push_back(packet);
    }
    else
    {
        // if the table is not full yet, add the packet
        if (top_connections.size() < 10)
        {
            top_connections.push_back(packet);
        }
        else
        {
            // if the table is full, check if the packet has more traffic than the smallest packet in the table
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

// function that will sort the top 10 connections with most traffic based on the -s argument (bytes or packets)
void Sorting::sort_most_traffic(char sort_by)
{
    if (sort_by == 'b')
    {
        std::sort(top_connections.begin(), top_connections.end(), [](Packet a, Packet b) { return ((a.getRx() + a.getTx()) > (b.getRx() + b.getTx())); });
    }
    else if (sort_by == 'p')
    {
        std::sort(top_connections.begin(), top_connections.end(), [](Packet a, Packet b) { return a.getPacketCount() > b.getPacketCount(); });
    }
    else
    {
        std::cerr << "Invalid argument for sorting the top connections." << std::endl;
        exit(EXIT_FAILURE);
    }
}

// function to clear the top connections table
void Sorting::clear_top_connections()
{
    top_connections.clear();
}


// getter for top_connections
std::vector<Packet>& Sorting::getTopConnections()
{
    return top_connections;
}