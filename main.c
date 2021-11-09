#include"simple_crypto.h"  // our library


void main(){
	char* text="GETCOVERATDAWN";
	text = one_time_pad("ATTACKATDAWN","JTTWKDRTHGOI");
	random_string(5);
}
