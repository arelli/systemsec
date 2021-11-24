#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/md5.h>

int get_uid_of_file(const char * filename){
	int uid;
	uid = getuid();  // returns UID of the current(calling) process.
	return uid;
}






FILE *
fopen(const char *path, const char *mode) 
{

	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char*, const char*);

	/* call the original fopen function */
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	original_fopen_ret = (*original_fopen)(path, mode);


	/* find the original fopen */
	FILE * file_ptr;
	file_ptr = (*original_fopen)("log", "a");
	/* find the original fwrite */
	size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");
	/*dummy write to the file*/
	int action_denied = 0;
	int access_type =  1;
	int time = 1630;
	char* fingerprint = "13219f13031g138d";
	char * output = (char * )malloc(sizeof(char)*256);  // a lot bigger than what we need
	int uid = getuid();
	sprintf(output, "%d,%s,%d,%d,%d,%s\n", uid, path,time,access_type,action_denied, fingerprint);
	(*original_fwrite)(output,strlen(output),sizeof(char),file_ptr);


	/* add your code here */
	// get UID of file
	// Get filename
	// Get timestamp
	// Access type(file creation = 0, file open = 1, file write = 2)
	// Is-Action-denied flag(user with no priviledges tried to access it)
	// File Fingerprint(i can use md5 hash function openssl)

	/* ... */
	/* ... */
	/* ... */
	/* ... */


	return original_fopen_ret;
}


size_t 
fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) 
{

	size_t original_fwrite_ret;
	size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);


	/* call the original fwrite function */
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");
	original_fwrite_ret = (*original_fwrite)(ptr, size, nmemb, stream);

	/* find the original fopen */
	FILE *(*original_fopen)(const char*, const char*);
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	FILE * file_ptr;
	file_ptr = (*original_fopen)("log", "a");



	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */


	return original_fwrite_ret;
}


