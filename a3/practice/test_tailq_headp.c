#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h> 

typedef struct q_item {
        int value;
        TAILQ_ENTRY(q_item) entries;
} q_item;

void enqueue(int n, struct headname *headp) {
	// enqueue the node with value n
	q_item *item;
	item = malloc(sizeof(q_item));
	item->value = n;
	printf("queued %d\n", item->value);
	TAILQ_INSERT_TAIL(headp, item, entries);
}

void dequeue(struct headname *headp) {
	q_item *returned_item;
	returned_item = TAILQ_FIRST(headp);
	printf("dequeued %d\n", returned_item->value);
	TAILQ_REMOVE(headp, returned_item, entries);
	free(returned_item);
}

int main() {

	TAILQ_HEAD(headname, q_item) head;
	struct headname *headp;
	TAILQ_INIT(headp);

	enqueue(1, headp);
	enqueue(2, headp);
	enqueue(3, headp);

	dequeue(headp);

	return 0;
}
