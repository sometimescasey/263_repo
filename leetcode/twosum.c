#include <stdio.h>
#include <stdlib.h>
/**
 * Note: The returned array must be malloced, assume caller calls free().
 */
int* twoSum(int* nums, int numsSize, int target) {
    int* answer;
    answer = malloc(2 * sizeof(int));
    int half_t = target / 2; // floor

}

int binSearch(int target, int* array, int start, int end) { 
	// returns index of item or first number bigger than item, between start and end
	// returns -1 if not found
	int mid = (start + end) / 2;
	if (target == array[mid]) {
		return mid;
	}
	else if (start == end) { // one item, and it wasn't right
		return -1;
	}
	else if (target < array[mid]) // look left {
		// start is still start
		// new end
		binSearch(int target, int* array, start, mid-1)
	}
	else { // look right
		// end still end
		// new start
		binSearch(int target, int* array, mid+1, end)	
	}
}



int main() {
	int* example[4];
	example[0] = 2;
	example[1] = 7;
	example[2] = 11;
	example[3] = 5;

	int* result = twoSum(example, 4, 9)
	printf("%d, ", result[0]);
	printf("%d\n", result[1]);



	
}