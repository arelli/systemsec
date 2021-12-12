#include <stdio.h>
#include <stdlib.h>
size_t* sieve_of_eratosthenes(int limit,size_t * prime_len){
	
	size_t *primes = NULL;
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
	primes = (size_t*)malloc(sizeof(size_t)*how_many_primes);
	
	for(int i; i<limit;i++){
		if (all_numbers[i]!=0){	
			printf("%d, \n", all_numbers[i]);
			primes[prime_index] = all_numbers[i];
			prime_index++;
		}
	}

	
	printf("There are in total %d prime numbers from 1 to %d .\n", how_many_primes,limit);
	return primes;
}


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



void main(){
	sieve_of_eratosthenes(50,NULL);
}
