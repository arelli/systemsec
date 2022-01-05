#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <sys/stat.h>  // to modify permissions of files

int main(int argc, char **argv) 
{
	int i;
	FILE *file;
	
	/* takes as first argument the filename and as second the file contents */
	file = fopen(argv[1], "w");
	fwrite(argv[2], strlen(argv[2]), 1, file);
	fclose(file);
	//for (i=0; i<argc; i++)
	//  printf("agv[%d]=%s\n",i, argv[i]);  
}


