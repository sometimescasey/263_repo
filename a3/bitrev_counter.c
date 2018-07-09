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

int increment(kbit_tuple *kbit_tuple) {
	// takes a pointer to kbit_tuple, increments it, and returns the decimal value
	int i = 0;
	// TODO: print to prove amort cost
	// printf("kbit_tuple->k: %d\n", kbit_tuple->k);
	
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
	// return k
	int k = ceil(log2(n)); // ceiling is k
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

	return new_array;
}

int main(int argc, char **argv) {

	// int upto;
	
	// if (argc == 2) {
	// 	upto = strtol(argv[1], NULL, 10);
	// } else {
	// 	printf("Usage: bitrev_counter number\n");
	// }

	// kbit_tuple kbit_tuple = new_kbit_tuple(2);

	// int i = 0;
	// int j;
	
	// printf("%d\n", kbit_tuple.dec);

	// while (i < upto) {
	// 	j = increment(&kbit_tuple);
	// 	printf("%d\n", j);
	// 	i++;
	// }

	int n = 4;
	int test[] = {0, 1, 2, 3};

	int *pointer;
	pointer = kbit_rev_permute(test, n);
	for (int i = 0; i < n; i++) {
		printf(" %d ", pointer[i]);
	}
	printf("\n");
	free(pointer);

	return 0;
}