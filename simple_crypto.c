#include<stdio.h>
#include<ctype.h>  // for checking if a character is printable
#include<stdlib.h>
#include<string.h>
#include"simple_crypto.h"

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
	for(i=1;i<=length;i++){
		if (isprint(random_string[i]))
			printf(" %c ", random_string[i]);
		else
			printf(" %x ", random_string[i]);
	}
	printf("\n");
	return random_string;

}


char* one_time_pad(char* text, char* pad){
	printf("This is the text returned:%s", text);
	return text;
}
