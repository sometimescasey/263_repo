#include <stdio.h>
#include "disjoint.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

#define MAXINSERTS 100 // for malloc purposes

const int n = 9; // inserts
const int m = 6; // extracts



void OffLineMinimum(int m, int n, Item **items) {
	int *extracted = malloc(sizeof(int) * m);
	for (int i = 0; i < n; i++) {
		int j = findSet(items[i])->j;
		if (j != (m+1)) {
			extracted[j] = i;

		}
	}
}

void initialize_k_exists(int *k_exists) {
	for (int i = 0; i < m+1; i++) {
		k_exists[i] = 0;
	}
}

int main() {
	// int *k_exists[m+1];
	// initialize_k_exists(k_exists);




	Item **items = malloc(sizeof(Item*) * n);
	char str[] = "4, 8, E, 3, E, 9, 2, 6, E, E, E, 1, 7, E, 5";
	int j_index = 1;

	char *list;
	list = strtok(str, ", ");
	char *endptr;
	int digit;

	Item *current;
	Item *prev;

	while (list != NULL) {
		printf("%s\n", list);
		if (strcmp(list, "E") == 0) { // replace with more robust function
			j_index += 1;
		} else {

			digit = strtol(list, &endptr, 10);
			if (endptr == list) {
				fprintf(stderr, "Parsing problem: strtol encountered a non-digit\n");
				exit(1);
			}

			current = makeSet(digit, j_index);

			items[digit-1] = current; // keep pointer for use later
			// subsequent calls must pass null to get the next item
			if (!(prev->j)) {

				prev = current;
			}
			else if ((prev->j) == j_index) {

				prev = union_set(current, prev);
				printf("newset | j: %d --------- \n", prev->j);
				print(prev);
				printf("newset end --------- \n");
			} else {
				prev = current;
			}
		}
		
		list = strtok(NULL, ", ");

	}



	return 0;
}

