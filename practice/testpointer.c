#include <stdio.h>
#include <stdlib.h>

int stack[1000];

int *sp;

#define push(sp, n) (*((sp)++) = (n)) // push to current spot which is empty, then increment to point to an empty spot
#define pop(sp) (*sp--)// backtrack one spot, return that value, point at that place so it'll be replaced by the next push 

int main() {
	int *i = malloc(3 * sizeof(int));

	*i = 1;
	printf("i is %p | *i is: %d\n", i, *i);
	*(i+1) = 2; // increments i, then returns the value at the pointer

	printf("i is %p | *i is: %d\n", i, *i);
	printf("i is %p | *i++ is: %d\n", i, *i++);
	printf("i is %p | *i is: %d\n", i, *i);
	// run pop

	printf("i is %p\n", i);
	printf("i is %p | *i-- is: %d\n", i, *i--);
	printf("i is %p\n", i); 

	// ---- test push pop

	sp = stack;

	printf("sp: %p\n", sp);
	push(sp, 10);
	printf("pushd 10\n");
	printf("sp: %p\n", sp);
	int x = pop(sp);
	printf("popped x\n");
	printf("sp: %p\n", sp);
	printf("x: %d\n", x);

}