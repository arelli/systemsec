
// print not printable characters
// takes as arguments the string, and its supposed length
// to avoid stoping at NULL's that may occur during
// OTP encryption
void print_npc(char* npc_string, int length){
	int i;
	for(i=0;i<length;i++){
		if(isprint(npc_string[i]))
			printf(" %c", npc_string[i]);  
		else
			printf(" 0x%X", npc_string[i] & 0xff);  // &0xff to mask only the 8 lower bits!
	}
	printf("\n");
}

// get random string of given length from urandom!
char* random_string(int length){
	char* random_string = (char*)malloc(length*sizeof(char));
	int i;
	if (random_string==NULL){
		printf("Error when allocating memory for random string...Exiting..\n");
		return NULL;
	}
	FILE *fpointer;
	fpointer = fopen("/dev/urandom", "r");  // read file in binary mode
	fread(random_string,sizeof(char),length,fpointer);  // read somr bytes from fpointer position to random_string
	
	//printf("\nThe random string generated is: ");
	//print_npc(random_string);

	return random_string;
}

// check if a character is a printable english language letter
int is_letter(char input){
	if(((input>='A')&&(input<='Z')) || ((input>='a')&&(input<='z')) || ((input>='0')&&(input<='9')) || (input==' '))
		return 1;
	else
		return 0;
}

// strip special characters from strings
char* strip_special(char* input){
	int i; int j=0;
	char* output = (char*)malloc(sizeof(char)*strlen(input));
	for(i=0;input[i]!='\0';i++){
		if(is_letter(input[i])){
			output[j] = input[i];
			j++;
		}
	}
	if(strlen(output)<strlen(input))
		output[j+1] = '\0';
	return output;
}


// implementation of the One Time Pad encryption Algorithm
// accepts two arguments, one being the text-to-be-encrypted(or decrypted)
// and the other thepad(key) for encryption/decryption.
char* one_time_pad(char* text, char* pad){
	int length = strlen(text);  // it will count the \0 character at the end!
	int counter;
	char temp;  // temporary to hold value of input text character during xor
	char* response = (char*)malloc(length);

	for (counter=0;counter<length;counter++){
		if ((text[counter]==pad[counter])){  // avoid generating NULL's, and skip special characters, while lowering security(just a... bit!)
			response[counter] = text[counter];		
		}
		else
			response[counter] = text[counter]^pad[counter];
		
	}
	response[counter] = '\0';  // add the end string character
	return response;
}

// search in string for term, and return the index if found
int find(char term,char* string){
	int i;
	for (i=0;string[i]!='\0';i++){
		if(string[i]==term)
			return i;
	}
	return 0;  // haven't found anything
}

// Implementation of the caesars shift encryption algorithm
// arguments: the string to encrypt/decrypt and the size of
// offset to apply.(either to encrypt or decrypt)
char* caesar(char* text, int shift){
	int length = strlen(text);
	int i;
	char* ciphertext = (char*)malloc(length*sizeof(char));
	char temp;
	char ch;
	int position;


	char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int alphabet_length = strlen(alphabet);

	// define the limits of the alphabet
	shift = shift%alphabet_length;

	for(i=0;i<length;i++){
		ch = text[i];
		position = find(ch,alphabet);

		if ((position+shift)>alphabet_length)  // z has index 61(62 characters)
			temp = alphabet[position + shift - alphabet_length];
		else if ((position+shift)<0)
			temp = alphabet[position + shift + alphabet_length];
		else
			temp = alphabet[position+shift];

		ciphertext[i]=temp;
	}
	return ciphertext;
}
