#include"simple_crypto.h"  // our library


void main(){
	random_string(5);
	printf("\n The ciphertext is %s \n", one_time_pad("attack","fatcat"));
}
