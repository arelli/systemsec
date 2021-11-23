#include <stdio.h>

int main(){
	FILE* file_ptr = fopen("test.txt", "r");
	printf("Calling the fopen() function..\n");
	if (file_ptr!=NULL){
		printf("Successfully opened test.txt!\n");
		return 1;
	}
	printf("Failed to open file. Exiting..\n");
	return 0;
}