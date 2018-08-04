#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include "disjoint.h"

void printExtracted(int *extracted, int m) {
	for (int i = 1; i <= m; i++) {
		printf("%d ", extracted[i]);
	}
	printf("\n");
}

void OffLineMinimum(Item **items, int *extracted, int *k_rep, int m) {
	int j;
	
	for (int i = 1; i <= 9; i++) {
		Item *rep = findSet(items[i]);
		j = rep->j;
		if (j != (m+1)) {
			extracted[j] = i;

			// find next k that isn't deleted
			int l = j+1;
			while (k_rep[l] == -1) {
				l += 1;
			}

			Item *kj_rep = items[k_rep[j]];
			Item *kl_rep = items[k_rep[l]];
			if (k_rep[l] == 0) {
				// K_l is empty, so just set its rep to K_j's rep, and update its j value
				k_rep[l] = k_rep[j];
				kj_rep->j = l;
			} else {
				Item *temp = union_set(kj_rep, kl_rep);
				temp->j = l;
				k_rep[l] = temp->v;
			}
			k_rep[j] = -1; // "delete k_j"
		}
	}
}

int isExtraction(char *s) {
	if (strcmp(s, "E") == 0 || strcmp(s, "e") == 0) { return 1; }
	return 0;
}

void parseString(char str[], Item **items, int *k_rep) {

	int j_index = 1;

	char *token;
	token = strtok(str, ", ");
	char *endptr;
	int digit;

	Item *current;
	Item *prev = dummyItem(); // prevent seg fault

	while (token != NULL) {
		printf("%s\n", token);
		if (isExtraction(token)) {
			j_index += 1;
		} else {

			digit = strtol(token, &endptr, 10);
			if (endptr == token) {
				fprintf(stderr, "Parsing problem: strtol encountered a non-digit token %s\n", token);
				exit(1);
			}

			current = makeSet(digit, j_index);

			items[digit] = current; // store pointers for reference later

				if ((prev->j) == j_index) {
					prev = union_set(current, prev);
				} else {
					prev = current;
				}
			k_rep[j_index] = prev->v;
		}
		
		// subsequent calls to strtok must pass null to get the next item
		token = strtok(NULL, ", ");
	}
}

int main() {

	char str[] = "4, 8, E, 3, E, 9, 2, 6, E, E, E, 1, 7, E, 5";
	int n = 9;
	int m = 6;

	// +1 to all array sizes for not being zero indexed
	// we waste one slot on each, but the math is easier to work with
	Item **items = malloc((n+1) * sizeof *items);
	int *extracted = malloc((m+1) * sizeof *extracted);
	int *k_rep = malloc((m+2) * sizeof *k_rep);
	// additional +1 because we have one more K than there are extractions

	printf("Parsing sequence: \n");
	parseString(str, items, k_rep);
	
	OffLineMinimum(items, extracted, k_rep, m);

	printf("Printing extracted array:\n");
	printExtracted(extracted, m);

	// cleanup
	free(items);
	free(extracted);
	free(k_rep);
	
	return 0;
}

