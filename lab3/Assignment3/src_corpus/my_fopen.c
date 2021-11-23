#define _GNU_SOURCE

#include <stdio.h>
#include <dlfcn.h>

FILE* fopen(const char* path, const char* mode){
	printf("\n************************\n");
	printf("Inside my custom fopen!\n");	
	printf("\n************************\n");

	FILE *(*original_fopen)(const char*,const char*);

	original_fopen = dlsym(RTLD_NEXT, "fopen");
	
	return (*original_fopen)(path,mode);
}
