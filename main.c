#include<stdio.h>
#include"simple_crypto.h"  // our library
#include"simple_crypto.c"

void main(){
	char* text="GETCOVERATDAWN";
	text = one_time_pad("ATTACKATDAWN","JTTWKDRTHGOI");
	random_string(5);
}
