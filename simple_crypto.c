
// print not printable characters
void print_npc(char* npc_string){
	int i;
	for(i=0;i<=strlen(npc_string);i++){
		if(isprint(npc_string[i]))
			printf(" %c ", npc_string[i]);
		else
			printf(" %x ", npc_string[i]);
	}
	printf("\n");
}

char* random_string(int length){
	char* random_string = (char*)malloc(length*sizeof(char));
	int i;
	if (random_string==NULL){
		printf("Error when allocating memory for random string...Exiting..\n");
		return NULL;
	}
	FILE *fpointer;
	fpointer = fopen("/dev/urandom", "r");  // read file in binary mode
	fread(random_string,sizeof(char),length,fpointer);  // read somr bytes from fpointer position to random_string
	printf("\nThe random string generated is: ");
	// print the random string, even non printable characters as hex(%x)
	/*for(i=1;i<=length;i++){
		if (isprint(random_string[i]))
			printf(" %c ", random_string[i]);
		else
			printf(" %x ", random_string[i]);
	}
	printf("\n");
	*/
	print_npc(random_string);
	return random_string;

}


char* one_time_pad(char* text, char* pad){
	int length = strlen(text);  // it will count the \0 character at the end!
	int counter;
	char temp;  // temporary to hold value of input text character during xor
	char * output = (char*)malloc(length*sizeof(char));
	for (counter=0;counter<length;counter++){
		temp = text[counter]^pad[counter];
		output[counter] = temp;
	}
	output[counter] = '\0';  // add the end string character
	printf("This is the output before exiting: %s \n", output);
	return output;
}
