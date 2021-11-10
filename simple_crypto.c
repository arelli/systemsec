
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

	return random_string;
}


char* random_string_letters(char lowest, char highest,int length){
	int letter_counter = 0;
	char* temp_ptr;
	char temp;
	char* random_str = (char*)malloc(sizeof(char)*length);

	while(letter_counter<length){
		temp_ptr = random_string(1);  // get random ASCII character
		temp = *temp_ptr;
		if (temp>=lowest && temp<=highest){
			random_str[letter_counter] = temp;
			letter_counter++;
		}
	}
	return random_str;
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
	int length = strlen(pad);   // TODO: dangeroud! may result to dumped core. Maybe need to use strlen(text)
	int counter;
	char temp;  // temporary to hold value of input text character during xor
	char* response = (char*)malloc(length);

	for (counter=0;counter<length;counter++){
		//if ((text[counter]==pad[counter])){  // avoid generating NULL's, and skip special characters, while lowering security(just a... bit!)
		//	response[counter] = text[counter];		
		//}
		//else
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
// Works also for single characters, and is used in vigenere() later(only the core).
char* caesar_core(char* text, int shift, char* alphabet){
	int length = strlen(text);
	int i;
	char* ciphertext = (char*)malloc(length*sizeof(char));
	char temp;
	char ch;
	int position;

	//char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	int alphabet_length = strlen(alphabet);

	// define the limits of the alphabet
	shift = shift%alphabet_length;

	for(i=0;i<length;i++){
		ch = text[i];
		position = find(ch,alphabet);

		if ((position+shift)>=alphabet_length)  // z has index 61(62 characters)
			temp = alphabet[position + shift - alphabet_length];
		else if ((position+shift)<0)
			temp = alphabet[position + shift + alphabet_length];
		else
			temp = alphabet[position+shift];

		ciphertext[i]=temp;
	}
	return ciphertext;
}

// wrapper function to provide this exact alphabet
char* caesar(char* text, int shift){
	char alphabet[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	return caesar_core(text,shift,alphabet);
}




// Vigenere cipher implementation usin caesar_core
char* vigenere(char* text, char* key){
	char alphabet[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	char* temp_alphabet= (char*)malloc(27);  // will be changed right away 
	char* ciphertext = (char*)malloc(strlen(text)*sizeof(char));
	int i;
	int length = strlen(text);
	char temp;
	char* temp_result;
	int row;
	int column;

    /*
	TODO: make uppercase the key and the text! 
	*/
	for(i=0;i<length;i++){
		if(key[i]>='a' && key[i]<='z')
	        {
	            key[i] = key[i] - 32;
	        }
	    if(text[i]>='a' && text[i]<='z')
	        {
	            text[i] = text[i] - 32;
	        }
	    }


	for(i=0;i<strlen(text);i++){
		temp = text[i];
		row = find(temp,alphabet);  // "row number". the temp_alphabet is shifted that amount.
		temp_alphabet = caesar_core(alphabet,row,alphabet);  // shift the alphabet left by "row" characters

		temp = key[i];
		column = find(temp, temp_alphabet);  // the index of encrypted character is the column of shifted alphabet...

		ciphertext[i] = alphabet[column];  //... and corresponds to the non-shifted alphabet.
	}

	return ciphertext;
}
