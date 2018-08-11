#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "disjoint.h"

void printExtracted(int *extracted, int m) {
	printf("Printing extracted array:\n");
	for (int i = 1; i <= m; i++) {
		printf("%d ", extracted[i]);
	}
	printf("\n");
}

void OffLineMinimum(Item **items, int *k_rep, int m) {
	
	int j;
	int *extracted = malloc((m+1) * sizeof *extracted);

	for (int i = 1; i <= 9; i++) {
		printf("current testing i = %d, items[i-1]: %d | ", i, items[i-1]->v);
		Item *rep = findSet(items[i-1]);
		j = rep->j;
		printf("rep value: %d | j: %d | \n", rep->v, j);
		if (j != (m+1)) {
			printf("setting extracted [%d] to i = %d\n", j, i);
			extracted[j] = i;
			// find next k that isn't deleted
			int l = j+1;
			while (k_rep[l] == -1) {
				l += 1;
			}
			printf("j is %d | k_rep[j]: %d | l is now %d | k_rep[l]: %d\n", j, k_rep[j], l, k_rep[l]);
			Item *one;
			Item *two;
			one = items[k_rep[j]-1];
			two = items[k_rep[l]-1];
			if (k_rep[l] == 0) {
				// set is empty, just set the rep for l
				k_rep[l] = k_rep[j];
				one->j = l;
			} else {
				Item *temp = union_set(one, two);
				temp->j = l;
				printf("temp: %d\n", temp->v);
				k_rep[l] = temp->v;
			}
			k_rep[j] = -1; // "delete k_j"
		}
	}

	printExtracted(extracted, m);
}

void parseString(char str[], Item **items, int *k_rep) {

	int j_index = 1;

	char *list;
	list = strtok(str, ", ");
	char *endptr;
	int digit;

	Item *current;
	Item *prev = dummyItem(); // avoid segfault

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
			printf("stored item with value %d at items[] = %d\n", current->v, digit-1);
			// subsequent calls must pass null to get the next item
			printf("fine\n");

				printf("prev->j: %d\n", prev->j);
				if ((prev->j) == j_index) {

					prev = union_set(current, prev);
					printf("newset | j: %d --------- \n", prev->j);
					print(prev);
					printf("newset end --------- \n");
					printf("item %d is now in set with rep %d\n", current->v, findSet(current)->v);
				} else {
					printf("j changed, stored current->v %d into k_rep[%d]\n", current->v, j_index);
					prev = current;
				}
			
			
			
			printf("j_index: %d\n", j_index);
			k_rep[j_index] = prev->v;
		}
		
		list = strtok(NULL, ", ");

	}
}


int main() {

	int *k_rep;
	k_rep = malloc(8 * sizeof *k_rep);

	char str[] = "4, 8, E, 3, E, 9, 2, 6, E, E, E, 1, 7, E, 5";
	int n = 9;
	int m = 6;

	Item **items;
	items = malloc(n * sizeof *items);

	parseString(str, items, k_rep);

	OffLineMinimum(items, k_rep, m);

	free(items);

	return 0;
}

