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

/* the element of the dynamic array. It *contains*
 * an array of entry_log structs, and 2 variables 
 * know when to allocate more space. Access is
 * only enabled with insert_array() and init_array(),
 * to keep the memory safely allocated.
 */
typedef struct entry_array{
	entry* array;
	size_t used;
	size_t size;
}e_array;


/* stores a file's modifications based on 
 * its filename.
 */
typedef struct file_mods{
	e_array* modifications;
	char filename[256];
	int mods;
}f_mods;

/* a "dynamic array" of f_mods structs. Each struct contains 
 * info about all modifications of a file(a dynamic array of 
 * f_mods which in turn contain an e_array, a dyamic array 
 * of entries.
 */
typedef struct file_array{
	f_mods* fm;
	size_t used;
	size_t size;
}f_array;


/* pass by reference a "dynamic array" of structs 
 * to be initialised. It doesnt return anything, because
 * the contents of the given struct as argument are
 * themselves updated(outside the function).
 */
void e_init_array(e_array* ar, size_t initial_size){
	ar->array = malloc(initial_size*sizeof(entry));
	ar->used=0;
	ar->size=initial_size;
}
/* Takes as arguments the "dynamic" array and the 
 * entry(struct that keeps one log for one file)
 * that we want to add to it. Doesn't need to return
 * anything, since the structs are passed by reference.
 */
void e_insert_array(e_array* ar, entry new_element){
	/* first, check if we need more space */
	if(ar->used==ar->size){
		ar->size+=100;  /* add space for 100 new entries */
		/* here's where the magic happens:
		 * we allocate enough space, to fit 100 more entries
		 * in our array. Realloc() copies the old data to the
		 * newly allocated space.
		 */
		ar->array = realloc(ar->array,(ar->size*sizeof(entry)) );
	}
	ar->array[ar->used] = new_element; /* actually insert the new element */
	ar->used++;
}
	
/* the same functions, just for f_mods arrays */
void f_init_array(f_array* ar, size_t initial_size){
        ar->fm = malloc(initial_size*sizeof(f_mods));
        ar->used=0;
        ar->size=initial_size;
}

void f_insert_array(f_array* ar, f_mods new_file){
	if(ar->used==ar->size){
                ar->size+=100; 
		ar->fm = realloc(ar->fm,(ar->size*sizeof(f_mods)) );
        }
        ar->fm[ar->used] = new_file;
	ar->used++;
}

/* searches a file by name in a given f_array,
 * a dynamic array of files. We do not use a hash, 
 * because it changes every time the file is moified.
 */
int search_file_name(f_array * ar, char* filename){
	int size = ar->size;  /* get how many files we have */
	for (int i; i < size; i++){
		if(strcmp(ar->fm[i].filename,filename)){
			return i;  /* return the index */
		}
	}
	return -1;  /* not found! */
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

	printf("The lines counted are: %lu\n", lines);
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
	entry e1;
	entry_list = (entry*)malloc(lines*sizeof(entry)+1); 
	printf("\nnumber of lines= %d, size of entry struct= %dB,total memory needed=%fKB \n",
		       	lines,sizeof(entry), (float)(lines*sizeof(entry)+1)/1024.0);

	unsigned long counter = 0;

	f_array files;
	f_init_array(&files,1000); 

	/* Parse the file! */
	while(fgets(buffer, buffer_length, log)!=NULL) {  
    		sscanf(buffer,"%[^,],%[^,],%[^,],%[^,],%[^,],%[^,]", entry_list[counter].uid, 
    			entry_list[counter].file , entry_list[counter].access_type, 
			entry_list[counter].action_denied, entry_list[counter].fingerprint,
		       	entry_list[counter].date_time);

		/* check if the file already is in the files list */
		int index = search_file_name(&files,entry_list[counter].file);
		if(index>=0){  /* the file is already in the array */
			files.fm[index].mods++;  /* note the extra modification in the file */
			e_insert_array( files.fm[index].modifications, entry_list[counter]);
		}
		else{  /* the file is not in the array */
			f_mods* new_file = (f_mods*)malloc(sizeof(f_mods));
			strcpy(new_file->filename, entry_list[counter].file);
			e_array entries;
			e_init_array(&entries,100);
			new_file = 0;
			f_insert_array(&files,*new_file);
		}

    		counter ++;
	}

	for(int i=0; i<counter; i++){
		/* create an array of file structs */
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

	fclose(log);
	argc -= optind;
	argv += optind;	
	
	return 0;
}
