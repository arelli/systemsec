#include"simple_crypto.h"  // our library


void main(){
	//char* text = strip_special("messAbgez");  

	char buffer[101];
	printf("[OTP]input: ");
	scanf(" %100[^\n]", buffer);
	char* text = (char *) malloc(strlen(buffer) + 1);
	strcpy(text, buffer);
	text = strip_special(text);
	//printf("The cleared string is: \"%s\"\n\n", text);  // debug
	
	int length = strlen(text);
	char * pad = (char*)malloc(length*sizeof(char));
	char* ciphertext = (char*)malloc(length*sizeof(char));

	// ONE TIME PAD testing
	pad  = random_string(length);  
	ciphertext = one_time_pad(text,pad);
	printf("[OTP]encrypted:");
    print_npc(ciphertext);  
	printf("[OTP]decrypted:");
    print_npc(one_time_pad(ciphertext, pad));  


	// CAESARS SHIFT testing
	ciphertext = caesar(text,5);
	printf("Caesar for text and offset 5 is: %s\n" , ciphertext);
	printf("Caesar decrypted(-5 offset): %s\n", caesar(ciphertext, -5));
}
