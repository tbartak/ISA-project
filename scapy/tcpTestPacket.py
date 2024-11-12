from scapy.all import *

# Target settings
target_ip = "127.0.0.1"

target_dport1 = 8080
target_dport2 = 4567

target_sport1 = 4040
target_sport2 = 1234

# Create a TCP packet with custom content
content1 = "Hello TCP 1!"
packet1 = IP(dst=target_ip)/TCP(sport=target_sport1, dport=target_dport1)/Raw(load=content1)

content2 = "Hello TCP 2!"
packet2 = IP(dst=target_ip)/TCP(sport=target_sport2, dport=target_dport2)/Raw(load=content2)

# Send the packet
send(packet1)
send(packet2)