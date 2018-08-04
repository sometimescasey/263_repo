#include <stdio.h>
#include <stdlib.h>

typedef struct Item Item;

struct Item {
	Item *f; // forward pointer, disjoint set tree
	Item *b; // backward pointer, circular ll for printing
	int v;   // value
	int j;   // j-index, for the purposes of the question. 
	         // Note this may never be updated on lower nodes 
			 // so we must always check on the representative via findSet()
	int rank;
};

Item* findSet(Item *item) {
	// given item, find its representative. Use path compression
	if (item->f != item) {
		item->f = findSet(item->f);
	}
	return item->f;
}

Item* union_set(Item *a, Item *b) {
	Item *a_rep = findSet(a); // basically linear
	Item *b_rep = findSet(b); // basically linear
	Item *hi_rank;
	Item *lo_rank;	
	
	// rest of operations take place in constant time
	if (a_rep != b_rep) { // check they aren't already in same set
		if (a_rep->rank > b_rep->rank) { // union-by-rank
			hi_rank = a_rep;
			lo_rank = b_rep;	 	
		} else {
			hi_rank = b_rep;
			lo_rank = a_rep;
			if (hi_rank->rank == lo_rank->rank) {
				hi_rank->rank += 1;
			}	
		}
	
		// DST-forest, point lo to hi			
		lo_rank->f = hi_rank;

		// circular linked list portion, switch the backwards pointers
		Item *temp = hi_rank->b;
		hi_rank->b = lo_rank->b;
		lo_rank->b = temp;
	}

	return hi_rank;
}

Item* dummyItem() {
	// make and return a dummy item to address segfaults
	Item *i = malloc(sizeof(Item));
	i->v = -1;
	i->f = i;
	i->b = i;
	i->rank = -1;
	i->j = -1;
	return i;
}

Item* makeSet(int n, int j) {
	Item *i = malloc(sizeof(Item));
	i->v = n;
	i->f = i;
	i->b = i;
	i->rank = 0;
	i->j = j;

	return i;
}

void print(Item *i) { // just traverse the backward edges until we get to the start. O(|V|) runtime
	Item *current = i;
	while (current->b != i) {
		printf("%d\n", current->v);
		current = current->b;
	}
	printf("%d\n", current->v); // one more print to get the last value
}

// int main() {

// 	Item *a = makeSet(1);
// 	Item *b = makeSet(2);
// 	Item *c = makeSet(3);

// 	a = union_set(a,b);
// 	a = union_set(a,c);

// 	print(a);

// 	return 0;
// }