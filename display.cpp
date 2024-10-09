#include "display.h"


Display::Display() 
{
    initscr(); // start ncurses mode
    cbreak(); // disable line buffering
    noecho(); // disable echoing of characters
}

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
void Display::print_packets(int time)
{
    clear(); // clear the screen

    double interval = (double)time; // interval in seconds

    int row = 0;

    // header of the table
    mvprintw(row, SRC_IP_COL, "Src IP:port");
    mvprintw(row, DST_IP_COL, "Dst IP:port");
    mvprintw(row, PROTOCOL_COL, "Proto");
    mvprintw(row, LENGTH_COL, "Length");
    mvprintw(row, RX_COL, "RX");
    mvprintw(row, TX_COL, "TX");
    mvprintw(row, PACKET_COUNT_COL, "Packet Count");

    row++;


    for (auto &packet : sorting.getTopConnections())
    {        
        // print only packets that have some transmission in the last second // TODO: not needed anymore, since the packets that are not communicating with the local machine are not added to the table
        if (packet.getLength() != 0)
        {      
            if (packet.getSrcPort() == -1 || packet.getDstPort() == -1)
            {
                mvprintw(row, SRC_IP_COL, "%s", packet.getSrcIp().c_str());
                mvprintw(row, DST_IP_COL, "%s", packet.getDstIp().c_str());
                mvprintw(row, PROTOCOL_COL, "%s", packet.getProtocol().c_str());
                mvprintw(row, LENGTH_COL, "%sB/s", convert_data_amount(packet.getLength() / interval).c_str());
                mvprintw(row, RX_COL, "%sB/s", convert_data_amount(packet.getRx() / interval).c_str());
                mvprintw(row, TX_COL, "%sB/s", convert_data_amount(packet.getTx() / interval).c_str());
                mvprintw(row, PACKET_COUNT_COL, "%sp/s", convert_data_amount(packet.getPacketCount() / interval).c_str());
            }
            else
            {
                // add port to the IP address of the source and destination
                mvprintw(row, SRC_IP_COL, "%s:%d", packet.getSrcIp().c_str(), packet.getSrcPort());
                mvprintw(row, DST_IP_COL, "%s:%d", packet.getDstIp().c_str(), packet.getDstPort());
                mvprintw(row, PROTOCOL_COL, "%s", packet.getProtocol().c_str());
                mvprintw(row, LENGTH_COL, "%sB/s", convert_data_amount(packet.getLength() / interval).c_str());
                mvprintw(row, RX_COL, "%sB/s", convert_data_amount(packet.getRx() / interval).c_str());
                mvprintw(row, TX_COL, "%sB/s", convert_data_amount(packet.getTx() / interval).c_str());
                mvprintw(row, PACKET_COUNT_COL, "%sp/s", convert_data_amount(packet.getPacketCount() / interval).c_str());
            }
            row++;
        }
    }
    mvprintw(20, 0, "Press Ctrl+C to stop the program.");
    refresh();
}

// TODO: debug function for printing a single packet
void Display::print_packet(std::string key) 
{
    const auto &packet_table = packet_config.getPacketTable(); // TODO: ended here
    auto packet = packet_table.at(key);
    std::cout << "Source IP: " << packet.getSrcIp() << " Source Port: " << packet.getSrcPort() << " Destination IP: " << packet.getDstIp() << " Destination Port: " << packet.getDstPort() << " Protocol: " << packet.getProtocol() << " Length: " << packet.getLength() << " Bytes " << " RX: " << packet.getRx() << " Bytes " << " TX: " << packet.getTx() << " Bytes " << "Count of packets: " << packet.getPacketCount() << std::endl;
}

// timer function that will be called every second (or other time specified by -t) to refresh the screen
void Display::timer(int time, char sort)
{
    while (!stop_flag.load())
    {
        // TODO: finish timer function
        // wait for the specified time
        // could optimize, lets say that it will always sleep for 1 second, but if the time was set to 10 seconds, it will repeat the sleep 9 more times
        for (int i = 0; i < time; i++)
        {
            std::this_thread::sleep_for(std::chrono::seconds(1));
            if (stop_flag.load()) // if the stop flag is set, stop the timer -> close the program
            {
                return;
            }
        }
        // std::this_thread::sleep_for(std::chrono::seconds(time)); // when closing using Ctrl+C, this will still wait for the period to finish, even though it wont receive any more data from the packets
        sorting.search_most_traffic();
        sorting.sort_most_traffic(sort);
        print_packets(time); // to calculate the bandwidth, we pass the time set by the user to the function
        clear_data(); // TODO: or clear_packets()? to clear all data of the packets? could be better for memory management
    }
}