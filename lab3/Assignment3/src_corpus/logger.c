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

/* returns the full path of the file, from the file pointer.
 * works by getting the file descriptor using fileno(), and 
 * then the path of the proclink. This is Unix specific!! 
 * In the end, it returns the file path using readlink().
 * the "path" string is allocated inside the function.
 */
const char* get_path_from_fp(FILE* file_ptr){
	/* Get the file descriptor-> and then the path */
	int fno;  // stores the file descriptor from fileno()
	int MAXSIZE = 0xFFF;
	char proclnk[0xFFF];  // the link is stored here
	char *path = (char*)malloc(0xFFF);  // the filepath to return is stored here
	ssize_t r;  // stores the number of bytes returned by readlink()

	fno = fileno(file_ptr);  // the file pointer, from arguments
	sprintf(proclnk, "/proc/self/fd/%d", fno);  // works on Unix OS's only!Creates the full path of the proc link

	r = readlink(proclnk, path, MAXSIZE);  
    if (  r < 0){
        printf("failed to readlink()! Exiting..\n");
        exit(1);
    }
    path[r] = '\0';
    return path;
}

/* Returns the md5 hash from the contents of the file at the specified path. 
 * The returned value is also, self memory declared
 */
const char* get_md5_from_path(char* path){
	long filesize;
	unsigned char *buf;
	unsigned char *hash = NULL;

	FILE *original_fopen_ret;
	FILE *(*original_fopen)(const char*, const char*);
	original_fopen = dlsym(RTLD_NEXT, "fopen");

	original_fopen_ret = (*original_fopen)(path, "rb");
	if (original_fopen_ret==NULL){
		printf("Could not access file to generate md5 hash... Exiting...");
		return 0;
	}
	// send the cursor to the end of the file, measure length, and return it at the beginning
	fseek(original_fopen_ret, 0, SEEK_END);
	filesize = ftell(original_fopen_ret);
	fseek(original_fopen_ret, 0, SEEK_SET);

	buf = malloc(filesize);  // now that we know the filesize, we can allocate a buffer to hold it
	fread(buf, filesize, 1, original_fopen_ret);

	hash = malloc(MD5_DIGEST_LENGTH);  // hash length is always the same
	MD5(buf, filesize, hash);  // MD5 is actually produced here
	free(buf);
	fclose(original_fopen_ret);
	return (const char*)hash;
}


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


	/* H A S I N G  */
	unsigned char *hash = NULL;

	if( access( path, F_OK ) == 0 )   // if file does exist...	
		hash = (unsigned char *) get_md5_from_path(path);
	else{
		access_type = 0;  // file creation(file did not exist)
		hash = (unsigned char*)"0"; // md5 for empty file
	}
		

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

	/* call the original fopen function */
	original_fopen_ret = (*original_fopen)(path, mode);
	if(errno == EACCES  && original_fopen_ret == NULL){  // the error code returned by fopen, EACCESS= not sufficient priviledges
		action_denied = 1;
	}


	sprintf(output, "uid:%d, %s, access:%d, denied:%d,", uid, (char*)get_path_from_fp(original_fopen_ret), access_type, action_denied);
	for (int i=0; i < MD5_DIGEST_LENGTH; i++){
			sprintf(output+strlen(output), "%02x",  hash[i]);
	}
	sprintf(output + strlen(output), ", %s", asctime(timeinfo));  // add the strlen of output, to ont overwrite the previous data
	(*original_fwrite)(output,strlen(output),sizeof(char),file_ptr);  // actually write the data to log file

	return original_fopen_ret;
}


size_t 
fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) 
{
	//FILE * original_fopen_ret;
	FILE *(*original_fopen)(const char*, const char*);
	original_fopen = dlsym(RTLD_NEXT, "fopen");
	int action_denied = 0;
	int access_type =  2;  // action = file open

	
	size_t original_fwrite_ret;
	size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);

	/* call the original fwrite function */
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");
	original_fwrite_ret = (*original_fwrite)(ptr, size, nmemb, stream);



	char* path = (char *)get_path_from_fp(stream);  // the memory is allocated inside the function


	/************* tHe logger follows****************/

	/* H A S I N G */
	unsigned char *hash = NULL;
	hash = get_md5_from_path(path);


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

	sprintf(output, "uid:%d, %s, access:%d, denied:%d,", uid, path, access_type, action_denied);
	for (int i=0; i < MD5_DIGEST_LENGTH; i++){
			sprintf(output+strlen(output), "%02x",  hash[i]);
	}
	sprintf(output + strlen(output), ", %s", asctime(timeinfo));  // add the strlen of output, to ont overwrite the previous data
	(*original_fwrite)(output,strlen(output),sizeof(char),file_ptr);  // actually write the data to log file

	return original_fwrite_ret;
}


