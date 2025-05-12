/**
 * @file packet_info.cpp
 * @author Tomáš Barták (xbarta51)
 */

#include "packet_info.h"

/**
 * @brief Constructor for a new Packet object.
 * 
 * @param src_ip Source IP address.
 * @param src_port Source port.
 * @param dst_ip Destination IP address.
 * @param dst_port Destination port.
 * @param protocol Protocol of the packet.
 * @param length Length of the packet.
 * @param rx Received bytes.
 * @param tx Transmitted bytes.
 * @param rx_packet_count Count of packets received.
 * @param tx_packet_count Count of packets transmitted.
 * @param packet_count Count of packets captured.
 */
Packet::Packet(std::string src_ip, int src_port, std::string dst_ip, int dst_port, std::string protocol, int length, int rx, int tx, int rx_packet_count, int tx_packet_count, int packet_count) : 
src_ip(src_ip), src_port(src_port), dst_ip(dst_ip), dst_port(dst_port), protocol(protocol), length(length), rx(rx), tx(tx), rx_packet_count(rx_packet_count), tx_packet_count(tx_packet_count), packet_count(packet_count) {}

Packet::Packet() : src_ip(""), src_port(-1), dst_ip(""), dst_port(-1), protocol(""), length(0), rx(0), tx(0), rx_packet_count(0), tx_packet_count(0), packet_count(0) {}

/**
 * @brief Destructor for the Packet object.
 * 
 */
Packet::~Packet() {}

/**
 * @brief Getter for the source IP address.
 * 
 * @return source IP address of the packet.
 */
std::string Packet::getSrcIp() const { return src_ip; }

/**
 * @brief Getter for the source port.
 * 
 * @return source port of the packet.
 */
int Packet::getSrcPort() const { return src_port; }

/**
 * @brief Getter for the destination IP address.
 * 
 * @return destination IP address of the packet.
 */
std::string Packet::getDstIp() const { return dst_ip; }

/**
 * @brief Getter for the destination port.
 * 
 * @return destination port of the packet.
 */
int Packet::getDstPort() const { return dst_port; }

/**
 * @brief Getter for the protocol.
 * 
 * @return protocol of the packet.
 */
std::string Packet::getProtocol() const { return protocol; }

/**
 * @brief Getter for the length of the packet.
 * 
 * @return length of the packet.
 */
int Packet::getLength() const { return length; }

/**
 * @brief Getter for the received bytes.
 * 
 * @return received bytes of the packet.
 */
int Packet::getRx() const { return rx; }

/**
 * @brief Getter for the transmitted bytes.
 * 
 * @return transmitted bytes of the packet.
 */
int Packet::getTx() const { return tx; }

/**
 * @brief Getter for the count of received packets.
 * 
 * @return count of received packets.
 */
int Packet::getRxPacketCount() const { return rx_packet_count; }

/**
 * @brief Getter for the count of transmitted packets.
 * 
 * @return count of transmitted packets.
 */
int Packet::getTxPacketCount() const { return tx_packet_count; }

/**
 * @brief Getter for the count of packets.
 * 
 * @return count of packets.
 */
int Packet::getPacketCount() const { return packet_count; }

/**
 * @brief Setter for the source IP address.
 * 
 * @param src_ip source IP address of the packet.
 */
void Packet::setSrcIp(std::string src_ip) { this->src_ip = src_ip; }

/**
 * @brief Setter for the source port.
 * 
 * @param src_port source port of the packet.
 */
void Packet::setSrcPort(int src_port) { this->src_port = src_port; }

/**
 * @brief Setter for the destination IP address.
 * 
 * @param dst_ip destination IP address of the packet.
 */
void Packet::setDstIp(std::string dst_ip) { this->dst_ip = dst_ip; }

/**
 * @brief Setter for the destination port.
 * 
 * @param dst_port destination port of the packet.
 */
void Packet::setDstPort(int dst_port) { this->dst_port = dst_port; }

/**
 * @brief Setter for the protocol.
 * 
 * @param protocol protocol of the packet.
 */
void Packet::setProtocol(std::string protocol) { this->protocol = protocol; }

/**
 * @brief Setter for the length of the packet.
 * 
 * @param length length of the packet.
 */
void Packet::setLength(int length) { this->length = length; }

/**
 * @brief Setter for the received bytes.
 * 
 * @param rx received bytes of the packet.
 */
void Packet::setRx(int rx) { this->rx = rx; }

/**
 * @brief Setter for the transmitted bytes.
 * 
 * @param tx transmitted bytes of the packet.
 */
void Packet::setTx(int tx) { this->tx = tx; }

/**
 * @brief Setter for the count of received packets.
 * 
 * @param rx_packet_count count of received packets.
 */
void Packet::setRxPacketCount(int rx_packet_count) { this->rx_packet_count = rx_packet_count; }

/**
 * @brief Setter for the count of transmitted packets.
 * 
 * @param tx_packet_count count of transmitted packets.
 */
void Packet::setTxPacketCount(int tx_packet_count) { this->tx_packet_count = tx_packet_count; }

/**
 * @brief Setter for the count of packets.
 * 
 * @param packet_count count of packets.
 */
void Packet::setPacketCount(int packet_count) { this->packet_count = packet_count; }

/**
 * @brief Function that adds length to the current length of the packet.
 * 
 * @param length updated length after addition.
 */
void Packet::addLength(int length) { this->length += length; }

/**
 * @brief Function that adds received bytes to the current received bytes of the packet.
 * 
 * @param rx updated received bytes after addition.
 */
void Packet::addRx(int rx) { this->rx += rx; }

/**
 * @brief Function that adds transmitted bytes to the current transmitted bytes of the packet.
 * 
 * @param tx updated transmitted bytes after addition.
 */
void Packet::addTx(int tx) { this->tx += tx; }

/**
 * @brief Function that increments the count of received packets.
 * 
 * @param rx_packet_count updated count of received packets after increment.
 */
void Packet::addRxPacketCount(int rx_packet_count) { this->rx_packet_count += rx_packet_count; }

/**
 * @brief Function that increments the count of transmitted packets.
 * 
 * @param tx_packet_count updated count of transmitted packets after increment.
 */
void Packet::addTxPacketCount(int tx_packet_count) { this->tx_packet_count += tx_packet_count; }

/**
 * @brief Function that increments the count of packets.
 * 
 */
void Packet::addPacketCount() { this->packet_count += 1; }
