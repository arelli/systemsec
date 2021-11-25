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
	int access_type =  1;  // action = file open
	/* get the pointer to the original fopen we wrap: */
	original_fopen = dlsym(RTLD_NEXT, "fopen");

	/* find the original fwrite */
	size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");


	/* H A S I N G  A L G O R I T H M */
	long filesize;
	unsigned char *buf;
	unsigned char *hash = NULL;
	int i;

	if( access( path, F_OK ) == 0 ) {  // if file does exist...	
		/* call the original fopen function */
		original_fopen_ret = (*original_fopen)(path, "rb");
		if (original_fopen_ret==NULL){
			printf("Could not access file to generate md5 hash... Exiting...");
			return 0;
		}
		// send the cursor to the end of the file, measure length, and return it at the beginning
		fseek(original_fopen_ret, 0, SEEK_END);
		filesize =10; //  ftell(original_fopen_ret);
		fseek(original_fopen_ret, 0, SEEK_SET);

		buf = malloc(filesize);  // now that we know the filesize, we can allocate a buffer to hold it
		fread(buf, filesize, 1, original_fopen_ret);

		hash = malloc(MD5_DIGEST_LENGTH);  // hash length is always the same
		MD5(buf, filesize, hash);  // MD5 is actually produced here
		free(buf);
		fclose(original_fopen_ret);
		/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/
	}
	else{
		access_type = 0;  // file creation(file did not exist)
		hash = (unsigned char*)"0"; // md5 for empty file
	}
		

	/* get time */
	time_t raw_time;
	struct tm * timeinfo;
	time ( &raw_time );
  	timeinfo = localtime ( &raw_time );


	/* write to log file*/

	/* find the original fopen */
	FILE * file_ptr;
	file_ptr = (*original_fopen)("log", "a");

	char * output = (char * )malloc(sizeof(char)*256);  // a lot bigger than what we need
	int uid = getuid();

	sprintf(output, "uid:%d, %s, access:%d, denied:%d,", uid, path, access_type, action_denied);
	for (i=0; i < MD5_DIGEST_LENGTH; i++){
			sprintf(output+strlen(output), "%02x",  hash[i]);
	}
	sprintf(output + strlen(output), ", %s", asctime(timeinfo));  // add the strlen of output, to ont overwrite the previous data
	(*original_fwrite)(output,strlen(output),sizeof(char),file_ptr);  // actually write the data to log file


	/* call the original fopen function */
	original_fopen_ret = (*original_fopen)(path, mode);
	if(errno == EACCES  && original_fopen_ret == NULL){  // the error code returned by fopen, EACCESS= not sufficient priviledges
		action_denied = 1;
	}

	return original_fopen_ret;
}


size_t 
fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) 
{
	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char*, const char*);
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	int action_denied = 0;
	int access_type =  2;  // action = file open



	/* Get the file descriptor-> and then the path */
	int fno;
	int MAXSIZE = 0xFFF;
	char proclnk[0xFFF];  // the link is stored here
	char path[0xFFF];
	ssize_t r;

	fno = fileno(stream);  // the file pointer, from arguments
	sprintf(proclnk, "/proc/self/fd/%d", fno);  // works on Unix OS's only!Creates the full path of the proc link

	r = readlink(proclnk, path, MAXSIZE);
    if (  r < 0){
        printf("failed to readlink()! Exiting..\n");
        exit(1);
    }
    path[r] = '\0';





	size_t original_fwrite_ret;
	size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);


	/* call the original fwrite function */
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");
	original_fwrite_ret = (*original_fwrite)(ptr, size, nmemb, stream);



	/************* tHe logger follows****************/

	/* H A S I N G  A L G O R I T H M */
	long filesize;
	unsigned char *buf;
	unsigned char *hash = NULL;
	int i;

	original_fopen_ret = (*original_fopen)(path, "rb");
	if (original_fopen_ret==NULL){
			printf("Could not access file to generate md5 hash... Exiting...");
			return 0;
	}

	fseek(original_fopen_ret, 0, SEEK_END);
	filesize =10; //  ftell(original_fopen_ret);
	fseek(original_fopen_ret, 0, SEEK_SET);

	buf = malloc(filesize);  // now that we know the filesize, we can allocate a buffer to hold it
	fread(buf, filesize, 1, original_fopen_ret);

	hash = malloc(MD5_DIGEST_LENGTH);  // hash length is always the same
	MD5(buf, filesize, hash);  // MD5 is actually produced here
	free(buf);
	fclose(original_fopen_ret);

	/* get time */
	time_t raw_time;
	struct tm * timeinfo;
	time ( &raw_time );
  	timeinfo = localtime ( &raw_time );



	/* find the original fopen */
	FILE * file_ptr;
	file_ptr = (*original_fopen)("log", "a");

	char * output = (char * )malloc(sizeof(char)*256);  // a lot bigger than what we need
	int uid = getuid();

	sprintf(output, "uid:%d,%s access:%d, denied:%d,", uid, path, access_type, action_denied);
	for (i=0; i < MD5_DIGEST_LENGTH; i++){
			sprintf(output+strlen(output), "%02x",  hash[i]);
	}
	sprintf(output + strlen(output), ", %s", asctime(timeinfo));  // add the strlen of output, to ont overwrite the previous data
	(*original_fwrite)(output,strlen(output),sizeof(char),file_ptr);  // actually write the data to log file

	return original_fwrite_ret;
}


