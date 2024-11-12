from scapy.all import *
from scapy.contrib import *

# Target settings
target_ip = "::1"

# Create an MLD packet
mld_report = ICMPv6MLReport2()
mld_report.numaddr = 1
mld_record = ICMPv6MLDMultAddrRec(rtype=4, dst="ff02::1")
packet = IPv6(dst=target_ip)/mld_report/mld_record

# Send the packet
send(packet)
