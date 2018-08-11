/* 
Summer 2018
CSC263 Assignment 4 Question 7
CLRS Problem 21-1, Offline-Minimum
*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "disjoint.h"

typedef struct Kset Kset;

struct Kset {
	Kset *next;
	Kset *prev;
	Item *rep;
	int j;
};

void printExtracted(int *extracted, int m) {
	for (int i = 1; i <= m; i++) {
		printf("%d ", extracted[i]);
	}
	printf("\n");
}

void OffLineMinimum(Item **items, int *extracted, Kset **k_rep, int m) {
	int j;

	for (int i = 1; i <= 9; i++) {
		Item *rep = findSet(items[i]);
		j = rep->j;
		if (j != (m+1)) {
			extracted[j] = i;

			// find next k that isn't deleted
			// int l = j+1;
			// while (k_rep[l] == -1) {
			// 	l += 1;
			// }

			// Item *kj_rep = items[k_rep[j]];
			// Item *kl_rep = items[k_rep[l]];


			Item *kj_rep = (k_rep[j])->rep;
			printf("kj_rep is: %d | j on the set is: %d\n", kj_rep->v, (k_rep[j])->j);
			Kset *current = k_rep[j];
			Kset *next = current->next;
			Kset *prev = current->prev;
			if (next) {
				printf("we got a next\n");
				Item *nextrep = next->rep;
				if (nextrep) {
					printf("we got a next->rep, it is %d, its j is %d\n", next->rep->v, next->rep->j);
				}
			}
			

			if (!(next->rep)) {
				printf("next doesn't have rep\n");
				// K_l is empty, so just set its rep to K_j's rep, and update its j value
				int temp_j = (k_rep[j]->next)->j;
				(next)->rep = kj_rep;
				(next)->j = temp_j;

			} else {
				Item *temp = union_set(kj_rep, next->rep);
				temp->j = next->rep->j;
				printf("here now\n");
			}
			// k_rep[j] = -1; // "delete k_j"
			if (next) {
				prev->next = current->next;
			}
			
			if (prev) {
				next->prev = current->prev;
			}		
		}
	}
}

int isExtraction(char *s) {
	if (strcmp(s, "E") == 0 || strcmp(s, "e") == 0) { return 1; }
	return 0;
}

void parseString(char str[], Item **items, Kset **k_rep) {

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
			k_rep[j_index-1]->next = k_rep[j_index];
			k_rep[j_index]->j = j_index;
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
					k_rep[j_index-1]->next = k_rep[j_index];
					prev = current;
				}
			k_rep[j_index]->rep = prev;
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
	Kset **k_rep = malloc((m+2) * sizeof *k_rep);
	// additional +1 because we have one more K than there are extractions

	printf("Parsing sequence: \n");
	parseString(str, items, k_rep);
	
	OffLineMinimum(items, extracted, k_rep, m);

	// printf("Printing extracted array:\n");
	// printExtracted(extracted, m);

	// cleanup
	free(items);
	free(extracted);
	free(k_rep);
	
	return 0;
}

