#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* the path to save the log file to */
static const char LOG_PATH[] = "/tmp/file_access.log";

struct entry {

	char uid[32]; /* user id, max is 65536 in Linux */
	char access_type[16]; /* access type values [0-2] */
	char action_denied[16]; /* is action denied values [0-1] */

	char  date_time[32]; /* file access date and time, max length 29 in current format*/

	char file[256]; /* filename/path. Can be a bit long, giving it 150 bytes of space */
	char fingerprint[64]; /* file fingerprint-32 bytes long */
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
/* Takes an integer array and checks if a number is inside it */
int index_in_array(int number, int * array, size_t length_of_array){
	for(int i=0;i<length_of_array;i++){
		if(array[i] == number)
			return i;  /* return the position of it */
	}
	return -1; // didnt found anything
}

/*
int index_in_str_array(char* string, int * array, size_t length_of_array){
	for(int i=0;i<length_of_array;i++){
		if(array[i] == number)
			return i;  
	}
	return -1; // didnt found anything
}

*/


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

	/* Load data from the log file in a array of structs */
	int buffer_length = 255;
	char buffer[buffer_length];
	char dummy_buf[200];

	struct entry * entry_list;
	size_t entry_size = sizeof(entry_list);
	entry_list = (struct entry*)malloc(entry_size*lines+1);

	int counter = 0;


	/* Parse the file! */
	while(fgets(buffer, buffer_length, log)) {  
    	sscanf(buffer,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", entry_list[counter].uid, 
    		entry_list[counter].file , entry_list[counter].access_type, entry_list[counter].action_denied,
    		entry_list[counter].fingerprint, entry_list[counter].date_time);
    	counter ++;
	}


	/* Just a test print to check if everything has been loaded */
	
	counter = 0;
	for (counter=0;counter<lines;counter++){
		printf("line:%d,uid:%s,denied:%s,type:%s,fingerprint:%s \n", counter+1, entry_list[counter].uid,
			entry_list[counter].action_denied,entry_list[counter].access_type,entry_list[counter].fingerprint);
	}
	


	struct entry * denied;
	denied = (struct entry*)malloc(entry_size*lines+1);
	char * denied_flag = "1";

	for(int i=0;i<lines;i++) {  
		if (strncmp(entry_list[i].action_denied,denied_flag,1)==0)
			printf("File %s had unauthorized access \n", entry_list[i].file);
	}







	int ch;
	if (argc < 2)
		usage();		

	while ((ch = getopt(argc, argv, "hi:m")) != -1) {
		switch (ch) {		
		case 'i':
			//list_file_modifications(log, optarg);




			break;
		case 'm':
			//list_unauthorized_accesses(log);
			counter = 0;
			int *uids_in_log = (int*)malloc(sizeof(int)*lines);
			int *uid_no_of_denials = (int*)malloc(sizeof(int)*lines);  /* hold how many times each user has been denied access */
			int size_of_array = 0;  /* refers to uids_in_log array */
			for (counter=0;counter<lines;counter++){
				if(entry_list[counter].action_denied=="1"){  /* if action was indeed denied */
					int index_of_uid = index_in_array(atoi(entry_list[counter].uid),uids_in_log,sizeof(int)*size_of_array);

					if (index_of_uid == -1){  /* -1 means no prior elements have this uid */
						uids_in_log[size_of_array] = atoi(entry_list[counter].uid);  /* save the misbehaving user! */
						uid_no_of_denials[size_of_array] = 1;
						size_of_array++;
					}
					else{
						uid_no_of_denials[index_of_uid]++;
					}
				}
			}

			for(int i = 0; i< size_of_array; i++){
				if(uid_no_of_denials[i]>=7)
					printf("The user with uid %d has accessed %d times prohibited files.\n",uids_in_log[i], uid_no_of_denials[i]);
			}
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
