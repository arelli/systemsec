#include"simple_crypto.h"  // our library


void main(){
	char text[] = "abb";
	
	int length = strlen(text);
	char * pad = (char*)malloc(length*sizeof(char));
	char* ciphertext = (char*)malloc(length*sizeof(char));
	char* deciphered = (char*)malloc(length*sizeof(char));

	// test the OTP algorithm
	pad = random_string(length);

	struct otp_response *otp_rsp;

	otp_rsp = (struct otp_response *)malloc(sizeof(struct otp_response));
	otp_rsp->encrypted_text = (char*)malloc(length);
	otp_rsp->pad = (char*)malloc(length);


	*otp_rsp = one_time_pad(text,pad);

	ciphertext = otp_rsp->encrypted_text;
	pad = otp_rsp->pad;
	printf("\n The ciphertext of \"%s\" (with pad=%s) is :", text, pad);
    print_npc(ciphertext);  // debug
	printf("\n");


	char * new_pad = (char*)malloc(length);

	new_pad = otp_rsp->pad;

	*otp_rsp = one_time_pad(new_pad,ciphertext);
	deciphered = otp_rsp->encrypted_text;
	pad = otp_rsp->pad;

	printf("\n The deciphered text of \"%s\" with key \"%s\" is: %s\n\n\n", ciphertext,pad, deciphered);


	// CAESARS SHIFT
	ciphertext = caesar(text,5);
	printf("Caesar for text and offset 5 is: %s\n" , ciphertext);
	printf("Caesar decrypted(-5 offset): %s\n", caesar(ciphertext, -5));
}
