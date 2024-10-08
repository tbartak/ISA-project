#include "packet_info.h"
#include <string>


Packet::Packet(std::string src_ip, int src_port, std::string dst_ip, int dst_port, std::string protocol, int length, int rx, int tx, int packet_count) : 
src_ip(src_ip), src_port(src_port), dst_ip(dst_ip), dst_port(dst_port), protocol(protocol), length(length), rx(rx), tx(tx), packet_count(packet_count) {}

Packet::Packet() : src_ip(""), src_port(-1), dst_ip(""), dst_port(-1), protocol(""), length(0), rx(0), tx(0), packet_count(0) {}

std::string Packet::getSrcIp() const { return src_ip; }

int Packet::getSrcPort() const { return src_port; }

std::string Packet::getDstIp() const { return dst_ip; }

int Packet::getDstPort() const { return dst_port; }

std::string Packet::getProtocol() const { return protocol; }

int Packet::getLength() const { return length; }

int Packet::getRx() const { return rx; }

int Packet::getTx() const { return tx; }

int Packet::getPacketCount() const { return packet_count; }

// set functions
void Packet::setSrcIp(std::string src_ip) { this->src_ip = src_ip; }

void Packet::setSrcPort(int src_port) { this->src_port = src_port; }

void Packet::setDstIp(std::string dst_ip) { this->dst_ip = dst_ip; }

void Packet::setDstPort(int dst_port) { this->dst_port = dst_port; }

void Packet::setProtocol(std::string protocol) { this->protocol = protocol; }

void Packet::setLength(int length) { this->length = length; }

void Packet::setRx(int rx) { this->rx = rx; }

void Packet::setTx(int tx) { this->tx = tx; }

void Packet::setPacketCount(int packet_count) { this->packet_count = packet_count; }

// add functions
void Packet::addLength(int length) { this->length += length; }
void Packet::addRx(int rx) { this->rx += rx; }
void Packet::addTx(int tx) { this->tx += tx; }
void Packet::addPacketCount() { this->packet_count += 1; }
