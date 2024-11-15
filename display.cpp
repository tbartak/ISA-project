/**
 * @file display.cpp
 * @author Tomáš Barták (xbarta51)
 */

#include "display.h"

/**
 * @brief Constructor for a Display object, initializes ncurses mode.
 * 
 */
Display::Display() 
{
    initscr(); // start ncurses mode
    cbreak(); // disable line buffering
    noecho(); // disable echoing of characters
}

/**
 * @brief Destructor for a Display object, ends ncurses mode.
 * 
 */
Display::~Display()
{
    endwin(); // end ncurses mode
}


// function to print all packets in the hash map // TODO: for testing purposes, will be removed later
// void Display::print_packets(int time)
// {
//     (void)time; // unused variable
//     for (auto &packet : packet_config.getPacketTable()) // TODO: change for top_connections
//     {
//         if (packet.second.getSrcPort() == -1 || packet.second.getDstPort() == -1)
//         {
//             std::cout << "Source IP: " << packet.second.getSrcIp() << " Destination IP: " << packet.second.getDstIp() << " Protocol: \n" << packet.second.getProtocol() << " Length: " << packet.second.getLength() << " Bytes " << " RX: " << packet.second.getRx() << " Bytes " << " TX: " << packet.second.getTx() << " Bytes " << "Count of packets: " << packet.second.getPacketCount() << std::endl;
//         }
//         else
//         {
//         std::cout << "Source IP: " << packet.second.getSrcIp() << " Source Port: " << packet.second.getSrcPort() << " Destination IP: " << packet.second.getDstIp() << " Destination Port: " << packet.second.getDstPort() << " Protocol: \n" << packet.second.getProtocol() << " Length: " << packet.second.getLength() << " Bytes " << " RX: " << packet.second.getRx() << " Bytes " << " TX: " << packet.second.getTx() << " Bytes " << "Count of packets: " << packet.second.getPacketCount() << std::endl;
//         }
//     }
// }

// function to print all packets in the hash map using ncurses
/**
 * @brief Function that prints the top connections from the hash map onto the screen using ncurses.
 * 
 * @param time time interval that is used to calculate the bandwidth.
 */
void Display::print_packets(int time)
{
    Utility utility;
    clear(); // clear the screen

    double interval = (double)time; // interval in seconds

    int row = 0;

    // header of the table
    mvprintw(row, SRC_IP_COL, "Src IP:port");
    mvprintw(row, DST_IP_COL, "Dst IP:port");
    mvprintw(row, PROTOCOL_COL, "Proto");
    // mvprintw(row, LENGTH_COL, "Length");
    mvprintw(row, RX_COL, "Rx");
    mvprintw(row, TX_COL, "Tx");
    // mvprintw(row, PACKET_COUNT_COL, "Packet Count");
    // mvprintw(row, RX_PACKET_COUNT_COL, "RX Packet Count");
    // mvprintw(row, TX_PACKET_COUNT_COL, "TX Packet Count");

    row++; // Rx and Tx header under the main header

    mvprintw(row, RX_BYTES_COL, "b/s");
    mvprintw(row, TX_BYTES_COL, "b/s");
    mvprintw(row, RX_PACKET_COUNT_COL, "p/s");
    mvprintw(row, TX_PACKET_COUNT_COL, "p/s");

    row++; // to write below the header

    // print all sorted packets from the hash map
    for (auto &packet : sorting.getTopConnections())
    {        
        // print only packets that have some transmission in the last time interval
        if (packet.getLength() != 0)
        {      
            // if the port is not defined, print only the IP address
            if (packet.getSrcPort() == -1 || packet.getDstPort() == -1)
            {
                mvprintw(row, SRC_IP_COL, "%s", packet.getSrcIp().c_str());
                mvprintw(row, DST_IP_COL, "%s", packet.getDstIp().c_str());
                mvprintw(row, PROTOCOL_COL, "%s", packet.getProtocol().c_str());
                // mvprintw(row, LENGTH_COL, "%sB/s", utility.convert_data_amount(packet.getLength() / interval).c_str());
                mvprintw(row, RX_BYTES_COL, "%s", utility.convert_data_amount(packet.getRx() / interval).c_str());
                mvprintw(row, TX_BYTES_COL, "%s", utility.convert_data_amount(packet.getTx() / interval).c_str());
                // mvprintw(row, PACKET_COUNT_COL, "%sp/s", utility.convert_data_amount(packet.getPacketCount() / interval).c_str());
                mvprintw(row, RX_PACKET_COUNT_COL, "%s", utility.convert_data_amount(packet.getRxPacketCount() / interval).c_str());
                mvprintw(row, TX_PACKET_COUNT_COL, "%s", utility.convert_data_amount(packet.getTxPacketCount() / interval).c_str());
            }
            else // if the port is defined, print the IP address and the port
            {
                // add port to the IP address of the source and destination
                mvprintw(row, SRC_IP_COL, "%s:%d", packet.getSrcIp().c_str(), packet.getSrcPort());
                mvprintw(row, DST_IP_COL, "%s:%d", packet.getDstIp().c_str(), packet.getDstPort());
                mvprintw(row, PROTOCOL_COL, "%s", packet.getProtocol().c_str());
                // mvprintw(row, LENGTH_COL, "%sB/s", utility.convert_data_amount(packet.getLength() / interval).c_str());
                mvprintw(row, RX_BYTES_COL, "%s", utility.convert_data_amount(packet.getRx() / interval).c_str());
                mvprintw(row, TX_BYTES_COL, "%s", utility.convert_data_amount(packet.getTx() / interval).c_str());
                // mvprintw(row, PACKET_COUNT_COL, "%sp/s", utility.convert_data_amount(packet.getPacketCount() / interval).c_str());
                mvprintw(row, RX_PACKET_COUNT_COL, "%s", utility.convert_data_amount(packet.getRxPacketCount() / interval).c_str());
                mvprintw(row, TX_PACKET_COUNT_COL, "%s", utility.convert_data_amount(packet.getTxPacketCount() / interval).c_str());
            }
            row++; // move to the next row
        }
    }
    mvprintw(20, 0, "Press Ctrl+C to stop the program."); // shortcut to end the program
    refresh(); // refresh the screen to display the new data
}

// TODO: debug function for printing a single packet
void Display::print_packet(std::string key) 
{
    const auto &packet_table = packet_config.getPacketTable();
    auto packet = packet_table.at(key);
    std::cout << "Source IP: " << packet.getSrcIp() << " Source Port: " << packet.getSrcPort() << " Destination IP: " << packet.getDstIp() << " Destination Port: " << packet.getDstPort() << " Protocol: " << packet.getProtocol() << " Length: " << packet.getLength() << " Bytes " << " RX: " << packet.getRx() << " Bytes " << " TX: " << packet.getTx() << " Bytes " << "Count of packets: " << packet.getPacketCount() << std::endl;
}

/**
 * @brief Function that is called every second to refresh the screen with new data.
 * 
 * @param time time period that controls how often the screen is refreshed.
 * @param sort sort type ('b' for bytes, 'p' for packets).
 */
void Display::timer(int time, char sort)
{
    while (!stop_flag.load())
    {
        // wait for the specified time
        for (int i = 0; i < time; i++)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (stop_flag.load()) // if the stop flag is set, stop the timer -> close the program
            {
                return;
            }
        }
        sorting.search_most_traffic(); // search for the most traffic in the last time interval
        sorting.sort_most_traffic(sort); // sort it by bytes or packets afterwards
        print_packets(time);
        Utility utility;
        utility.clear_data(); // TODO: or clear_packets()? to clear all data of the packets? could be better for memory management
    }
}