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
    mvprintw(row, RX_COL, "Rx");
    mvprintw(row, TX_COL, "Tx");

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
                mvprintw(row, RX_BYTES_COL, "%s", utility.convert_data_amount(packet.getRx() / interval).c_str());
                mvprintw(row, TX_BYTES_COL, "%s", utility.convert_data_amount(packet.getTx() / interval).c_str());
                mvprintw(row, RX_PACKET_COUNT_COL, "%s", utility.convert_data_amount(packet.getRxPacketCount() / interval).c_str());
                mvprintw(row, TX_PACKET_COUNT_COL, "%s", utility.convert_data_amount(packet.getTxPacketCount() / interval).c_str());
            }
            else // if the port is defined, print the IP address and the port
            {
                // add port to the IP address of the source and destination
                mvprintw(row, SRC_IP_COL, "%s:%d", packet.getSrcIp().c_str(), packet.getSrcPort());
                mvprintw(row, DST_IP_COL, "%s:%d", packet.getDstIp().c_str(), packet.getDstPort());
                mvprintw(row, PROTOCOL_COL, "%s", packet.getProtocol().c_str());
                mvprintw(row, RX_BYTES_COL, "%s", utility.convert_data_amount(packet.getRx() / interval).c_str());
                mvprintw(row, TX_BYTES_COL, "%s", utility.convert_data_amount(packet.getTx() / interval).c_str());
                mvprintw(row, RX_PACKET_COUNT_COL, "%s", utility.convert_data_amount(packet.getRxPacketCount() / interval).c_str());
                mvprintw(row, TX_PACKET_COUNT_COL, "%s", utility.convert_data_amount(packet.getTxPacketCount() / interval).c_str());
            }
            row++; // move to the next row
        }
    }
    mvprintw(20, 0, "Press Ctrl+C to stop the program."); // informational message for the user
    refresh(); // refresh the screen to display the new data
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
        utility.clear_data();
    }
}