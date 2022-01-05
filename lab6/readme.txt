
Antonios Rafail Ellinitakis 2017030118

To begin with, I have installed the gcc version (Ubuntu 9.3.0-17ubuntu1~20.04) 9.3.0.

To compile the project, use make. Use "make clean" to remove compiled files and services catalog.
To run the program with the testfile provided(test_pcap_5mins.pcap), use "make run-testfile".


I didn't implement:
-> Network flows. Due to lack of time.(12.a, 12.b, 12.c)
-> Retransmission detection. It is redundant in the case of UDP packets, because they do not 
support retransmission. In the case of TCP packets, it requires knowledge from previous packets
and my program currently doesn not support that.(9,10,11)

One final note, is that due to the nature of higher-level-protocol-detection, the program is 
a bit slow, taking about 20 seconds to digest the whole example pcap file.
The protocol detection works fine, and uses the current services stored in the /etc/services 
location of the machine.


