#include"simple_crypto.h"  // our library


void main(){
	char text[] = "attackordont";
	
	int length = strlen(text);
	char * pad = (char*)malloc(length*sizeof(char));
	char* ciphertext = (char*)malloc(length*sizeof(char));
	char* deciphered = (char*)malloc(length*sizeof(char));

	pad = random_string(length);
	ciphertext = one_time_pad(text,pad);

	printf("\n The ciphertext of \"%s\" is :", text);
       	print_npc(ciphertext);
	printf("\n");

	deciphered = one_time_pad(pad,ciphertext);
	printf("\n The deciphered text of \"%s\" is: %s\n", ciphertext, deciphered);

	
}
