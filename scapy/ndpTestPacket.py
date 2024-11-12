from scapy.all import *
from scapy.layers.inet6 import ICMPv6ND_NS, ICMPv6NDOptSrcLLAddr

# Target settings
target_ip = "::1"

# Create an NDP packet
ll_addr = "00:01:02:03:04:05"
packet = IPv6(dst=target_ip)/ICMPv6ND_NS(tgt=target_ip)/ICMPv6NDOptSrcLLAddr(lladdr=ll_addr)

# Send the packet
send(packet)
