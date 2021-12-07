#include "rsa.h"
#include "utils.h"

/*
 * Sieve of Eratosthenes Algorithm
 * https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes
 *
 * arg0: A limit
 * arg1: The size of the generated primes list. Empty argument used as ret val
 *
 * ret:  The prime numbers that are less or equal to the limit
 */
size_t *
sieve_of_eratosthenes(int limit, int *primes_sz)
{
	size_t *primes;
	
	/* used to store all numbers, including primes */
	size_t *all_numbers = (int*)malloc(sizeof(int)*limit);
	for(int i=2;i<=limit){
		all_numbers[i-2]=i;
	}

	int index = 0;

	/* the step of the sieve */
	size_t current_step = 2;
	size_t last_step = 2;

	/* used to find the next step */
	bool not_found = true;
	int temp_index = 0;

	while(true){
		/* there are only first numbers left in our array */
		if (current_step*current_step > limit){
			break;
		}
		
		/* start from the beginning, increasing the sieving step */
		if (index>=limit){
			index = 0;
			temp_index = 0;
			
			/* find the index of the current step in all_numbers[] */
			while(true){
				temp_index++;
				if (all_numbers[temp_index]==current_step)
					break;  
				if (temp_index>limit)
					break;  /* error state */
			}

			/* find the next non-zero number in the sieve
			 * to serve as the next step distance        */
			not_found = true;
			while(not_found){
				temp_index ++;
				if(all_numbers[temp_index]!=0){
					last_step = current_step;
					current_step = all_numbers[temp_index];
					not_found = false;
				}
			}
		}
		
		/* set this element as non-prime, with the zero notation */
		index += current_step; 
		all_numbers[index] = 0;
	}

	int how_many_primes = 0;
	for(int i; i<limit-2;i++){
		if (all_numbers[i]!=0){
			how_many_primes++;
			printf("%d, \n");
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

	/* TODO */

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
