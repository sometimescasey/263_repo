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
	printf("i: %d\n", i);
	int j = c.j;
	printf("j: %d\n", j);
	int m = t->m;
	printf("m: %d\n", m);
	printf("(j-1)*m + i - 1: %d\n", (j-1)*m + (i-1));
	if ((j-1)*m + i - 1 < 0) {
		printf("(j-1)*m + i - 1 returning negative value: %d ----------- \n", (j-1)*m + i - 1);
	}
	return (j-1)*m + i - 1;
}

int get(tableau *t, cell c) {
	int index = getIndex(t, c);
	// printf("getIndex: %d\n", index);
	printf("t->elements[%d]: %d\n", index, t->elements[index]);
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
	printf("isIn c.i: %d\n", c.i);
	printf("isIn c.j: %d\n", c.j);
	if (c.i > 0) { printf("c.i > 0\n");}
	if (c.j > 0) { printf("c.j > 0\n");}
	if (c.i <= t->m) { printf("c.i <= t->m\n");}
	if (c.j <= t->n) { printf("c.j <= t->n\n");}
	
	if (c.i > 0 && c.j > 0 && c.i <= t->m && c.j <= t->n ) {
		return 1;
	}
	else {
		printf("isIn returned 0\n");
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
	printf("exch ran\n");
}

void bubbleDown(tableau *t, cell c) {
// recursively bubble down cell c
// compare down, compare right, exchange if smaller than either/both
// repeat until bigger than both

	cell pos = c; // track position in the tableau

	int c_v = get(t,c);

	cell d = lookDown(c);
	int d_v = get(t, d);

	cell r = lookRight(c);
	int r_v = get(t,r);

	char exch;

	if (isIn(t, d)) {
		if (isIn(t, r)) {
		// can look d/r
			if (c_v >= d_v && c_v >= r_v) { ; } // do nothing; we're fine
			else if (c_v < d_v) { exch = 'd'; } // swap c and d	
			else if (c_v < r_v) { exch = 'r'; } // swap c and r
			else { // smaller than both; swap c with the bigger one
				if (r_v > d_v) { exch = 'r'; }
				else { exch = 'd'; }
			}
		}
		else {
		// can only look d
			if (c_v < d_v) { exch = 'd'; }
		}
	}
	else {
		if (isIn(t, r)) {
		// can only look right
			if (c_v < r_v) { exch = 'r'; }
		} 
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

	// check if need to recurse
	if (0 == cellsEqual(t, pos, c)) { 
		printf("call bubbleDown again");
		bubbleDown(t, pos); }
}

void bubbleUp(tableau *t, cell c) {
// recursively bubble up cell c
// compare up, compare left, exchange if bigger than either/both
// repeat until smaller than both

	cell pos = c; // track position in the tableau

	int c_v = get(t,c);

	char look = 'n'; // none

	cell u = lookUp(c);
	cell l = lookLeft(c);
	if(isIn(t,u)) {
		if(isIn(t,l)) {
			look = 'b'; //both
		}
		else {
			look = 'u'; //up
		}
	}
	else {
		if(isIn(t,l)){
			look = 'l'; //left
		}
	}

	char exch;
	int u_v;
	int l_v;

	switch(look) {
		case 'b': // check both
			u_v = get(t, u);
			printf("u_v: %d\n", u_v);

			l_v = get(t,l);
			printf("l_v: %d\n", l_v);

			if (c_v <= u_v && c_v <= l_v) { printf("donothing\n"); } // do nothing; we're fine
			else if (c_v > u_v) { exch = 'u'; }	
			else if (c_v > l_v) { exch = 'l'; }
			else { // c bigger than both; swap c with the smaller one
				if (l_v < u_v) { exch = 'l'; }
				else { exch = 'u'; printf("should be this\n"); }
			}
			break;

		case 'l': // check left
			l_v = get(t,l);
			printf("l_v: %d\n", l_v);
			if (c_v > l_v) { exch = 'l'; }
			break;
		
		case 'u': //check up
			u_v = get(t, u);
			printf("u_v: %d\n", u_v);
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

	// if exchanged, call bubbleup on the two exchange positions
	// this is key!! an exchange can push something down to a bad place
	if (0 == cellsEqual(t, pos, c)) { 
		printf("call bubbleUp again\n");
		bubbleUp(t, pos);
		bubbleUp(t, c); }
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
		cell topLeftCell = cellAt(0,0);
		bubbleDown(t, topLeftCell);
	}

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

}

void printTableau(tableau *t) {
	for (int i=0; i < t->m*t->n; i++) {
		if (i % t->m == 0) {
			printf("\n"); // line break every mth item
		}
	printf("%d ", t->elements[i]);
	}
	printf("\n");
}

int main() {
	// make mxn tableau

	tableau *t = getNewTableau(3,3);
	insert(t, 1);
	printTableau(t);
	insert(t, 1);
	printTableau(t);
	insert(t, 2);
	insert(t, 5);
	insert(t, 15);
	insert(t, 20);
	insert(t, 2);
	printTableau(t);

	return 0;
}



