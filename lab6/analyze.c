/* A program that analyzes a pcap file and displays statistics */
#include<stdio.h>
#include<pcap.h>

void print_packet_info(const u_char *packet, struct pcap_pkthdr* packet_header);

void main(){
	char error_buffer[PCAP_ERRBUF_SIZE];
	struct pcap_pkthdr packet_header;
	pcap_t *handle ;
	handle = pcap_open_offline("test_pcap_5mins.pcap", error_buffer);

	/* define the neccessary variables for TCP and UDP filtering */
	struct bpf_program filter;
	char filter_exp[] = "tcp || udp";
	bpf_u_int32 subnet_mask, ip;

	int packet_counter = 0;

	/* compile and set the filters(contained in string filter_exp */
	if (pcap_compile(handle, &filter, filter_exp, 0, ip) == -1) {
	          printf("Bad filter - %s\n", pcap_geterr(handle));
	          return;
	}
	if (pcap_setfilter(handle, &filter) == -1) {
	          printf("Error setting filter - %s\n", pcap_geterr(handle));
	          return;
	}

	/* this is the main loop reading packets. with each pcap_next() call,
	 * a new packet is read from the stream(.pcap file in our case) 
	 * which indicates end of capture
	 */
	u_char *packet =(u_char*) pcap_next(handle,&packet_header);
	while(packet!=NULL){
		print_packet_info(packet, &packet_header);
		/* pcap_next "returns a pointer to the packet data" */
		packet =(u_char*) pcap_next(handle, &packet_header);
		int payload_length = packet_header.len;
		if (payload_length > 0) {
		        const u_char *temp_pointer = packet;
		        int byte_count = 0;
		        while (byte_count++ < payload_length) {
		            printf("%c", *temp_pointer);
	                    temp_pointer++;
		        }
		        printf("\n");
		}
		packet_counter++;
	}
	printf("\n The total number of packets UDP or TCP: %d \n ", packet_counter);
}


void print_packet_info(const u_char *packet, struct pcap_pkthdr* packet_header) {
    printf("Packet capture length: %d\n", packet_header->caplen);
    printf("Packet total length %d\n", packet_header->len);
}
