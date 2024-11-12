from scapy.all import *

# Target settings
target_ip = "127.0.0.1"

# Create an ICMPv4 Echo Request packet with custom data
content = "Hello ICMPv4"
packet = IP(dst=target_ip)/ICMP(type="echo-request")/Raw(load=content)

# Send the packet
send(packet)
