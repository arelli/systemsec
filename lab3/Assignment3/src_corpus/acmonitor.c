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
	if (length_of_array==0)
		return -1;
	for(int i=0;i<length_of_array;i++){
		if(array[i] == number)
			return i;  /* return the position of it */
	}
	return -1; // didnt find anything
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
	/*
	counter = 0;
	for (counter=0;counter<lines;counter++){
		printf("line:%d,uid:%s,denied:%s,type:%s,fingerprint:%s \n", counter+1, entry_list[counter].uid,
			entry_list[counter].action_denied,entry_list[counter].access_type,entry_list[counter].fingerprint);
	}
	*/
	

	int ch;
	if (argc < 2)
		usage();	

	/* declarations for -m mode */
	struct entry * denied;
	denied = (struct entry*)malloc(entry_size*lines+1);
	char * denied_flag = "1";
	int denied_index =0;  
	int *uids_denied = (int*)malloc(sizeof(int)*lines);
	int *no_of_denials = (int*)malloc(sizeof(int)*lines);
	int size_of_denials = 0;
	int index_of_uid;
	int size_of_array = 0;	
	/* the max number of different filenames is lines. This is 
	 * because at the extreme case we only have logged fopen()'s,
	 * each line can hold at most 1 different files.
	 */
	char **filenames = malloc(lines * sizeof(char *));
	for (int i = 0; i < lines; ++i) 
		filenames[i] = (char *)malloc(4096+1);  /* in lunux max filepath size is 4096 */

	int string_found = 0;
	int filenames_length = 0;

	/* end of declarations */

	while ((ch = getopt(argc, argv, "hi:m")) != -1) {
		switch (ch) {		
		case 'i':
			//list_file_modifications(log, optarg);

			break;
		case 'm':
			/* get the entries which show denial of permissions, to a new array of structs.
			 * Also, save the user id's in a seperate array(users_denied)
			 */
			for(int i=0;i<lines;i++) {  
				if (strncmp(entry_list[i].action_denied,denied_flag,1)==0){
					denied[denied_index]= entry_list[i];
					//printf("File %s had unauthorized access by uid %s \n", denied[denied_index].file, denied[denied_index].uid);

					/* check if the uid is already in the list */
					index_of_uid = index_in_array(atoi(entry_list[i].uid),uids_denied,sizeof(int)*size_of_array);

					/* if filename is not previously encountered */
					/* TODO: make it work for sifferent files not acceses!! 
					if ((filenames_length ==0) || (string_found = 0)){
						filenames[filenames_length] = entry_list[i].file;
					}
					*/
				
					if (index_of_uid == -1){  /* -1 means no prior elements have this uid */
						uids_denied[size_of_array] = atoi((const char*)entry_list[i].uid);  /* save the misbehaving user! */
						size_of_array++;
						no_of_denials[index_of_uid] = 1;
					}
					else{
						no_of_denials[index_of_uid] ++;
						size_of_denials++;
					}
				}

				denied_index++;
			}
			printf("Uids that tried to access prohibited files:\n");
			for (int i; i<size_of_array;i++){
				//if (no_of_denials[i]>=7)
				printf("uid %d, %d times\n", uids_denied[i], no_of_denials[i]);
			}
			printf("/*Note: I haven't been able to make it work "
				   " taking into consideration the filenames yet.\n"
				   "This is due to academic load from the school's "
				   "other classes. */\n" );

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
