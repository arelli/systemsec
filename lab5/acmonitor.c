#define _GNU_SOURCE
#define __USE_XOPEN

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* the path to save the log file to */
static const char LOG_PATH[] = "file_logging.log"; 

/* a struct to keep all the info from one entry in out log */
typedef struct entry_log {
	char uid[8]; /* user id, max is 65536 in Linux */
	char access_type[2]; /* access type values [0-2] */
	char action_denied[2]; /* is action denied values [0-1] */
	char  date_time[32]; /* file access date and time, max length 29 in current format*/
	char file[256]; /* 256 to save space. Should be 4096 bytes long. This could create sigsegvs!*/
	char fingerprint[64]; /* file md5 digest, 32 bytes long */
} entry;

/* from SO */ 
char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}


void
usage(void)
{
	printf("\nusage:\n\t./monitor \nOptions:\n-m, Prints malicious users\n"
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

	return;

}


void
list_file_modifications(FILE *log, char *file_to_scan)
{
	/* add your code here */

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
 	FILE *log;
	log = fopen(LOG_PATH,"r");
	if (log == NULL) {
		printf("Error opening log file at \"%s\"\n", LOG_PATH);
		return 1;
	}

	/* parse the file */
	unsigned long lines = 0;
	while(!feof(log))  /* while the end of file is not reached */
	  if(fgetc(log) == '\n') 
	    lines++;

	/* return the "cursor" at the beginning of the file */
	fseek(log, 0, SEEK_SET);

	/* Load data from the log file in a array of structs */
	int buffer_length = 1024;
	char buffer[buffer_length];
	char dummy_buf[200];

	/* here I allocate space for all the entries in the log file.
	 * This means I allocate enough space for the neccessary struct
	 * instances to be in, but I am not declairng each instance 
	 * explicitly! This may cause errors.
	 */
	entry * entry_list;
	entry_list = (entry*)malloc(lines*sizeof(entry)+1); 

	unsigned long counter = 0;
	/* Parse the file! */
	while(fgets(buffer, buffer_length, log)!=NULL) {  
		entry new_entry;
    		sscanf(buffer,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", new_entry.uid, 
    			new_entry.file , new_entry.access_type, 
			new_entry.action_denied, new_entry.fingerprint,
		       	new_entry.date_time);
		memcpy(&entry_list[counter], &new_entry,sizeof(entry));
    		counter ++;
	}


	/* Handle Aguments */
	int ch;
	if (argc < 2)
		usage();	

	while ((ch = getopt(argc, argv, "i:v:hem")) != -1) {
		switch (ch) {		
		case 'i':
			/* Prints table of users that modified 
			 * the file given, and the number of modifications
			 */
			break;
		case 'm':
			break;

		case 'e':
			/*Prints all the files that were encrypted by the ransomware */
			counter = 0;
			int enc_files_counter = 0;
			char** encrypted_filenames;
			encrypted_filenames = (char**)malloc(sizeof(char*)*lines+1);
			char enc_suffix[]=".encrypt";
			char* filename;
			int found = 0;
			for (counter=0;counter<lines;counter++){
				filename = (char*)calloc(strlen(entry_list[counter].file)+1,sizeof(char));
				strcpy(filename,entry_list[counter].file);
				strrev(filename);  /* reverse the string */
				strrev(enc_suffix);
				if(strncmp(filename,enc_suffix,sizeof(enc_suffix)-1)==0){  /* if the file ending is ".encrypted" */
					/* check if the filename is already stored */		
					for(int i = 0; i<enc_files_counter; i++){
						if(strncmp(encrypted_filenames[i],entry_list[counter].file,strlen(entry_list[counter].file))==0)
								found=1;  // do not store the filename again 
					}
					
					if(found==0){
						encrypted_filenames[enc_files_counter] = entry_list[counter].file;
						enc_files_counter++;
					}
					found=0;
				}
			}
		
			for (int i = 0; i<enc_files_counter; i++){
				printf("%s\n",encrypted_filenames[i]);
			}
			break;

		case 'v':
			/* Show how many files were created the last 20 miniutes,
			 * above a threshold X which is given in argv[2]
			 */
			counter = 0;
			int files_counter = 0;
			char** filenames;
			filenames = (char**)malloc(sizeof(char*)*lines+1);

			char* date;
			for (counter=0;counter<lines;counter++){
				date = (char*)calloc(strlen(entry_list[counter].date_time)+1,sizeof(char));
				strcpy(date,entry_list[counter].date_time);
				printf("%s\n",date);
				struct tm tm;
				strptime(date, "%a %b %Od %H:%M:%S %Y", &tm);
				time_t t = mktime(&tm);

			}
		

			break;
		default:
			usage();
		}
	}


	/* add your code here */
	/* ... */

	fclose(log);
	argc -= optind;
	argv += optind;	
	
	return 0;
}
