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


FILE *
fopen(const char *path, const char *mode) 
{
	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char*, const char*);
	int action_denied = 0;
	int access_type =  1;  // file open
	/* get the pointer to the original fopen we wrap: */
	original_fopen = dlsym(RTLD_NEXT, "fopen");


	
	if( access( path, F_OK ) != 0 ) {  // if file does not exist...
		access_type = 0;  // file creation
	} 

	/* call the original fopen function */
	original_fopen_ret = (*original_fopen)(path, mode);
	if(errno == EACCES  && original_fopen_ret == NULL){  // the error code returned by fopen, EACCESS= not sufficient priviledges
		action_denied = 1;
	}

	/* find the original fopen */
	FILE * file_ptr;
	file_ptr = (*original_fopen)("log", "a");

	/* find the original fwrite */
	size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");

	/* H A S I N G  A L G O R I T H M */
	FILE *fh;
	long filesize;
	unsigned char *buf;
	unsigned char *hash = NULL;
	int i;

	fh = (*original_fopen)(path, "r");

	fseek(fh, 0L, SEEK_END);
	filesize = ftell(fh);
	fseek(fh, 0L, SEEK_SET);

	buf = malloc(filesize);
	fread(buf, filesize, 1, fh);
	fclose(fh);

	hash = malloc(MD5_DIGEST_LENGTH);
	MD5(buf, filesize, hash);
	printf("MD5 (%s) = ", path);
	for (i=0; i < MD5_DIGEST_LENGTH; i++){
		printf("%02x",  hash[i]);
	}
	printf("\n");
	//free(hash);
	free(buf);
	/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

	/*dummy write to the file*/
	char * output = (char * )malloc(sizeof(char)*256);  // a lot bigger than what we need
	int uid = getuid();

	/* get time */
	time_t raw_time;
	struct tm * timeinfo;
	time ( &raw_time );
  	timeinfo = localtime ( &raw_time );

	sprintf(output, "uid:%d, %s, access:%d, denied:%d, %s, %s", uid, path, access_type, action_denied, hash, asctime(timeinfo));
	(*original_fwrite)(output,strlen(output),sizeof(char),file_ptr);

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
	//FILE * file_ptr;
	//file_ptr = (*original_fopen)("log", "a");

	//int access_type =  2;  // file write


	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */


	return original_fwrite_ret;
}


