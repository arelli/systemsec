#include"simple_crypto.h"  // our library


void main(){
	// ONE TIME PAD testing
	char buffer[101];
	printf("[OTP]input:           ");
	scanf(" %100[^\n]", buffer);
	char* text = (char *) malloc(strlen(buffer) + 1);
	strcpy(text, buffer);
	text = strip_special(text);

	int length = strlen(text);
	char * pad = (char*)malloc(length*sizeof(char));
	char* ciphertext = (char*)malloc(length*sizeof(char));

	pad  = random_string(length);  
	ciphertext = one_time_pad(text,pad);
	printf("[OTP]encrypted:      ");
    print_npc(ciphertext, length);  
	printf("[OTP]decrypted:      ");
    print_npc(one_time_pad(ciphertext, pad),length);  


	// CAESARS SHIFT testing
	printf("\n\n[CAESAR]input:           ");
	scanf(" %100[^\n]", buffer);
	text = (char *) malloc(strlen(buffer) + 1);
	strcpy(text, buffer);
	text = strip_special(text);

	length = strlen(text);
	pad = (char*)malloc(length*sizeof(char));
	ciphertext = (char*)malloc(length*sizeof(char));

	srand(time(NULL));
	int offset = rand() % 60;
	ciphertext = caesar(text,offset);
	printf("[CAESAR]encrypted:   %s\n" , ciphertext);
	printf("[CAESAR]key:         %d\n", offset);
	printf("[CAESAR]decrypted:   %s\n", caesar(ciphertext, (-1)*offset));


	//Vigenere Testing
	printf("\n\n[Vigenere]input:           ");
	scanf(" %100[^\n]", buffer);
	text = (char *) malloc(strlen(buffer) + 1);
	strcpy(text, buffer);
	text = strip_special(text);

	length = strlen(text);
	pad = (char*)malloc(length*sizeof(char));
	ciphertext = (char*)malloc(length*sizeof(char));


	pad =random_string_letters('A','Z',strlen(text));	
	ciphertext = vigenere(text,pad);

	printf("[Vigenere]key:       %s\n", pad);
	printf("[Vigenere]encrypted: %s\n" , ciphertext);
	printf("[Vigenere]decrypted: %s\n", vigenere(ciphertext,pad));



}
