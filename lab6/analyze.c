/* A program that analyzes a pcap file and displays statistics */
#include<stdio.h>
#include<pcap.h>

void print_packet_info(const u_char *packet, struct pcap_pkthdr* packet_header);

void main(){
	char error_buffer[PCAP_ERRBUF_SIZE];
	struct pcap_pkthdr packet_header;
	pcap_t *handle ;
	handle = pcap_open_offline("test_pcap_5mins.pcap", error_buffer);
	/* use pcap_next() to get the packets one by one */
	u_char *packet =(u_char*) pcap_next(handle,&packet_header);  /* needs u_char* type but it's not standard! */
	while(packet!=NULL){
		print_packet_info(packet, &packet_header);
		packet =(u_char*) pcap_next(handle, &packet_header);
	}
}


void print_packet_info(const u_char *packet, struct pcap_pkthdr* packet_header) {
    printf("Packet capture length: %d\n", packet_header->caplen);
    printf("Packet total length %d\n", packet_header->len);
}
