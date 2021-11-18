#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <openssl/evp.h>
#include <openssl/err.h>
#include <openssl/conf.h>
#include <openssl/cmac.h>
// compile with: gcc assign_2.c -lssl -lcrypto 

#define BLOCK_SIZE 16  


/* function prototypes */
void print_hex(unsigned char *, size_t);
void print_string(unsigned char *, size_t); 
void usage(void);
void check_args(char *, char *, unsigned char *, int, int);
void keygen(unsigned char *, unsigned char *, unsigned char *, int);
int encrypt(unsigned char *, int, unsigned char *, unsigned char *, 
    unsigned char *, int );
int decrypt(unsigned char *, int, unsigned char *, unsigned char *, 
    unsigned char *, int);
void gen_cmac(unsigned char *, size_t, unsigned char *, unsigned char *, int);
int verify_cmac(unsigned char *, unsigned char *);
int write_to_file( char *,  char*, int length);
char* read_from_file(char*, int* );


/* TODO Declare your function prototypes here... */


/*
 * Prints the hex value of the input
 * 16 values per line
 */
void
print_hex(unsigned char *data, size_t len)
{
	size_t i;

	if (!data)
		printf("NULL data\n");
	else {
		for (i = 0; i < len; i++) {
			if (!(i % 16) && (i != 0))
				printf("\n");
			printf("%02X ", data[i]);
		}
		printf("\n");
	}
}


/*
 * Prints the input as string
 */
void
print_string(unsigned char *data, size_t len)
{
	size_t i;

	if (!data)
		printf("NULL data\n");
	else {
		for (i = 0; i < len; i++)
			printf("%c", data[i]);
		printf("\n");
	}
}


/*
 * Prints the usage message
 * Describe the usage of the new arguments you introduce
 */
void
usage(void)
{
	printf(
	    "\n"
	    "Usage:\n"
	    "    assign_1 -i in_file -o out_file -p passwd -b bits" 
	        " [-d | -e | -s | -v]\n"
	    "    assign_1 -h\n"
	);

	printf(
	    "\n"
	    "Options:\n"
	    " -i    path    Path to input file\n"
	    " -o    path    Path to output file\n"
	    " -p    psswd   Password for key generation\n"
	    " -b    bits    Bit mode (128 or 256 only)\n"
	    " -d            Decrypt input and store results to output\n"
	    " -e            Encrypt input and store results to output\n"
	    " -s            Encrypt+sign input and store results to output\n"
	    " -v            Decrypt+verify input and store results to output\n"
	    " -h            This help message\n"
	);
	exit(EXIT_FAILURE);  // completely exit the program and return to parent process EXIT_FAILURE
}


/*
 * Checks the validity of the arguments
 * Check the new arguments you introduce
 */
void
check_args(char *input_file, char *output_file, unsigned char *password, 
    int bit_mode, int op_mode)
{
	if (!input_file) {
		printf("Error: No input file!\n");
		usage();
	}

	if (!output_file) {
		printf("Error: No output file!\n");
		usage();
	}

	if (!password) {
		printf("Error: No user key!\n");
		usage();
	}

	if ((bit_mode != 128) && (bit_mode != 256)) {
		printf("Error: Bit Mode <%d> is invalid!\n", bit_mode);
		usage();
	}

	if (op_mode == -1) {
		printf("Error: No mode\n");
		usage();
	}
}


/*
 * Generates a key using a password
 */
void
keygen(unsigned char *password, unsigned char *key, unsigned char *iv,
    int bit_mode)
{	// set the cipher type variable
    const EVP_CIPHER *cipher_alg;

    // set the cipher algorithm used
    if (bit_mode==128)
    	cipher_alg = EVP_aes_128_ecb(); 
	else if (bit_mode==256)
		cipher_alg = EVP_aes_256_ecb(); 
	else{
		printf("Invalid bit_mode..Exiting..");
		return;
	}
    // set the hashing algorithm
    const EVP_MD * hashing_algorithm =  EVP_get_digestbyname("sha1");
    // the salt is null and the iteration count is 1. here the Hashing is being done
    // warning: strlen takes a const char* as argument, so we use a cast.
    EVP_BytesToKey(cipher_alg, hashing_algorithm, NULL, password, strlen((const char*)password), 1 , key, iv);

    //printf("\n The password entered is: %s\n", password);
    //printf("\nThe hashed output from the password: %s\n", key);

}


/*
 * Encrypts the data
 */
int encrypt(unsigned char *plaintext, int plaintext_len, unsigned char *key,
    unsigned char *iv, unsigned char *ciphertext, int bit_mode)
{
	EVP_CIPHER_CTX* context;

	int length, padding_len;

	/*initialise the context*/
	context = EVP_CIPHER_CTX_new();

	if (bit_mode==128)
      EVP_EncryptInit_ex(context, EVP_aes_128_ecb(), NULL, key, iv);  
    else if (bit_mode==256)
      EVP_EncryptInit_ex(context, EVP_aes_256_ecb(), NULL, key, iv); 
    else{
      printf("Wrong bitmode (%d) provided.. Exiting..", bit_mode);
      return -1;
    }

    /* Set the content to be encrypted and get the output */
    /* some arguments are passed by reference so that the function 
    can change their values */
    EVP_EncryptUpdate(context, ciphertext, &length, plaintext, plaintext_len);



    /* Finalise the encryption */
    EVP_EncryptFinal_ex(context, ciphertext + length, &padding_len);
    plaintext_len += padding_len;  /* IMPORTANT!!: this adds the padding length to the overall, returned length!*/

    EVP_CIPHER_CTX_free(context);
   	// the ciphertext is returned by reference
   	return plaintext_len;

}


/*
 * Decrypts the data and returns the plaintext size
 */
int
decrypt(unsigned char *ciphertext, int ciphertext_len, unsigned char *key,
    unsigned char *iv, unsigned char *plaintext, int bit_mode)
{
	int plaintext_len;

	plaintext_len = 0;

	/*TODO Task C */
	EVP_CIPHER_CTX *context;
	int length;

	context = EVP_CIPHER_CTX_new();

	if (bit_mode==128)
      EVP_DecryptInit_ex(context, EVP_aes_128_ecb(), NULL, key, iv);
    else if (bit_mode==256)
      EVP_DecryptInit_ex(context, EVP_aes_256_ecb(), NULL, key, iv); 
    else{
      printf("Wrong bitmode (%d) provided.. Exiting..", bit_mode);
      return -1;
    }

    EVP_DecryptUpdate(context,plaintext,&length,ciphertext,ciphertext_len);
    plaintext_len = length;

    EVP_DecryptFinal_ex(context, plaintext+length, &length);
    plaintext_len = length;

    EVP_CIPHER_CTX_free(context);


	return plaintext_len;
}


/*
 * Generates a CMAC
 */
void
gen_cmac(unsigned char *data, size_t data_len, unsigned char *key,  unsigned char *cmac, int bit_mode)
{

	/* TODO Task D */
	CMAC_CTX *context ; 
	context = CMAC_CTX_new();
	size_t extrabytes;  // how many new bytes will be added by the cmac

	const EVP_CIPHER *cipher;

	if (bit_mode == 128)
		cipher = EVP_aes_128_ecb();
	else 
		cipher = EVP_aes_256_ecb();

	CMAC_Init(context,key,bit_mode/8,cipher, NULL);

	CMAC_Update(context, data, data_len);

	CMAC_Final(context, cmac, &extrabytes);

	*(cmac+BLOCK_SIZE) = '\0';  // terminate the created string

	CMAC_CTX_free(context);
}


/*
 * Verifies a CMAC
 */
int
verify_cmac(unsigned char *cmac1, unsigned char *cmac2)
{
	/* TODO Task E */
	return strcmp((const char*)cmac1,(const char*)cmac2);
	 // if they match, return 1
}



/* TODO Develop your functions here... */

int write_to_file(char * filename, char* data, int length){
	FILE * file_ptr = NULL;
	file_ptr = fopen(filename,"wb");
	if (file_ptr==NULL) return -1;
	//fputs(data, file_ptr);
	fwrite(data, sizeof(char), length, file_ptr);
	fclose(file_ptr);
	return 0;
}


int append_to_file(char * filename, char* data, int length){
	FILE * file_ptr = NULL;
	file_ptr = fopen(filename,"ab");
	if (file_ptr==NULL) return -1;
	//fputs(data, file_ptr);
	fwrite(data, sizeof(char), length, file_ptr);
	fclose(file_ptr);
	return 0;
}


char* read_from_file(char* filename, int* returned_length){
	FILE * file_ptr = NULL;
	int file_length = 0;
	int counter;
	file_ptr = fopen(filename,"rb");
	if (file_ptr==NULL) exit(EXIT_FAILURE);

	fseek(file_ptr,0,SEEK_END);
	file_length = ftell(file_ptr);  // return the position of the cursor
	*returned_length = file_length;
	rewind(file_ptr);  // return the pointer at the beginning of the strem(file)

	char * buffer = (char*)malloc(sizeof(char)*file_length+1);  

	for(counter = 0; counter < file_length; counter++) {
       if(fread(buffer+counter, 1, 1, file_ptr)!=1){
       	printf("ERROR while reading the file...Aborting..");
       	exit(EXIT_FAILURE);
       }
    }
	fclose(file_ptr);
	return buffer;
}



/*
 * Encrypts the input file and stores the ciphertext to the output file
 *
 * Decrypts the input file and stores the plaintext to the output file
 *
 * Encrypts and signs the input file and stores the ciphertext concatenated with 
 * the CMAC to the output file
 *
 * Decrypts and verifies the input file and stores the plaintext to the output
 * file
 */
int
main(int argc, char **argv)
{
	int opt;			/* used for command line arguments */
	int bit_mode;			/* defines the key-size 128 or 256 */
	int op_mode;			/* operation mode */
	char *input_file;		/* path to the input file */
	char *output_file;		/* path to the output file */
	unsigned char *password;	/* the user defined password */

	/* Init arguments */
	input_file = NULL;
	output_file = NULL;
	password = NULL;
	bit_mode = -1;
	op_mode = -1;

	/* test code to check the I/O functions:*/
	/*
	write_to_file("randomfilename","this is data!",14);

	const char* buffer =(char*)malloc(1024) ;
	buffer = read_from_file("randomfilename");

	printf("This is th read string: %s\n", buffer);
	*/

	

	/*
	 * Get arguments
	 */
	while ((opt = getopt(argc, argv, "b:i:m:o:p:desvh:")) != -1) {
		switch (opt) {
		case 'b':
			bit_mode = atoi(optarg);
			break;
		case 'i':
			input_file = strdup(optarg);
			break;
		case 'o':
			output_file = strdup(optarg);
			break;
		case 'p':
			password = (unsigned char *)strdup(optarg);
			break;
		case 'd':
			/* if op_mode == 1 the tool decrypts */
			op_mode = 1;
			break;
		case 'e':
			/* if op_mode == 1 the tool encrypts */
			op_mode = 0;
			break;
		case 's':
			/* if op_mode == 1 the tool signs */
			op_mode = 2;
			break;
		case 'v':
			/* if op_mode == 1 the tool verifies */
			op_mode = 3;
			break;
		case 'h':
		default:
			usage();
		}
	}


	/* check arguments */
	check_args(input_file, output_file, password, bit_mode, op_mode);

	/* TODO Develop the logic of your tool here... */

	/* Initialize the library */

	/* Keygen from password */
	unsigned char* key = (unsigned char*)malloc(16*sizeof(char));  // cmac key is always 16 bytes

	keygen(password, key,NULL,bit_mode);

	char * temp = (char*)malloc(4096*sizeof(char));
	char * output = (char*)malloc(1024*sizeof(char));  // TODO: do (input_length%block_size)+input_length(and + blocksize, if cmac)


	int file_length;  // the file length is gonna be stored here by reference
	int ciphertext_length;
	/*
	temp = read_from_file(input_file, &file_length);
	char * data =(char*)malloc(sizeof(char)*file_length);
	memcpy(data,temp,file_length+1);
	*/
	char* data = (char*)malloc(4096);
	data = read_from_file(input_file, &file_length);


// for the verification and signing
	unsigned char *cmac_output;


	if (bit_mode==128){
		printf("[KEY]hex: \n");
		print_hex((unsigned char *)key,128/8);
	}
	else{
		printf("[KEY]hex: \n");
		print_hex((unsigned char *)key, 256/8);
	}
	

	/* Operate on the data according to the mode */
	/* encrypt */
	if (op_mode == 0){
	 	ciphertext_length = encrypt((unsigned char*)data, file_length, key, NULL ,(unsigned char*)output,bit_mode);

	 	
	 	file_length = ciphertext_length;

		printf("[DATA]encrypted-hex:(length = %d) \n", file_length);
		print_hex((unsigned char *)output,file_length);

		write_to_file(output_file,output,file_length);
	 }




	/* decrypt */
 	else if (op_mode == 1){
		decrypt((unsigned char*)data, file_length, key, NULL,(unsigned char*)output,bit_mode);

		printf("[DATA]decrypted-ascii:\n%s\n", output);

		write_to_file(output_file,output,file_length);
	 }



	/* sign - no else if here!!!*/
	if (op_mode == 2){
		// calculate length of output after the CMAC addition
		//cmaced_output = BLOCK_SIZE -(file_length%BLOCK_SIZE);
		// allocate space for the cmac output
		cmac_output = (unsigned char*)malloc(BLOCK_SIZE*sizeof(char));

		//unsigned char *total_output= (unsigned char*)malloc(cmaced_output*sizeof(char));

		gen_cmac((unsigned char*)data,BLOCK_SIZE+file_length, key, cmac_output, bit_mode);
		printf("[CMAC]cmac in ASCII is %s HEX is: \n",cmac_output);
		print_hex(cmac_output, BLOCK_SIZE);


		ciphertext_length = encrypt((unsigned char*)data, file_length, key, NULL ,(unsigned char*)output,bit_mode);

		
		char * out_buf = (char*)malloc(sizeof(char)*4096) ;
		int counter = 0;
		// copy the ciphertext in the buffer
		for(counter=0;counter<ciphertext_length;counter++)
			out_buf[counter] = output[counter];
		// copy the CMAC at the end of the buffer
		for (counter = 0; counter < BLOCK_SIZE; counter++)
			out_buf[ciphertext_length + counter] = cmac_output[counter];

		write_to_file(output_file,out_buf,ciphertext_length + BLOCK_SIZE);

	 }




	 else if (op_mode == 3){
	 	decrypt((unsigned char*)data, file_length, key, NULL ,(unsigned char*)output,bit_mode);
	 	cmac_output = (unsigned char*)malloc(BLOCK_SIZE*sizeof(char));
	 	gen_cmac((unsigned char*)data,BLOCK_SIZE+file_length, key, cmac_output, bit_mode);
	 	char* cmac = (char *)malloc(sizeof(char)*BLOCK_SIZE);
	 	int counter = 0;
	 	for(counter=0;counter<BLOCK_SIZE;counter++){
	 		*(cmac+counter) = *(output+file_length-counter);
	 	}
	 	print_hex((unsigned char*)cmac, BLOCK_SIZE);
	 }


	/* Clean up */
	free(input_file);
	free(output_file);
	free(password);


	/* END */
	return 0;
}
