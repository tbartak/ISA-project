/**
 * @file display.h
 * @author Tomáš Barták (xbarta51)
 */

#ifndef DISPLAY_H
#define DISPLAY_H

#include <ncurses.h>
#include <vector>
#include "packet_info.h"
#include <string>
#include <chrono>
#include "utility.h"
#include "packet_config.h"
// #include "sorting.h"
#include "globals.h"

// column widths for ncurses
#define SRC_IP_COL 0
#define DST_IP_COL 35
#define PROTOCOL_COL 75
#define LENGTH_COL 90
#define RX_COL 105
#define TX_COL 120
#define PACKET_COUNT_COL 135
#define RX_PACKET_COUNT_COL 150
#define TX_PACKET_COUNT_COL 165

/**
 * @brief class for displaying packets in the terminal
 */
class Display
{
    public:
        // Constructor and destructor
        Display();
        ~Display();

        // Methods
        static void print_packets(int time);
        void print_packet(std::string key);
        static void timer(int time, char sort);

    
};


#endif // DISPLAY_H