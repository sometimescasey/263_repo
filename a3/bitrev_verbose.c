#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 12

typedef struct kbit {
	int *array;
	int k;
} kbit;

int* to_kbit(int n, int k) { // O(k)
	int *kbit = malloc(sizeof(int) * k);
	int remain = n;
	int a_i;
	int denom;
	for (int i = 0; i < k; i++) { // <- O(k) step
		denom = pow(2,(k-i-1));
		a_i = (remain / denom);
		printf("i: %d | remain: %d | denom: %d | a_i: %d\n", i, remain, denom, a_i);
		kbit[i] = a_i;
		if (a_i > 0) {
			remain -= denom;
		}
	}
	return kbit; 
}

kbit new_kbit(int index, int k) {
	// helper function to make a new kbit
	int* kbit_array = to_kbit(index, k);
	kbit kbit = {.array = kbit_array, .k = k};
	free(kbit_array);
	return kbit;
}

int get_k(int n) {
	// return k
	int k = ceil(log2(n)); // ceiling is k
	printf("k is %d\n", k);
	return k;
}

int to_index(kbit kbit) { // O(k)
	int index = 0;
	int power;
	int k = kbit.k;
	for (int i = 0; i < k; i++) {
		power = pow(2, k-i-1);
		if (kbit.array[i] == 1) {
			index += power;
		}
	}
	return index;
}

void rev(kbit *kbit) { // O(k)
	int* array = kbit->array;
	int k = kbit->k;
	int placeholder;

	for (int i = 0; i < k/2; i++) { // <- O(k) step. Stop halfway, or they'll all get swapped back again
		placeholder = array[i];
		array[i] = array[k-i-1];
		array[k-i-1] = placeholder; 
	}
}

void print_kbit(kbit kbit) {
	for (int i = 0; i < kbit.k; i++) {
		printf("%d", kbit.array[i]);
	}
	printf("\n");
}

int* bitrev_permute(int *array, int n, int k) { // O(nk)
	kbit kbit_index;
	int rev_index;

	int *permuted_arr = malloc(n * sizeof(int));

	for (int i = 0; i < n; i++) { // <- loop runs n times
		printf("Processing i = %d\n", i);
		// get kbit of index
		kbit_index = new_kbit(i, k); // <- O(k) to convert to k-bit representation
		// reverse it
		rev(&kbit_index); // <- O(k) to reverse
		// back to base 10
		rev_index = to_index(kbit_index); // <- O(k) to turn back into index (base 10)
		printf("kbit_index: ");
		print_kbit(kbit_index);
		printf("| i: %d | rev_index: %d\n", i, rev_index);
		// if in reversed index is in array AND not equal to i, swap the values
		if (rev_index != i && rev_index < n) {
			permuted_arr[rev_index] = array[i];
			permuted_arr[i] = array[rev_index]; 
		}
		else {
			permuted_arr[i] = array[i];
		}
	}
	return permuted_arr;
}

int main(int argc, char** argv) {
	int t;
	if (argc == 2) {
		t = strtol(argv[1], NULL, 10);
	} else {
		printf("Usage: bitrev number\n");
		exit(1);
	}

	// unit test for exp()
	int q = 5;
	printf("testing exp(): \n");
	printf("%d to the power of %d = %d\n", t, q, (int) pow(t,q));
	
	// unit test for new_kbit()
	printf("\ntesting new_kbit(): \n");
	kbit kbit = new_kbit(t, 4);

	// Todo: helper fxn for new kbit
	// test if working!
	print_kbit(kbit);
	rev(&kbit);
	printf("rev'd\n");
	print_kbit(kbit);

	// test bitrev_permute
	int n = 12;
	int test[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
	int k = get_k(n);
	int *pointer;
	pointer = bitrev_permute(test, n, k);
	for (int i = 0; i < n; i++) {
		printf(" %d ", pointer[i]);
	}
	printf("\n");
	free(pointer);

	return 0;

}