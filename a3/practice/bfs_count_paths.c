#include <stdio.h>
#include <stdlib.h>
#include <glib.h> // for the hash table, O(1) avg lookup
#include <sys/queue.h> // for the BFS queue
#include <math.h>

#define MAXADJ 10
#define POS_INF 2147483647

TAILQ_HEAD(, vertex) head; // make queue for BFS

typedef enum vertex_color 
        { 
          white, 
          gray,
          black 
        } vertex_color; 

typedef struct vertex vertex;

struct vertex {
	int value;
	int *neighbours; // array of adjacent ints
	int list_len;
	
	vertex_color vertex_color;
	int d;
	vertex *pi;
	TAILQ_ENTRY(vertex) entries;
};

typedef struct Graph {
	GHashTable *adj_list;
} Graph;

// helper to make a new graph. Returns pointer to the graph
Graph * newGraph() {
	Graph *newGraph = malloc(sizeof(Graph));
	// adjacency list
	newGraph->adj_list = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_free);
	return newGraph;
}

vertex * getVertex(Graph *graph, int n) {
	// given int n, gets the pointer to the vertex with that value
	vertex *v_pointer = g_hash_table_lookup(graph->adj_list, GINT_TO_POINTER(n));
	return v_pointer;
}

vertex * addVertex(Graph *graph, int value) {
	// make a vertex and add it to the Graph
	vertex *v = g_malloc(sizeof(vertex));
	v->value = value;
	v->neighbours = malloc(MAXADJ * sizeof(int));
	v->list_len = 0;

	g_hash_table_insert(graph->adj_list, GINT_TO_POINTER(v->value), v);
	printf("Inserted %d\n", v->value);

	// return pointer to vertex
	return v;
}

void addNeighbour(Graph *graph, vertex *vertex, int neighbour) {
	// check that neighbour isn't already in list
	for (int i = 0; i < vertex->list_len; i++) {
		if (vertex->neighbours[i] == neighbour) {
			fprintf(stderr, "%d already has %d as neighbour\n", vertex->value, neighbour);
			return;
		}
	}
	vertex->neighbours[vertex->list_len] = neighbour;
	vertex->list_len += 1;
}

void addEdge(Graph *graph, int from_vertex, int to_vertex, int directed) {
	// Add edge. If directed != 0, adds an edge back in the other direction too.
	// Amends the neighbours array of the relevant vertex/vertices.

	// check that both vertices exist
	vertex *from_exists = g_hash_table_lookup(graph->adj_list, GINT_TO_POINTER(from_vertex));
	vertex *to_exists = g_hash_table_lookup(graph->adj_list, GINT_TO_POINTER(to_vertex));

	if (from_exists && to_exists) {
		// search for from_vertex
		addNeighbour(graph, from_exists, to_vertex);

		if (directed != 0) {
			addNeighbour(graph, to_exists, from_vertex);
			printf("%d<->%d\n", from_vertex, to_vertex);
		} else {
			printf("%d-->%d\n", from_vertex, to_vertex);
		}
	} else if (!from_exists && !to_exists) {
		fprintf(stderr, "%d, %d do not exist\n", from_vertex, to_vertex);
		return;
	} else if (!from_exists) {
		fprintf(stderr, "%d does not exist\n", from_vertex);
		return;
	} else {
		fprintf(stderr, "%d does not exist\n", to_vertex);
		return;
	}
}

void printListRow(gpointer key, gpointer value, gpointer userdata)
{
    vertex *deref = ((vertex*) value);
    printf("key: %d | neighbours: ", GPOINTER_TO_INT(key));
    for (int i = 0; i < deref->list_len; i++) {
    	printf("%d ", deref->neighbours[i]);
    }
    printf("\n");
}

void initialize_bfs(gpointer key, gpointer value, gpointer userdata) {
	vertex *deref = ((vertex*) value);
	deref->vertex_color = white;
	deref->d = POS_INF;
	deref->pi = NULL;
}

void reset_colors(gpointer key, gpointer value, gpointer userdata) {
	vertex *deref = ((vertex*) value);
	deref->vertex_color = white;
}

void printAdjList(Graph * graph) {
	// print this graph's adjacency table.
	g_hash_table_foreach(graph->adj_list, printListRow, NULL);
}
void enqueue_v(Graph *graph, vertex* v) {
	printf("queued vertex %d\n", v->value);
	TAILQ_INSERT_TAIL(&head, v, entries);
}

// TODO: check if this is ever used
void enqueue(Graph *graph, int n) {
	// enqueue the node with value n
	vertex *item;
	item = g_hash_table_lookup(graph->adj_list, GINT_TO_POINTER(n));
	printf("queued %d\n", item->value);
	TAILQ_INSERT_TAIL(&head, item, entries);
}

vertex* dequeue(Graph *graph) {
	vertex *returned_item;
	returned_item = TAILQ_FIRST(&head);
	printf("dequeued %d\n", returned_item->value);
	TAILQ_REMOVE(&head, returned_item, entries);
	return returned_item;
}

int bfs(Graph *graph, int start) {
	g_hash_table_foreach(graph->adj_list, initialize_bfs, NULL);

	vertex *s = g_hash_table_lookup(graph->adj_list, GINT_TO_POINTER(start));
	s->vertex_color = gray;
	s->d = 0;
	s->pi = NULL;

	enqueue_v(graph, s);
	vertex *u;
	while (!TAILQ_EMPTY(&head)) {
		u = dequeue(graph);
		int nbr;
		vertex *v;
		for (int i = 0; i < u->list_len; i++) {
			nbr = u->neighbours[i];
			v = getVertex(graph, nbr);
			if (v->vertex_color == white) {
				v->vertex_color = gray;
				v->d = u->d + 1;
				v->pi = u;
				enqueue_v(graph, v);
			}
		}
		u->vertex_color = black;
	}

	return 0;
}

int bfs_countback(Graph *graph, vertex *t, vertex *initial) {
	int path_num = 0;
	// given a graph with BFS distances filled in
	int target = initial->value;
	int distance = t->d;

	for (int i = 0; i < t->list_len; i++) {
		int nbr = t->neighbours[i];
		vertex *v = getVertex(graph, nbr); 
		if (v->value == target) {
			return 1; // one of the neighbours is the target value; it's one edge away
		}
		if (v->d == (distance - 1)) {
			path_num += bfs_countback(graph, v, initial); // call bfs_countback on that one
		}
	}

	return path_num;
}

// int bfs_countback(Graph *graph, vertex *t, vertex *s) {

// 	// TODO: s never gets used

// 	int path_counter = 0; // if bfs_countback is being called, then there is at least one known path
	
// 	// given a graph that has bfs() run on it, and had populated d values
// 	// walk from node t back to node s to count # of shortest paths
// 	g_hash_table_foreach(graph->adj_list, reset_colors, NULL);
// 	// make all nodes white again but keep d values

// 	// start at t
	
// 	t->vertex_color = gray;
// 	enqueue_v(graph, t);
// 	vertex *u;
// 	int parent_d;
// 	while (!TAILQ_EMPTY(&head)) {
// 		u = dequeue(graph);
// 		parent_d = u->d;
// 		int nbr;
// 		vertex *v;

// 		for (int i = 0; i < u->list_len; i++) {
// 			nbr = u->neighbours[i];
// 			v = getVertex(graph, nbr);
// 			if (v->d == (parent_d - 1)) {
// 				path_counter += 1;
// 			}

// 			if (v->vertex_color == white) {
// 				v->vertex_color = gray;
// 				enqueue_v(graph, v);
// 			}
// 		}
// 		u->vertex_color = black;
// 		printf("counter is: %d\n\n", path_counter);
// 	}

// 	return path_counter;
// }

void printVertexDistance(gpointer key, gpointer value, gpointer userdata)
{
    vertex *deref = ((vertex*) value);
    printf("key: %d | distance: %d\n", GPOINTER_TO_INT(key), deref->d);
}

void printDistances(Graph *graph) {
	g_hash_table_foreach(graph->adj_list, printVertexDistance, NULL);
}



int main() {
	Graph *graph = newGraph();

	// addVertex(graph, 1);
	// addVertex(graph, 2);
	// addVertex(graph, 3);
	// addVertex(graph, 4);
	// addVertex(graph, 5);

	// addVertex(graph, 10);	
	// addVertex(graph, 11);

	// // addVertex(graph, 30);

	// // addVertex(graph, 20);
	// // addVertex(graph, 21);

	// // addEdge(graph, 1, 2, 1);
	// // addEdge(graph, 1, 3, 1);
	// addEdge(graph, 2, 4, 1);
	// addEdge(graph, 3, 4, 1);
	// addEdge(graph, 4, 5, 1);

	// // addEdge(graph, 1, 10, 1);
	// addEdge(graph, 10, 11, 1);
	// addEdge(graph, 11, 5, 1);

	// // addEdge(graph, 1, 30, 1);
	// // addEdge(graph, 30, 4, 1);

	// // addEdge(graph, 1, 20, 1);
	// // addEdge(graph, 20, 21, 1);
	// // addEdge(graph, 21, 5, 1);

	
// CASE: 8 paths ---------------------
	addVertex(graph, 1);
	addVertex(graph, 2);
	addVertex(graph, 3);
	addVertex(graph, 4);
	addVertex(graph, 5);
	addVertex(graph, 6);
	addVertex(graph, 7);
	addVertex(graph, 8);

	addEdge(graph, 1, 2, 1);
	addEdge(graph, 1, 3, 1);

	addEdge(graph, 2, 4, 1);
	addEdge(graph, 2, 5, 1);

	addEdge(graph, 3, 4, 1);
	addEdge(graph, 3, 5, 1);

	addEdge(graph, 4, 6, 1);
	addEdge(graph, 4, 7, 1);

	addEdge(graph, 5, 6, 1);
	addEdge(graph, 5, 7, 1);

	addEdge(graph, 6, 8, 1);
	addEdge(graph, 7, 8, 1);



	// printAdjList(graph);

	TAILQ_INIT(&head);

	int bfs_result = bfs(graph, 1);
	printf("bfs_result: %d\n", bfs_result);

	printDistances(graph);

	vertex *a = getVertex(graph, 8);
	vertex *b = getVertex(graph, 1);

	int count = bfs_countback(graph, a, b);
	printf("# shortest paths from %d to %d: %d\n", a->value, b->value, count);

	return 0;
}
