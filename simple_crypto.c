
// print not printable characters
void print_npc(char* npc_string){
	int i;
	for(i=0;i<strlen(npc_string);i++){
		if(isprint(npc_string[i]))
			printf(" %c ", npc_string[i]);
		else
			printf(" %x ", npc_string[i]);
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

int is_letter(char input){
	if(((input>='A')&&(input<='Z')) || ((input>='a')&&(input<='z')) || ((input>='0')&&(input<='9')))
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

// Implementation of the caesars shift encryption algorithm
// arguments: the string to encrypt/decrypt and the size of
// offset to apply.(either to encrypt or decrypt)
char* caesar(char* text, int shift){
	int length = strlen(text);
	int i;
	char* ciphertext = (char*)malloc(length*sizeof(char));
	char temp;
	char ch;

	// define the limits of the alphabet
	if (shift>0)
		shift = shift%26;
	else if (shift<0)
		shift = (-1)*(((-1)*shift)%26);

	for(i=0;i<length;i++){
		ch = text[i];
		if ((text[i] >= 'A') && (ch<='Z')){  // if uppercase..
			if ((ch+shift)> 'Z')
				temp= ch +(shift-26);  // the english alphabet has 26 letters
			else if ((ch+shift)<'A')
				temp = text[i]+(shift+26);
			else
				temp = ch + shift;
		}
		else if ((ch >= 'a') && (ch<='z')){  // if lowercase..
			if ((ch+shift)> 'z')
				temp= ch+(shift-26);
			else if ((ch+shift)<'a')
				temp = ch+(shift+26);
			else
				temp = ch + shift;
		}
		ciphertext[i]=temp;
	}
	return ciphertext;
}
