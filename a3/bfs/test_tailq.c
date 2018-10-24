#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h> 

TAILQ_HEAD(, q_item) head;

typedef struct q_item {
        int value;
        TAILQ_ENTRY(q_item) entries;
} q_item;

void enqueue(int n) {
	// enqueue the node with value n
	q_item *item;
	item = malloc(sizeof(q_item));
	item->value = n;
	printf("queued %d\n", item->value);
	TAILQ_INSERT_TAIL(&head, item, entries);
}

void dequeue() {
	q_item *returned_item;
	returned_item = TAILQ_FIRST(&head);
	printf("dequeued %d\n", returned_item->value);
	TAILQ_REMOVE(&head, returned_item, entries);
	free(returned_item);
}

int main() {

	TAILQ_INIT(&head);

	enqueue(1);
	enqueue(2);
	enqueue(3);

	dequeue();

	return 0;
}
