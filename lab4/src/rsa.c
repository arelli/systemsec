#include <stdio.h>
#include <stdlib.h>
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

	static size_t primes[RSA_SIEVE_LIMIT];
	/* disable stdout buffering! dangerous with many printf's! */
	setvbuf(stdout,NULL,_IONBF,0);  /* this is neededwith fish terminal in arch! */
	
	/* used to store all numbers, including primes */
	size_t *all_numbers = (size_t*)malloc(sizeof(size_t)*limit);
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

	
	printf("There are in total %d prime numbers from 1 to %d .\n", how_many_primes,limit);
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
	for (int i=2;i<greatest_number; i++){
		if(a % i == 0  && b % i == 0){
			gcd = i;
		}
	}
	printf("The gcd of %d and %d is %d\n", a,b,gcd);
	return gcd;

}


/*
 * Chooses 'e' where 
 *     1 < e < fi(n) AND gcd(e, fi(n)) == 1
 *
 * arg0: fi(n)
 *
 * ret: 'e'
 */
size_t
choose_e(size_t fi_n)
{
	size_t e;

	/* TODO */

	return e;
}


/*
 * Calculates the modular inverse
 *
 * arg0: first number
 * arg1: second number
 *
 * ret: modular inverse
 */
size_t
mod_inverse(size_t a, size_t b)
{

	/* TODO */

}


/*
 * Generates an RSA key pair and saves
 * each key in a different file
 */
void
rsa_keygen(void)
{
	size_t p;
	size_t q;
	size_t n;
	size_t fi_n;
	size_t e;
	size_t d;

	/* TODO */

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


void main(){
	int primes_length;
	size_t* prime_numbers = sieve_of_eratosthenes(RSA_SIEVE_LIMIT,&primes_length);
	for(int i = 0; i<primes_length-1; i++){
		printf("%d, ", prime_numbers[i]);
	}
	printf("\n");
}
