#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* the path to save the log file to */
static const char LOG_PATH[] = "file_logging.log";  // This is to be accessible by all :"/tmp/file_access.log";

typedef struct entry_log {

	char uid[8]; /* user id, max is 65536 in Linux */
	char access_type[2]; /* access type values [0-2] */
	char action_denied[2]; /* is action denied values [0-1] */

	char  date_time[32]; /* file access date and time, max length 29 in current format*/

	char file[256]; /* 256 to save space. Should be 4096 bytes long. This could create sigsegvs!*/
	char fingerprint[64]; /* file md5 digest, 32 bytes long */
} entry;


struct file_history{
	// list of entries for this file
};
	


void
usage(void)
{
	printf(
	       "\n"
	       "usage:\n"
	       "\t./monitor \n"
		   "Options:\n"
		   "-m, Prints malicious users\n"
		   "-i <filename>, Prints table of users that modified "
		   "the file <filename> and the number of modifications\n"
		   "-e prints the files that were encrypted by the ransomware(lab5)\n"
		   "-v prints how many files were encrypted by the ransomware "
		   " the last 20 minutes.(lab5)\n"
		   "-h, Help message\n\n"
		   );

	exit(1);
}


void 
list_unauthorized_accesses(FILE *log)
{
	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */

	return;

}


void
list_file_modifications(FILE *log, char *file_to_scan)
{
	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */

	return;
}

void 
list_encrypted_files(FILE *log){
	/* add code here */
}

void 
how_many_creations(FILE* log){
	/* add code here */
}


int 
main(int argc, char *argv[])
{
	/* get the log file */
 	FILE *log;
	log = fopen(LOG_PATH,"r");
	if (log == NULL) {
		printf("Error opening log file at \"%s\"\n", LOG_PATH);
		return 1;
	}

	/* parse the file */
	unsigned long lines = 0;
	while(!feof(log))  /* while the end of file is not reached */
	  if(fgetc(log) == '\n')  /* whenever a newline character is found, increment lines */
	    lines++;

	/* return the "cursor" at the beginning of the file */
	fseek(log, 0, SEEK_SET);

	printf("The lines counted are: %lu\n", lines);
	/* Load data from the log file in a array of structs */
	int buffer_length = 1024;
	char buffer[buffer_length];
	char dummy_buf[200];

	entry * entry_list;
	entry e1;
	entry_list = (entry*)malloc(lines*sizeof(entry)+1);  /* 364 is the size that all the fileds in the struct need */
	printf("\nnumber of lines= %d, size of entry struct= %dB,total memory needed=%fKB \n",
		       	lines,sizeof(entry), (float)(lines*sizeof(entry)+1)/1024.0);

	unsigned long counter = 0;

	/* Parse the file! */
	while(fgets(buffer, buffer_length, log)!=NULL) {  
    		sscanf(buffer,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", entry_list[counter].uid, 
    			entry_list[counter].file , entry_list[counter].access_type, 
			entry_list[counter].action_denied, entry_list[counter].fingerprint,
		       	entry_list[counter].date_time);
    		counter ++;
	}


	/* Just a test print to check if everything has been loaded */
	/*	
	counter = 0;
	for (counter=0;counter<lines;counter++){
		printf("line:%d,uid:%s,denied:%s,type:%s,fingerprint:%s \n", counter+1,
	       		entry_list[counter].uid, entry_list[counter].action_denied,
			entry_list[counter].access_type,entry_list[counter].fingerprint);
	}
	*/
	
	

	int ch;
	if (argc < 2)
		usage();	

	while ((ch = getopt(argc, argv, "hi:m")) != -1) {
		switch (ch) {		
		case 'i':
			/* Prints table of users that modified 
			 * the file given, and the number of modifications
			 */
			break;
		case 'm':
			/* get the entries which show denial of permissions,
			 * to a new array of structs.
			 */

			break;
		case 'e':
			/*Prints all the files that were encrypted by the ransomware */
			list_encrypted_files(log);
			break;
		case 'v':
			/* Prints total number of files created the last 20 minutes */
			how_many_creations(log);
			break;
		default:
			usage();
		}

	}


	/* add your code here */
	/* ... */
	/* ... */
	/* ... */
	/* ... */


	fclose(log);
	argc -= optind;
	argv += optind;	
	
	return 0;
}
