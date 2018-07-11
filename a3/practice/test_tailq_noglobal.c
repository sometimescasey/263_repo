#include <stdio.h>
#include <stdlib.h>
#include <sys/queue.h> 

typedef struct q_item {
        int value;
        TAILQ_ENTRY(q_item) entries;
} q_item;

void enqueue(int n, TAILQ_HEAD(, q_item) * head) {
	// enqueue the node with value n
	q_item *item;
	item = malloc(sizeof(q_item));
	item->value = n;
	printf("queued %d\n", item->value);
	TAILQ_INSERT_TAIL(head, item, entries);
}

void dequeue(TAILQ_HEAD(, q_item) * head) {
	q_item *returned_item;
	returned_item = TAILQ_FIRST(head);
	printf("dequeued %d\n", returned_item->value);
	TAILQ_REMOVE(head, returned_item, entries);
	free(returned_item);
}

int main() {

	TAILQ_HEAD(, q_item) head;
	TAILQ_INIT(&head);

	enqueue(1, &head);
	enqueue(2, &head);
	enqueue(3, &head);

	dequeue(&head);

	return 0;
}
