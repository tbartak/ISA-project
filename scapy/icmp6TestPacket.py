from scapy.all import *
from scapy.layers.inet6 import ICMPv6EchoRequest

# Target settings
target_ip = "::1"

# Create an ICMPv6 Echo Request packet with custom content
content = "Hello ICMPv6"
packet = IPv6(dst=target_ip)/ICMPv6EchoRequest(data=content)

# Send the packet
send(packet)
