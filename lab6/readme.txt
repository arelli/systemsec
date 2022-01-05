


To begin with, I have installed the gcc version 11.1.0. The first thing I had to do in order
to analyze the pcap file, was to instal libpcap.
--> $paru -S libcpap
Because I am on Arch Linux(5.15.7-arch1-1), the packets are not divided in developer and 
non-developer.Paru is a packet installer/helper, and behaves like the apt utility in ubuntu.

To compile the code, the flag -lpcap is needed:
--> $gcc <c_filename> -o <output_filename> -lpcap



