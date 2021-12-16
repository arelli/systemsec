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

/* the path to save the log file to */
static const char LOG_PATH[] = "file_logging.log";  // This is to be accessible by all : "/tmp/file_access.log";  // tmp is accessible by all users, but is deleted at reboot


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
    path[r] = '\0';  /* complete the string with a null character*/
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

/* Returns the time in a string. The time/date 
 * format is the following: 
 * "Fri Nov 26 22:02:56 2021"
 */
const char* get_time(){
	/* get time */
	time_t raw_time;
	struct tm * timeinfo;
	time ( &raw_time );
  	timeinfo = localtime ( &raw_time );
  	return asctime(timeinfo);
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

	/*  get md5 of file if not empty  */
	unsigned char *hash = NULL;
	/* access() returns 0 if file exists, and -1 else */
	if( access( path, F_OK ) == 0 ){ // if file does exist...
		hash = (unsigned char *) get_md5_from_path((char*)path);
	}
	else{
		access_type = 0;  // access type = file creation
		hash = (unsigned char*)malloc(MD5_DIGEST_LENGTH+1);
		for (int i=0; i < MD5_DIGEST_LENGTH; i++) 
			hash[i]= '\0';  /* fill the md5 result with zeroes */
	}

	/* call the original fopen */
	FILE * file_ptr;
	file_ptr = (*original_fopen)(LOG_PATH, "a");

	/* create the output log data buffer */
	char * output = (char * )malloc(sizeof(char)*256);  // a lot bigger than what we need
	int uid = getuid();

	/* call the original fopen function */
	original_fopen_ret = (*original_fopen)(path, mode);
	if(errno == EACCES  && original_fopen_ret == NULL)  // the error code returned by fopen, EACCESS= not sufficient priviledges
		action_denied = 1;  // DOES NOT work well on ntfs mounted systems bc they do not support the same privileges!

	/* create a string with all the info to write */
	sprintf(output, "%d,%s,%d,%d,", uid, (char*)get_path_from_fp(original_fopen_ret), access_type, action_denied);
	for (int i=0; i < MD5_DIGEST_LENGTH; i++) sprintf(output+strlen(output), "%02x",  hash[i]);
	sprintf(output + strlen(output), ",%s", get_time());  // add the strlen of output, to ont overwrite the previous data
	
	/* Write all the gathered info to the log file */
	/* find the original fwrite */
	size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");
	(*original_fwrite)(output,strlen(output),sizeof(char),file_ptr);  // actually write the data to log file

	/* Return the actual pointer to the file, the user at higher level requested */
	return original_fopen_ret;
}

FILE*
fopen64(const char *path, const char *mode){
        return fopen(path,mode);
}




size_t 
fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream) 
{
	FILE *(*original_fopen)(const char*, const char*);
	
	/* find the pointer to the original fopen */
	original_fopen = dlsym(RTLD_NEXT, "fopen");

	int action_denied = 0;
	int access_type =  2;  // action = file write
	size_t original_fwrite_ret;
	size_t (*original_fwrite)(const void*, size_t, size_t, FILE*);

	/* call the original fwrite function */
	original_fwrite = dlsym(RTLD_NEXT, "fwrite");
	original_fwrite_ret = (*original_fwrite)(ptr, size, nmemb, stream);
	
	/* this little magic line of code! writes everything to the file
	 * before fclose() is called. This way we can read the file 
	 * again a few lines later in logger.c, to get its md5 hash
	 *(of the newli writtend data!!)
	 */
	fflush(stream); 

	/* get the path of the file pointed by stream argument */
	char* path = (char *)get_path_from_fp(stream);  // the memory is allocated inside the function

	/* get md5 */
	unsigned char *hash = NULL;
	hash = (unsigned char*)get_md5_from_path(path);

	/* call the original fopen */
	FILE * file_ptr;
	file_ptr = (*original_fopen)(LOG_PATH, "a");

	/* to initialise log output buffer */
	char * output = (char * )malloc(sizeof(char)*256);  // a lot bigger than what we need(we need about 150 bites)
	int uid = getuid();

	/* Fill the output buffer with the data to be logged */
	sprintf(output, "%d,%s,%d,%d,", uid, path, access_type, action_denied);
	for (int i=0; i < MD5_DIGEST_LENGTH; i++) sprintf(output+strlen(output), "%02x",  hash[i]);
	sprintf(output + strlen(output), ",%s", get_time());  // add the strlen of output, to ont overwrite the previous data

	/* Write the output buffer */
	(*original_fwrite)(output,strlen(output),sizeof(char),file_ptr);  // actually write the data to log file

	/* return how many elements were successfully written 
	 * by fwrite() when the actual high-level call was made
	 */
	return original_fwrite_ret;
}


