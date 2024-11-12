from scapy.all import *
from scapy.contrib.igmp import IGMP

# Target settings
target_ip = "127.0.0.1"

# Create an IGMP packet
packet = IP(dst=target_ip)/IGMP(type=0x22, mrcode=0, gaddr="224.0.0.1")

# Send the packet
send(packet)
