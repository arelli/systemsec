#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "rsa.h"

/*
 * Sieve of Eratosthenes Algorithm
 * https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
 *
 * arg0: A limit
 * arg1: The size of the generated primes list. Empty argument used as ret val
 *
 * ret:  The prime numbers that are less or equal to the limit
 */
size_t* sieve_of_eratosthenes(int limit,int * primes_len){

	static size_t primes[RSA_SIEVE_LIMIT];  /* dynamic allocation causes problems here! */
	/* disable stdout buffering! dangerous with many printf's! */
	setvbuf(stdout,NULL,_IONBF,0);  /* this is neededwith fish terminal in arch! */
	
	/* used to store all numbers, including primes */
	size_t *all_numbers = (size_t*)malloc(sizeof(size_t)*limit+1);
	//size_t all_numbers[10000];  /* this fixes memory problems!!
	for(int i=0;i<=limit;i++){
		all_numbers[i]=i;
	}
	
	int index = 2;
	int how_many_primes = limit;

	/* the step of the sieve */
	size_t current_step = 2;
	size_t last_step = 2;

	/* used to find the next step */
	int not_found = 1;
	int temp_index = 0;

	while(1){
		/* there are only first numbers left in our array */
		if (current_step*current_step > limit)
			break;
		
		/* start from the beginning, increasing the sieving step */
		if (index>=limit){
			//index = 2;
			temp_index = 0;
			
			/* find the index of the current step in all_numbers[] */
			while(1){
				temp_index++;
				if(temp_index>limit)
					break;  /* this is an error state */
				if (all_numbers[temp_index]==current_step)
					break;  
			}

			/* find the next non-zero number in the sieve
			 * to serve as the next step distance        */
			not_found = 1;
			while(not_found){
				temp_index ++;
				if(temp_index>=limit)
					break;
				if(all_numbers[temp_index]!=0){
					last_step = current_step;
					current_step = all_numbers[temp_index];
					not_found = 0;
				}
			}
			/* each time it must start from the current step index! */
			index = current_step;
		}
		
		/* set this element as non-prime, with the zero notation */
		index += current_step; 
		if (all_numbers[index] != 0){
			all_numbers[index]=0;
			how_many_primes--;
		}
	}

	/* by convention, we remove 1 from the primes */
	all_numbers[1] = 0;
	int prime_index = 0;
	//primes = (size_t*)malloc(sizeof(size_t)*how_many_primes);
	*primes_len = how_many_primes;
	for(int i; i<limit;i++){
		if (all_numbers[i]!=0){	
			//printf("%d, \n", all_numbers[i]);
			primes[prime_index] = all_numbers[i];
			prime_index++;
		}
	}

	return primes;
}


/*
 * Greatest Common Denominator
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: the GCD
 */
int
gcd(int a, int b)
{
	int greatest_number;
	int gcd = 0;
	if(a>b) 
		greatest_number = a;
	else
		greatest_number = b;
	for (int i=1;i<greatest_number; i++){
		if(a % i == 0  && b % i == 0){
			gcd = i;
		}
	}
	//printf("The gcd of %d and %d is %d\n", a,b,gcd);
	return gcd;

}


/*
 * Calculates the modular inverse, which is the number m for whom
 * it is true that:
 *		a*m=1 mod b, where mE{0,1,...,b-2,b-1}
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: modular inverse
 */
size_t
mod_inverse(size_t a, size_t b)  /* in our appliaction, a=e and b=fi(n)) */
{
	for (int x = 1; x < b; x++)
        if (((a%b) * (x%b)) % b == 1)
            return x;
}


/*
 * Generates an RSA key pair and saves
 * each key in a different file
 */
void
rsa_keygen(void)
{	int how_many_primes;
	size_t* primes = sieve_of_eratosthenes(RSA_SIEVE_LIMIT, &how_many_primes);
	/* select 2 random prime numbers from the list returned by the sieve .
	 * they must have a product of at least 127, because the end result will
	 * be modded by n (which is p*q).
	 */
	size_t p = primes[rand_int(how_many_primes)];
	size_t q = 0;
	while(p*q<=127){
		q = primes[rand_int(how_many_primes)];
	}
	size_t n = p * q;
	size_t fi_n = (p-1)*(q-1); //phi(n);
	size_t e = primes[rand_int(how_many_primes)];
	int found = 0;
	/* select a valid e completely randomly */
	while(1){
		if((e%fi_n!=0) && (gcd(e,fi_n)==1))
			break;
		else{
			e = primes[rand_int(how_many_primes)];  /* e must be random! */
		}
	}  // this loop is infinite if e can't be found!! TODO: check only neccessary primes

	size_t d = mod_inverse(e,(p-1)*(q-1));

}


/*
 * Encrypts an input file and dumps the ciphertext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void
rsa_encrypt(char *input_file, char *output_file, char *key_file)
{

	/* TODO */

}


/*
 * Decrypts an input file and dumps the plaintext into an output file
 *
 * arg0: path to input file
 * arg1: path to output file
 * arg2: path to key file
 */
void
rsa_decrypt(char *input_file, char *output_file, char *key_file)
{

	/* TODO */

}


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

/* wrapper for the random_string function, to generate 
 * crypto-safe random selection of prime numbers
 */
int rand_int(int limit){
	return abs((int)(*random_string(1)))%limit;
}

/* Eulers Totient Function 
 * counts the positive integers up to n that are relatively prime to n */
int phi(int n){
	int counter = 0;
	for(int i=0; i<n; i++){
		if (gcd(i,n)==1)
			counter++;
	}
	return counter;
}

void main(){
	int primes_length = 10;
	size_t* prime_numbers = sieve_of_eratosthenes(RSA_SIEVE_LIMIT,&primes_length);
	for(int i = 0; i<primes_length-1; i++){
		printf("%d, ", prime_numbers[i]);
	}
	printf("\n");
	
	int random = rand_int(primes_length);
	printf("a random number: %d ,", random);
	printf("and its respective prime: %d\n", prime_numbers[random]);
	printf("And phi(5013) = %d(should be 3336)\n", phi(5013));
	printf("and another random: %d \n", rand_int(primes_length));
	printf("The modular inverse of 3 and 11 is %d(should be 4) \n", mod_inverse(3,11));
	printf("The modular inverse of 7 and 26 is %d(should be 15)\n", mod_inverse(7,26));
}
