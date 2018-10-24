/* 
Summer 2018
CSC263 Assignment 3 Question 4

Describe an implementation of the BITREVERSEDINCREMENT procedure on A that allows 
the bit-reversal permutation on an n-element array to be performed in time O(n).

Note: compile needs -lm flag for <math.h>:
gcc -Wall -o kbit_rev_permute kbit_rev_permute.c -lm
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

typedef struct kbit_tuple {
	int dec;
	int *kbit_array;
	int k;
} kbit_tuple;

kbit_tuple new_kbit_tuple(int k) {
	// creates new kbit counter at 0
	kbit_tuple kbit_tuple = {
		.k = k,
		.dec = 0,
		.kbit_array = malloc(sizeof(int) * k)
	};
	kbit_tuple.kbit_array[0] = 0;
	return kbit_tuple;
}

int increment(kbit_tuple *kbit_tuple) {   // amortized O(1) per increment
	// takes a pointer to kbit_tuple, increments it, and returns the decimal value
	int i = 0;
	
		while(kbit_tuple->kbit_array[i] == 1) {
			kbit_tuple->kbit_array[i] = 0;
			kbit_tuple->dec -= pow(2, kbit_tuple->k-i-1);
			i++;
		}
		// upon encountering the first non-1 position from the left, 
		// increment it to 1 and add the value it represents
		if (i < kbit_tuple->k) {
			kbit_tuple->kbit_array[i] = 1;
			kbit_tuple->dec += pow(2, kbit_tuple->k-i-1);
		}
	

	return kbit_tuple->dec;
}

int get_k(int n) {
	int k = ceil(log2(n));
	return k;
}

int* kbit_rev_permute(int *array, int n) {
	// take in array of n numbers, run kbit-reversal permutation in O(n) amortized time
	int *new_array = malloc(sizeof(int) * n);
	kbit_tuple rev_counter = new_kbit_tuple(get_k(n));
	
	int rev_i = 0; // start rev_i at 0: rev(0) is always 0 for any k
	
	for (int i = 0; i < n; i++) { // for-loop: runs n times
		// if the kbit-reversed index position is in the array, swap
		if (rev_i != i && rev_i < n) {
			new_array[rev_i] = array[i];
		} else {
			new_array[i] = array[i];
		}
		rev_i = increment(&rev_counter);
	}

	free(rev_counter.kbit_array);
	return new_array;
}

int main(int argc, char **argv) {

	// TEST CASE 1 ------------

	// int n = 4;
	// int test[] = {0, 1, 2, 3};

	// TEST CASE 2 -----------

	int n = 10;
	int *test = malloc(sizeof(int) * n);

	for (int i = 0; i<n; i++) {
		test[i] = i;
	}

	// ------------------------

	int *pointer;
	pointer = kbit_rev_permute(test, n);
	for (int i = 0; i < n; i++) {
		printf(" %d ", pointer[i]);
	}
	printf("\n");
	
	// cleanup
	free(test);
	free(pointer);

	return 0;
}