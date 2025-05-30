/**
 * @file packet_info.h
 * @author Tomáš Barták (xbarta51)
 */

#ifndef PACKET_H
#define PACKET_H

#include <string>

/**
 * @brief Class for storing packet information, getters, setters and other methods of the packet.
 * 
 */
class Packet
{
    public:
        // Constructor
        Packet();
        Packet(std::string src_ip, int src_port, std::string dst_ip, int dst_port, std::string protocol, int length, int rx, int tx, int rx_packet_count, int tx_packet_count, int packet_count);

        // Destructor
        ~Packet();
        
        // Getters
        std::string getSrcIp() const;
        int getSrcPort() const;
        std::string getDstIp() const;
        int getDstPort() const;
        std::string getProtocol() const;
        int getLength() const;
        int getRx() const;
        int getTx() const;
        int getRxPacketCount() const;
        int getTxPacketCount() const;
        int getPacketCount() const;
        
        // Setters
        void setSrcIp(std::string src_ip);
        void setSrcPort(int src_port);
        void setDstIp(std::string dst_ip);
        void setDstPort(int dst_port);
        void setProtocol(std::string protocol);
        void setLength(int length);
        void setRx(int rx);
        void setTx(int tx);
        void setRxPacketCount(int rx_packet_count);
        void setTxPacketCount(int tx_packet_count);
        void setPacketCount(int packet_count);

        // Methods
        void addLength(int length);
        void addRx(int rx);
        void addTx(int tx);
        void addPacketCount();
        void addRxPacketCount(int rx_packet_count);
        void addTxPacketCount(int tx_packet_count);

    private:
        std::string src_ip;
        int src_port;
        std::string dst_ip;
        int dst_port;
        std::string protocol;
        int length;
        int rx;
        int tx;
        int rx_packet_count;
        int tx_packet_count;
        int packet_count;
};

#endif // PACKET_H