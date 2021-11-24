#define _GNU_SOURCE

#include <time.h>
#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <openssl/md5.h>

#include <errno.h>  // to get access-denied errno from fopen
#include <unistd.h>  // to check if a file exists in the same dir

int get_uid_of_file(const char * filename){
	int uid;
	uid = getuid();  // returns UID of the current(calling) process.
	return uid;
}






FILE *
fopen(const char *path, const char *mode) 
{
	int access_type =  1;  // file open
	if( access( path, F_OK ) != 0 ) {  // if file does not exists
		access_type = 0;  // file creation
	} 

	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char*, const char*);

	/* call the original fopen function */
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	original_fopen_ret = (*original_fopen)(path, mode);
	int action_denied = 0;
	if(errno == EACCES  && original_fopen_ret == NULL){  // the error code returned by fopen, EACCESS= not sufficient priviledges
		action_denied = 1;
	}



	/* find the original fopen */
	FILE * file_ptr;
	file_ptr = (*original_fopen)("log", "a");

	/* find the original fwrite */
	size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");

	/*dummy write to the file*/
	char * output = (char * )malloc(sizeof(char)*256);  // a lot bigger than what we need
	int uid = getuid();

	/* get time */
	time_t raw_time;
	struct tm * timeinfo;
	time ( &raw_time );
  	timeinfo = localtime ( &raw_time );

  	/* do the MD5 stuff */
  	unsigned char c[MD5_DIGEST_LENGTH];
  	MD5_CTX mdContext;
  	MD5_Init(&mdContext);
  	unsigned char data[1024];
  	int bytes;
  	while ((bytes = fread(data, 1, 1024, original_fopen_ret)) != 0)
        MD5_Update (&mdContext, data, bytes);
    MD5_Final (c,&mdContext);

    char* hash = (char*)malloc(sizeof(char)*128/8+1);  // MD5 returns a 128-bit key always
    printf("[MD5]: %s\n\n", c);
	/* convert array of integers to string */
    for (int i; i<MD5_DIGEST_LENGTH; i++){
    	sprintf(hash,"%d",(int)c[i]);
    }



	sprintf(output, "uid:%d, %s, access:%d, denied:%d, %s, %s", uid, path, access_type, action_denied, hash, asctime(timeinfo));
	(*original_fwrite)(output,strlen(output),sizeof(char),file_ptr);


	/* add your code here */
	// get UID of file X
	// Get filename X
	// Get timestamp X
	// Access type(file creation = 0, file open = 1, file write = 2) X
	// Is-Action-denied flag(user with no priviledges tried to access it) X
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

	int access_type =  2;  // file write


	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */


	return original_fwrite_ret;
}


