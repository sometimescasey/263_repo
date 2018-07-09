#include <stdio.h>
#include <stdlib.h>
#define MAX 10
#define DARY 3
#define INT_MIN -99999 // substitute for -Inf


// heap is an array of ints
void printHeap(int *arr, int *heapsize) {
	for (int i = 0; i < *heapsize; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");
}

void exchange(int *arr, int i, int j) {
	// exchange the items at index a and b
	int one = arr[i]; // parent value
	int two = arr[j]; // child value

	arr[j] = one;
	arr[i] = two;
}

int getMax(int *arr) {
	int max = arr[0]; // always first item in array
	printf("getMax(): %d\n", max);
	return max;
}

void maxHeapify(int *arr, int i, int *heapsize) { // maxHeapify the heap rooted at i
	// has DARY children
	int largest = i;
	int dary = DARY;
	for (int k = 1; k <= dary; k++) {
		int child = dary*i + k;
		if (arr[child] > arr[largest]) {
			largest = child;
		}
	}

	if (largest != i) {
		// swap with largest, then maxHeapify the tree rooted at largest
		exchange(arr, i, largest);
		maxHeapify(arr, largest, heapsize);
	}

}

int extractMax(int *arr, int *heapsize) {
	if (*heapsize < 1) {
		printf("ERROR: heap is empty. Nothing to return.\n");
		return INT_MIN;
	}
	else {
		int max = getMax(arr);
	// swap top with last item, get rid of last item
		int last = arr[*heapsize-1];
		arr[0] = last;
		*heapsize = *heapsize - 1;
		maxHeapify(arr, 0, heapsize);
		printf("max extracted\n");
		return max;
	}
}

void increaseKey(int *arr, int i, int key) {
	if (key < arr[i]){
		printf("ERROR: %d is less than current arr[i] %d\n", key, i);
	}
	else {
		arr[i] = key;
		while (i > 0) {
			int j = (i-1) / DARY; // parent
			if (arr[j] >= arr[i]) {
				// parent is greater
				break;
			} else {
				// parent is smaller; swap them
				exchange(arr, i, j);
				i = j;
			}

		}
	}
}

void insert(int *arr, int key, int *heapsize) {
	printf("Inserting %d\n", key);
	if (*heapsize == MAX) {
		printf("ERROR: not inserted. Heap is already at max size %d\n", *heapsize);
	} else {
		*heapsize = *heapsize + 1; // expand queue by 1
		arr[*heapsize-1] = INT_MIN; // set new, last item to large negative value
		increaseKey(arr, *heapsize-1, key); // update its priority to "key" 
											// and bubble it up to the right place
		printf("heapsize: %d\n", *heapsize);
		printHeap(arr, heapsize);
	}
}



void delete(int *arr, int i, int heapsize) {

}

void buildMaxHeap(int *arr, int *input, int *heapsize) {
	// check that input length <= arr length

}

int main() {

	int *arr = malloc(MAX * sizeof(int));
	int heapsize = 0;

	insert(arr, 10, &heapsize);
	insert(arr, 2, &heapsize);
	insert(arr, 3, &heapsize);
	insert(arr, 5, &heapsize);
	insert(arr, 11, &heapsize);
	extractMax(arr, &heapsize);
	printHeap(arr, &heapsize);

	free(arr);
	return 0;

}