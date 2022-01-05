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
void get_stats(pcap_t* handle	);

void main(int argc, char *argv[]){
	char error_buffer[PCAP_ERRBUF_SIZE];
	pcap_t *handle ;
	if ((argc<=2)||(strcmp(argv[1],"-r")!=0)){
		printf("You need to specify -r <pcap filename>!\nExiting... \n");
		return;
	}
	handle = pcap_open_offline(argv[2], error_buffer);
	get_stats(handle);
	pcap_close(handle);
	return;
}


void print_packet_info(const u_char *packet, struct pcap_pkthdr* packet_header) {
    printf("Packet capture length: %d\n", packet_header->caplen);
    printf("Packet total length %d\n", packet_header->len);
}

void get_stats(pcap_t* handle){
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
    int tcp_bytes_counter = 0;
    int udp_counter = 0;
    int udp_bytes_counter = 0;

	/* get all services with port numbers. get_sevices.sh must be run before.
     */
    FILE* fp;
    char line[256];
    /* get_services.sh is tested only on Ubuntu 20.04.3 LTS */
	fp = fopen("services", "r");


	struct pcap_pkthdr packet_header;
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


	        /* if the packet is TCP: */
	        if(ip_header->ip_p == IPPROTO_TCP){
	        	/* get clear of the ethernet and ip headers */
	        	tcp_header = (struct tcphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));
	        	/* find destination and source ports: */
	        	source_port = ntohs(tcp_header->source);
            	dest_port = ntohs(tcp_header->dest);

            	int dst_prot_found=0;
            	int src_prot_found=0;
            	printf("type: TCP source IP: %s dest IP: %s", sourceIP,destIP);
            	/* find protocol of source port number */
            	while (fgets(line, sizeof(line), fp) != NULL) {
            		char port_number[16];
            		sprintf(port_number, "%d", source_port);
            		/* if the port number is found in the "database"... */
				    if (strstr(line, port_number) != NULL) {
				    	/* the interesting offset below is to print characters only after
				    	 * the whitespace character. this is depentend on the format of
				    	 * our command.
				    	 */
				    	*(line+(strlen(line)-1))=' '; // remove trailing newline
				        printf(" src port: %d ( %s)", source_port, line+(int)(strchr(line,' ')-line+1) );
				        src_prot_found=1;
				        break;
				    }
				}
				if(!src_prot_found)
					printf(" src port: %d", source_port);
				rewind(fp);

				/* find protocol of destination port number */
				while (fgets(line, sizeof(line), fp) != NULL) {
            		char port_number[16];
            		sprintf(port_number, "%d", dest_port);
				    if (strstr(line, port_number) != NULL) {
				    	*(line+(strlen(line)-1))=' ';
				        printf(" dst port: %d ( %s)\n", dest_port, line+(int)(strchr(line,' ')-line+1) );
				        dst_prot_found=1;
				        break;
				    }
				}
				if(!dst_prot_found)
					printf(" dst port: %d\n", dest_port);
				rewind(fp);

	        	tcp_counter++;
	        	tcp_bytes_counter += packet_header.len;
	        }

	        /* if the packet is UDP: */
	        else if (ip_header->ip_p == IPPROTO_UDP){
	        	/* get clear of the ethernet and ip headers */
	        	udp_header = (struct udphdr*)(packet + sizeof(struct ether_header) + sizeof(struct ip));

	        	source_port = ntohs(udp_header->source);
            	dest_port = ntohs(udp_header->dest);

	        	printf("type: UDP source IP: %s dest IP: %s", sourceIP,destIP);

	        	int dst_prot_found=0;
            	int src_prot_found=0;
            	while (fgets(line, sizeof(line), fp) != NULL) {
            		char port_number[16];
            		sprintf(port_number, "%d", source_port);
            		/* if the port number is found in the "database"... */
				    if (strstr(line, port_number) != NULL) {
				    	/* the interesting offset below is to print characters only after
				    	 * the whitespace character. this is depentend on the format of
				    	 * our command.
				    	 */
				    	*(line+(strlen(line)-1))=' ';
				        printf(" src port: %d ( %s) ", source_port, line+(int)(strchr(line,' ')-line+1));
				        src_prot_found=1;
				        break;
				    }
				}
				if(!src_prot_found)
					printf(" src port: %d", source_port);
				rewind(fp);

				/* find protocol of destination port number */
				while (fgets(line, sizeof(line), fp) != NULL) {
            		char port_number[16];
            		sprintf(port_number, "%d", dest_port);
				    if (strstr(line, port_number) != NULL) {
				    	*(line+(strlen(line)-1))=' ';
				        printf(" dst port: %d ( %s)\n", dest_port, line+(int)(strchr(line,' ')-line+1));
				        dst_prot_found=1;
				        break;
				    }
				}
				if(!dst_prot_found)
					printf(" dst port: %d\n", dest_port);
				rewind(fp);

	        	udp_counter++;
	        	udp_bytes_counter += packet_header.len;
	        }


	    }

	    /* get the next packet */
	    packet =(u_char*) pcap_next(handle, &packet_header);
		packet_counter++;
	}
	/* close the "database" of services/ports */
	printf("\n The total number of UDP packets: %d and TCP packets: %d\n Total packets: %d \n ",udp_counter,tcp_counter, packet_counter);
	printf("\n The total TCP bytes: %d , total UDP bytes: %d \n", tcp_bytes_counter, udp_bytes_counter);

}

