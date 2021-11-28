#define _GNU_SOURCE

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <unistd.h>

/* from my 1st lab, produces random text */
char* random_string(int length){
	FILE *(*original_fopen)(const char*, const char*);
	
	/* find the pointer to the original fopen */
	original_fopen = dlsym(RTLD_NEXT, "fopen");


	char* random_string = (char*)malloc(length*sizeof(char));
	int i;
	if (random_string==NULL){
		printf("Error when allocating memory for random string...Exiting..\n");
		return NULL;
	}

	/* because we have preloaded our custom fopen, the original is called here
	 * to not interfere with the logs.
	 */
	FILE *fpointer = (*original_fopen)("/dev/urandom", "r");  // read file in binary mode
	fread(random_string,sizeof(char),length,fpointer);  // read somr bytes from fpointer position to random_string
	fclose(fpointer);

	return random_string;
}


int main() 
{
	int i;
	size_t bytes;
	FILE *file;
	char filenames[10][7] = {"file_0", "file_1", 
			"file_2", "file_3", "file_4",
			"file_5", "file_6", "file_7", 		
			"file_8", "file_9"};


	/* example source code */

	for (i = 0; i < 10; i++) {

		file = fopen(filenames[i], "w");
		if (file == NULL) 
			printf("fopen error\n");
		else {
			bytes = fwrite(filenames[i], strlen(filenames[i]), 1, file);
			/* in some cases, write some extra, random characters */
			//if ( (*random_string(1))>120 )
			//	fwrite(random_string(5), 5, 1, file);
			fclose(file);
		}

	}


	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */


}
