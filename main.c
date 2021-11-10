#include"simple_crypto.h"  // our library


void main(){
	//char* text = strip_special("messAbgez");  
	char buffer[101];
	printf("[OTP]input:      ");
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
    print_npc(ciphertext, length);  
	printf("[OTP]decrypted:");
    print_npc(one_time_pad(ciphertext, pad),length);  

	// CAESARS SHIFT testing
	srand(time(NULL));
	int offset = rand() % 60;
	ciphertext = caesar(text,offset);
	printf("[CAESAR]input:   %s \n", text);
	printf("[CAESAR]encrypted: %s\n" , ciphertext);
	printf("[CAESAR]key:       %d\n", offset);
	printf("[CAESAR]decrypted: %s\n", caesar(ciphertext, (-1)*offset));
}
