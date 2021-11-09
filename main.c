#include"simple_crypto.h"  // our library


void main(){
	char text[] = "message!!";
	
	int length = strlen(text);
	char * pad = (char*)malloc(length*sizeof(char));
	char* ciphertext = (char*)malloc(length*sizeof(char));
	char* deciphered = (char*)malloc(length*sizeof(char));

	// ONE TIME PAD testing
	pad  = random_string(length);

	ciphertext = one_time_pad(text,pad);

	printf("\n The ciphertext of \"%s\" (with pad=%s) is :", text, pad);
    //print_npc(ciphertext);  // debug
	printf("\n");


	deciphered = one_time_pad(ciphertext, pad);

	printf("\n The deciphered text of \"%s\" with key \"%s\" is: %s\n\n\n", ciphertext,pad, deciphered);


	// CAESARS SHIFT testing
	ciphertext = caesar(text,5);
	printf("Caesar for text and offset 5 is: %s\n" , ciphertext);
	printf("Caesar decrypted(-5 offset): %s\n", caesar(ciphertext, -5));
}
