#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

// model as a 1D array with mxn elements, starting at 0, going to m*n-1

typedef struct cell {
	int i;
	int j;
} cell;

typedef struct tableau {
	int *elements; //array of mxn elements
	int m;
	int n;
} tableau; // <-- this is alias so you don't have to type struct

void initEmptyTableau(tableau *t) {
// set all elements of t to negative inf
	int m = t->m;
	int n = t->n;
	int len = m*n;
	for (int i=0; i < len; i++) {
		t->elements[i] = -INT_MAX;
	}
}

int getIndex(tableau *t, cell c) {
	int i = c.i;
	int j = c.j;
	int m = t->m;
	return (j-1)*m + i - 1;
}

int get(tableau *t, cell c) {
	int index = getIndex(t, c);
	return t->elements[index];
}

int cellsEqual(tableau *t, cell a, cell b) {
	if (getIndex(t, a) == getIndex(t, b)) {
		return 1;
	}
	else { return 0; }
}

// helper function to create new empty tableau
tableau* getNewTableau(int m, int n) {
	tableau *newTableau = malloc(sizeof(tableau)); 
	newTableau->m = m; // deference pointer newTableau, set m
	newTableau->n = n;
	newTableau->elements = malloc(m*n*sizeof(int));
	initEmptyTableau(newTableau);
	return newTableau;
}

cell lookUp(cell c) {
	cell result = {c.i, c.j-1};
	return result; 	
}

cell lookDown(cell c) {
	cell result = {c.i, c.j+1};
	return result; 	
}

cell lookLeft(cell c) {
	cell result = {c.i-1, c.j};
	return result; 	
}

cell lookRight(cell c) {
	cell result = {c.i+1, c.j};
	return result; 	
}

cell cellAt(int i, int j) {
	cell result = {i, j};
	return result;
}

int isIn(tableau *t, cell c) { // is c in t?
	if (c.i > 0 && c.j > 0 && c.i <= t->m && c.j <= t->n ) {
		return 1;
	}
	else {
		return 0;
	}
}

void set(tableau *t, cell c, int val) {
	int i = c.i;
	int j = c.j;
	int m = t->m;
	int index = j*m + i;
	t->elements[index] = val;
}

void exchange(tableau *t, int a, int b) {
// swap elements at indices a and b
	int temp = t->elements[a];
	t->elements[a] = t->elements[b];
	t->elements[b] = temp;
}

void bubbleDown(tableau *t, cell c) {
// recursively bubble down cell c
// compare down, compare right, exchange if smaller than either/both
// repeat until bigger than both

	cell pos = c; // track position in the tableau
	int c_v = get(t,c);

	char look; // which directions can we look?
	cell d = lookDown(c);
	cell r = lookRight(c);
	
	if(isIn(t,d)) {
		if(isIn(t,r)) {
			look = 'b'; //can look d and r
		}
		else { look = 'd'; }  //down
	}
	else {
		if(isIn(t,r)){ look = 'r'; } //right
	}

	char exch;
	int d_v;
	int r_v;

	switch(look) {
		case 'b': // check both
			d_v = get(t, d);
			r_v = get(t, r);

			if (c_v >= d_v && c_v >= r_v) { ; } // do nothing; we're fine
			else if (c_v < d_v && c_v >= r_v) { exch = 'd'; }	
			else if (c_v < r_v && c_v >= d_v) { exch = 'r'; }
			else { // c smaller than both; swap c with the bigger one
				if (r_v > d_v) { exch = 'r'; }
				else { exch = 'd'; }
			}
			break;

		case 'r': // check r
			r_v = get(t, r);
			if (c_v < r_v) { exch = 'r'; }
			break;
		
		case 'd': //check d
			d_v = get(t, d);
			if (c_v < d_v) { exch = 'd'; }
			break;
	}

	// do exchange
	switch(exch) {
		case 'd':
			exchange(t, getIndex(t, c), getIndex(t, d));
			pos = d;
			break;
		case 'r':
			exchange(t, getIndex(t, c), getIndex(t, r));
			pos = r;
			break;
		default:
			break; // no exchange
	}

	// if exchanged, recursively call bubbleup on the two exchanged positions
	// this is key!! an exchange can push something down to a bad place
	if (0 == cellsEqual(t, pos, c)) { 
		bubbleDown(t, pos);
		bubbleDown(t, c); }
}

void bubbleUp(tableau *t, cell c) {
// recursively bubble up cell c
// compare up, compare left, exchange if bigger than either/both
// repeat until smaller than both

	cell pos = c; // track position in the tableau
	int c_v = get(t,c);

	char look; // which directions can we look?
	cell u = lookUp(c);
	cell l = lookLeft(c);
	
	if(isIn(t,u)) {
		if(isIn(t,l)) {
			look = 'b'; //can look up and left
		}
		else { look = 'u'; }  //up
	}
	else {
		if(isIn(t,l)){ look = 'l'; } //left
	}

	char exch;
	int u_v;
	int l_v;

	switch(look) {
		case 'b': // check both
			u_v = get(t, u);
			l_v = get(t,l);

			if (c_v <= u_v && c_v <= l_v) { ; } // do nothing; we're fine
			else if (c_v > u_v && c_v <= l_v) { exch = 'u'; }	
			else if (c_v > l_v && c_v <= u_v) { exch = 'l'; }
			else { // c bigger than both; swap c with the smaller one
				if (l_v < u_v) { exch = 'l'; }
				else { exch = 'u'; }
			}
			break;

		case 'l': // check left
			l_v = get(t,l);

			if (c_v > l_v) { exch = 'l'; }
			break;
		
		case 'u': //check up
			u_v = get(t, u);

			if (c_v > u_v) { exch = 'u'; }
			break;
	}

	// do exchange
	switch(exch) {
		case 'u':
			exchange(t, getIndex(t, c), getIndex(t, u));
			pos = u;
			break;
		case 'l':
			exchange(t, getIndex(t, c), getIndex(t, l));
			pos = l;
			break;
		default:
			break; // no exchange
	}

	// if exchanged, recursively call bubbleup on the two exchanged positions
	// this is key!! an exchange can push something down to a bad place
	if (0 == cellsEqual(t, pos, c)) { 
		bubbleUp(t, pos);
		bubbleUp(t, c); }
}

void printTableau(tableau *t) {
	for (int i=0; i < t->m*t->n; i++) {
		if (i % t->m == 0) {
			printf("\n"); // line break every mth item
		}
		if (t->elements[i] == -2147483647) {
			printf("-Inf\t");
		}
		else {
			printf("%d\t", t->elements[i]);
		}
	}
	printf("\n");
}

int extractMax(tableau *t) {
// max always at (0,0) in Reverse Young Tableau, or t.elements[0]
	int max = t->elements[0];
	if(max == -INT_MAX){
		printf("ERROR: tableau is empty. No max to return\n");
	} 
	else {
		int last = t->m*t->n-1;
		// swap max and last item (smallest)
		exchange(t, 0, last);
		// empty out the last position
		t->elements[last] = -INT_MAX;
		// recursively bubble down the element at 0th position
		cell topLeftCell = cellAt(1,1);
		bubbleDown(t, topLeftCell);
	}
	printf("\nmax %d extracted -----\n", max);
	printTableau(t);
	return max;	
}

void insert(tableau *t, int value) {
	int bottomRight = t->elements[t->m*t->n-1];
// always put at bottom-right most position and bubble up
	if(bottomRight != -INT_MAX) {
	// tableau is full
		printf("ERROR: tableau full, cannot insert\n");
	}
	else {
		t->elements[t->m*t->n-1] = value;
		cell bottomRightCell = cellAt(t->m, t->n);
		bubbleUp(t, bottomRightCell);
	}

	printf("\n---- Success inserting %d ------", value);
	printTableau(t);

}

void insertMany(tableau *t, int *arr, int len) {
	for (int i = 0; i < len; i++) {
		insert(t, arr[i]);
	}
}

int search(tableau *t, cell c, int value) {
	// search if value is in tableau
	// start top right
	// if too big, everything to its left is by definition also too big so check the next row
	// if too small, everything below it is by def too small, so move left
	// rinse and repeat until found or have eliminated entire tableau
	cell current = c;
	int cur_val = get(t, c);

	if (cur_val == value) {
		int index = getIndex(t, current);
		printf("found at getIndex() = %d\n", index);
		return index;
	}
	else if (cur_val > value) {
		// go down a row
		current.j += 1;
	} else {
		// go left a column
		current.i -=1;
	}

	// as long as new cell is valid, recurse on search
	if (isIn(t, current)) {
		return search(t, current, value);
	}
	
	// if all of the above fails to return anything
	return -1;
}

int searchFromStart(tableau *t, int value) {
	cell startCell = cellAt(t->m, 1);
	return search(t, startCell, value);
}

void sort(tableau * t) {
	int dim = (t->m) * (t->n); 
	printf("running sort");
	for (int i = 0; i < dim; i++) {
		printf("%d ",extractMax(t));
	}
}

int main() {
	// make mxn tableau

	tableau *t = getNewTableau(2,3);
	int len = 5;
	int *values = malloc(len * sizeof(int));
	values[0] = 5;
	values[1] = 2;
	values[2] = 7;
	values[3] = 1;
	values[4] = 9;
	values[4] = 8;
	

	insertMany(t, values, 5);
	sort(t);
	extractMax(t);
	extractMax(t);
	int value = 2;
	int search = searchFromStart(t, value);
	printf("searched for %d, found it at %d \n", value, search);
	
	
	return 0;
}



