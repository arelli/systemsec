#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* the path to save the log file to */
static const char LOG_PATH[] = "/tmp/file_access.log";

struct entry {

	char uid[100]; /* user id (positive integer) */
	char access_type[100]; /* access type values [0-2] */
	char action_denied[100]; /* is action denied values [0-1] */

	char  date_time[100]; /* file access date and time */

	char file[100]; /* filename (string) */
	char fingerprint[100]; /* file fingerprint */
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


int 
main(int argc, char *argv[])
{
 	FILE *log;
	log = fopen(LOG_PATH,"r");
	if (log == NULL) {
		printf("Error opening log file at \"%s\"\n", LOG_PATH);
		return 1;
	}

	/* parse the file */
	int lines = 0;
	while(!feof(log))  /* while the end of file is not reached */
	  if(fgetc(log) == '\n')  /* whenever a newline character is found, increment lines */
	    lines++;

	/* return the "cursor" at the beginning of the file */
	fseek(log, 0, SEEK_SET);


	int buffer_length = 255;
	char buffer[buffer_length];
	char dummy_buf[200];
	struct entry * entry_list;
	size_t entry_size = sizeof(entry_list);
	entry_list = (struct entry*)malloc(entry_size*lines+50);
	int counter = 0;
	while(fgets(buffer, buffer_length, log)) {  
    	sscanf(buffer,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", entry_list[counter].uid, 
    		entry_list[counter].file , entry_list[counter].access_type, entry_list[counter].action_denied,
    		entry_list[counter].fingerprint, entry_list[counter].date_time);
    	counter ++;
	}


	/* A test code to see id sscanf with regular expressions works! it is indeed.
	char uid[100], file[100], access_type[100], action_denied[100], fingerprint[100], date_time[100];

	while(fgets(buffer, buffer_length, log)) {  
    	sscanf(buffer,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", uid, file , access_type, action_denied,fingerprint,date_time);
    	printf("Fingerprint of line %d is %s\n", counter+1, fingerprint);
    	counter ++;
	}
	*/



	counter = 0;
	for (counter=0;counter<lines;counter++){
		printf("line:%d,uid:%s,denied:%s,type:%s,fingerprint:%s \n", counter+1, entry_list[counter].uid,
			entry_list[counter].action_denied,entry_list[counter].access_type,entry_list[counter].fingerprint);
	}




	int ch;
	if (argc < 2)
		usage();		

	while ((ch = getopt(argc, argv, "hi:m")) != -1) {
		switch (ch) {		
		case 'i':
			list_file_modifications(log, optarg);
			break;
		case 'm':
			list_unauthorized_accesses(log);
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
