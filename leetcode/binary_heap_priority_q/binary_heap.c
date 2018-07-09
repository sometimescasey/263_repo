#include <stdio.h>
#include <stdlib.h>
#define MAX 10 // # of ints in queue
#define ary 2;
#define INT_MIN -99999;


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
	int l_index = 2*i + 1;
	int r_index = 2*i + 2;
	int largest = i;

	// check that l and r indices actually exist before doing anything
	if (l_index < *heapsize && arr[l_index] > arr[i]) { 
		largest = l_index;
	}
	if (r_index < *heapsize && arr[r_index] > arr[largest]) {
		largest = r_index;
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
			int j = (i-1) / 2; //parent. different from slide definition due to indexing
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
		arr[*heapsize-1] = INT_MIN; // set new, last item to really negative
		increaseKey(arr, *heapsize-1, key); // update its priority to "key" and bubble it up to the right place
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

	insert(arr, 11, &heapsize);
	insert(arr, 2, &heapsize);
	insert(arr, 3, &heapsize);
	insert(arr, 10, &heapsize);
	insert(arr, 11, &heapsize);
	insert(arr, 15, &heapsize);
	insert(arr, 12, &heapsize);
	insert(arr, 9, &heapsize);
	insert(arr, 2, &heapsize);
	insert(arr, 88, &heapsize);
	extractMax(arr, &heapsize);
	printHeap(arr, &heapsize);



	free(arr);
	return 0;

}