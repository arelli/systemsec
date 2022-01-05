/* A program that analyzes a pcap file and displays statistics */
#include<stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <net/if.h>
#include <netinet/if_ether.h>
#include <net/ethernet.h>
#include <netinet/tcp.h>
#include <netinet/udp.h>
#include <arpa/inet.h>  /* for inet_ntop() functions */
#include<pcap.h>

#define PCAP_BUF_SIZE	1024
#define PCAP_SRC_FILE	2

void print_packet_info(const u_char *packet, struct pcap_pkthdr* packet_header);
void get_stats(pcap_t* handle, char* filter_exp);

void main(){
	char error_buffer[PCAP_ERRBUF_SIZE];
	pcap_t *handle ;
	handle = pcap_open_offline("test_pcap_5mins.pcap", error_buffer);
	get_stats(handle,"udp || tcp");
	pcap_close(handle);
	
}


void print_packet_info(const u_char *packet, struct pcap_pkthdr* packet_header) {
    printf("Packet capture length: %d\n", packet_header->caplen);
    printf("Packet total length %d\n", packet_header->len);
}

void get_stats(pcap_t* handle, char* filter_exp){
	const struct tcphdr* tcp_header;
    const struct udphdr* udp_header;
	const struct ether_header* ethernet_header;
    const struct ip* ip_header;
    char sourceIP[INET_ADDRSTRLEN];
    char destIP[INET_ADDRSTRLEN];
    u_int source_port, dest_port;
    u_char *data;
    int data_length = 0;
    int i;
    int tcp_counter = 0;
    int udp_counter = 0;


	struct pcap_pkthdr packet_header;
	/* define the neccessary variables for TCP and UDP filtering */
	struct bpf_program filter;
	//char filter_exp[] = "tcp || udp";
	bpf_u_int32 subnet_mask, ip;

	int packet_counter = 0;

	/* this is the main loop reading packets. with each pcap_next() call,
	 * a new packet is read from the stream(.pcap file in our case) 
	 * which indicates end of capture
	 */
	u_char *packet =(u_char*) pcap_next(handle,&packet_header);
	while(packet!=NULL){
		//print_packet_info(packet, &packet_header);
		printf("packet #%i:", packet_counter);

		if (packet_header.len != packet_header.caplen)
            printf("!:Capture size different than packet size!");

		int payload_length = packet_header.len; /* this is NOT always the captured length */

		ethernet_header = (struct ether_header*)packet;
		/* check if the packet is IP */
	    if (ntohs(ethernet_header->ether_type) == ETHERTYPE_IP) {
	    	/* clear off the offset of the ethernet header! */
	        ip_header = (struct ip*)(packet + sizeof(struct ether_header));
	        /* get src ip and dsk ip from the packet. inet_ntop translates
	         * the addresses into human readable format.
	         */
	        inet_ntop(AF_INET, &(ip_header->ip_src), sourceIP, INET_ADDRSTRLEN);
	        inet_ntop(AF_INET, &(ip_header->ip_dst), destIP, INET_ADDRSTRLEN);
	        printf(" source IP: %s dest IP: %s", sourceIP,destIP);

	        if(ip_header->ip_p == IPPROTO_TCP){
	        	printf(" [TCP] \n");
	        	tcp_counter++;
	        }
	        else if (ip_header->ip_p == IPPROTO_UDP){
	        	printf(" UDP \n");
	        	udp_counter++;
	        }


	    }

	    /* get the next packet */
	    packet =(u_char*) pcap_next(handle, &packet_header);
		packet_counter++;
	}

	printf("\n The total number of packets UDP(%d) or TCP(%d): %d \n ",udp_counter,tcp_counter, packet_counter);

}
