#include"simple_crypto.h"  // our library


void main(){
	char text[] = "attackordont";
	
	int length = strlen(text);
	char * pad = (char*)malloc(length*sizeof(char));
	char* ciphertext = (char*)malloc(length*sizeof(char));
	char* deciphered = (char*)malloc(length*sizeof(char));

	// test the OTP algorithm
	pad = random_string(length);

	struct otp_response otp_rsp;
	otp_rsp.encrypted_text = (char*)malloc(length);
	otp_rsp.pad = (char*)malloc(length);

	otp_rsp = one_time_pad(text,pad);

	ciphertext = otp_rsp.encrypted_text;


	printf("\n The ciphertext of \"%s\" is :", text);
       	print_npc(ciphertext);
	printf("\n");


	char * new_pad = (char*)malloc(length);

	new_pad = otp_rsp.pad;

	otp_rsp = one_time_pad(new_pad,ciphertext);
	printf("\n The deciphered text of \"%s\" is: %s\n", ciphertext, otp_rsp.encrypted_text);
	print_npc(otp_rsp.encrypted_text);

	// CAESARS SHIFT
	ciphertext = caesar(text,5);
	printf("Caesar for text and offset 2 is: %s\n" , ciphertext);
	printf("Caesar decrypted: %s\n", caesar(ciphertext, -5));
}
