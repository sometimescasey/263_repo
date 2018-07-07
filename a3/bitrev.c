#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define N 12

typedef struct kbit {
	int *array;
	int length;
} kbit;

int get_k(int n) {
	// return k
	int k = log2(n); // floor is k
	printf("k is %d\n", k);
	return k;
}

int* to_kbit(int n) {
	int k = get_k(n);
	int *kbit = malloc(sizeof(int) * k);
	int remain = n;
	int a_i;
	int denom;
	for (int i = 0; i < k; i++) {
		denom = pow(2,(k-i));
		printf("remain: %d | denom: %d\n", remain, denom);
		a_i = (remain / denom);
		printf("here\n");
		kbit[i] = a_i;
		if (a_i > 0) {
			remain -= denom;
		}
	}
	return kbit; 
}

void print_kbit(kbit kbit) {
	for (int i = 0; i < kbit.length; i++) {
		printf("|i=%d|", i);
		printf("%d", kbit.array[i]);
	}
	printf("\n");
}

int main() {

	// unit test for exp()
	int q = 5;
	printf("testing exp(): \n");
	printf("%d to the power of %d = %d\n", N, q, (int) pow(N,q));
	
	// unit test for to_kbit()
	int* kbit_array = to_kbit(N);
	int k = get_k(N);

	kbit kbit = {.array = kbit_array, .length = k};

	// to do: make this a struct to make stuff easier
	// or sizeof()?
	print_kbit(kbit);

	return 0;

}