#include <stdio.h>
#include <stdlib.h>

// open address hash table, using double hashing

#define m 256 
// table size. IRL should dynamic resize around 0.5, 0.6.
#define prime1 199
#define prime2 167

typedef struct HashItem {
	int key, value, delete_marker; // delete_marker = 1 if deleted, 0 otherwise
} HashItem;

HashItem newItem(int key, int value) {
	// helper function for new hash item
	HashItem x;
	x.key = key;
	x.value = value;
	x.delete_marker = 0;

	return x;
}

int doubleHash(int k, int i) { return (k % prime1 + (i * (k % prime2))); }

void hashInsert(HashItem **table, HashItem *x) {
	int i = 1;
	int current_index = doubleHash(x->key, i);
	HashItem *current = table[current_index];
	while (current != NULL) {
		if (current->delete_marker == 1) break;
		i++;
		current_index = doubleHash(x->key, i);
		current = table[current_index];
	}

	table[current_index] = x;
}

int hashSearch(HashItem **table, int k) {
	// look for HashItem with key k
	// if found, return the index of the slot where it is stored
	// otherwise return -1

	int i = 1;
	int current_index = doubleHash(k, i); // TODO if this works: simplify
	HashItem *current = table[current_index];
	while (current != NULL) {
		if (current->key == k) {
			if (current->delete_marker == 0) {
				return current_index;
			} else break;			
		}
		i++;
		current_index = doubleHash(k, i);
		current = table[current_index];
	}
	return -1;
}

void hashDelete(HashItem **table, int k) {
	// try to find HashItem with key k
	// if found, set its delete_marker to 1

	int index = hashSearch(table, k);
	if (index != -1) {
		(table[index])->delete_marker = 1;
	}
}

int main() {

	HashItem **table = malloc(m * sizeof(HashItem*)); 
	// array of pointers to HashItems
	
	HashItem x1 = newItem(2, 100);
	HashItem x2 = newItem(3, 67);
	HashItem x3 = newItem(4, -56);
	HashItem x4 = newItem(5, 78);
	HashItem x5 = newItem(6, 90);
	HashItem x6 = newItem(1, 10);

	hashInsert(table, &x1);
	hashInsert(table, &x2);
	hashInsert(table, &x3);
	hashInsert(table, &x4);
	hashInsert(table, &x5);
	hashInsert(table, &x6);

	int find_me = hashSearch(table, 1);
	printf("find_me: %d | key: %d | value: %d \n",
	 	find_me, table[find_me]->key, table[find_me]->value);

	hashDelete(table, 1);

	find_me = hashSearch(table, 1);
	printf("find_me: %d\n", find_me);

}