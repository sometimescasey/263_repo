/* 
Summer 2018
CSC263 Assignment 3 Question 7

We consider undirected graphs (with no weights). 
Often there are multiple shortest paths between two nodes of a graph.

Describe a linear-time algorithm such that, given an undirected, unweighted graph 
and two vertices u and v, the algorithm counts the number of distinct shortest paths 
from u to v. Justify its correctness and the running time.
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glib.h> // hash table
#include <sys/queue.h> // queue

#define MAXADJ 10 // max neighbours, for malloc() purposes
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
	int sp_count; // count of shortest paths
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
	if (!v_pointer) {
		fprintf(stderr, "getVertex(%d) failed: node does not exist\n", n);
	}
	return v_pointer;
}

vertex * addVertex(Graph *graph, int value) {
	// make a vertex and add it to the Graph
	vertex *v = g_malloc(sizeof(vertex));
	v->value = value;
	v->neighbours = malloc(MAXADJ * sizeof(int));
	v->list_len = 0;

	g_hash_table_insert(graph->adj_list, GINT_TO_POINTER(v->value), v);

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

void printAdjList(Graph * graph) {
	// print this graph's adjacency table.
	g_hash_table_foreach(graph->adj_list, printListRow, NULL);
}

void initialize_bfs(gpointer key, gpointer value, gpointer userdata) {
	vertex *deref = ((vertex*) value);
	deref->vertex_color = white;
	deref->d = POS_INF;
	deref->pi = NULL;
	deref->sp_count = -POS_INF;
}

void enqueue(Graph *graph, vertex* v) {
	TAILQ_INSERT_TAIL(&head, v, entries);
	// printf("queued vertex %d\n", v->value);
}

vertex* dequeue(Graph *graph) {
	vertex *returned_item;
	returned_item = TAILQ_FIRST(&head);
	TAILQ_REMOVE(&head, returned_item, entries);
	// printf("dequeued %d\n", returned_item->value);
	return returned_item;
}

int bfs(Graph *graph, int start) {
	g_hash_table_foreach(graph->adj_list, initialize_bfs, NULL);

	vertex *s = g_hash_table_lookup(graph->adj_list, GINT_TO_POINTER(start));
	s->vertex_color = gray;
	s->d = 0;
	s->pi = NULL;

	enqueue(graph, s);
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
				enqueue(graph, v);
			}
		}
		u->vertex_color = black;
	}
	return 0;
}

int bfs_countback(Graph *graph, vertex *t, vertex *initial) {
	printf("COUNTBACK CALLED ON T = %d\n", t->value);
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
			// ensure we don't waste time calling bfs_countback() on any node twice
			if (v->sp_count == -POS_INF) {
				v->sp_count = bfs_countback(graph, v, initial);	
			}
			path_num += v->sp_count;
		}
	}

	return path_num;
}

void path_count(Graph *graph, vertex *u, vertex *v) {
	int bfs_result = bfs(graph, u->value);
	if (bfs_result == 0) {
		int count = bfs_countback(graph, v, u);
		printf("# shortest paths from %d to %d: %d\n", u->value, v->value, count);
	}
}

int main(int argc, char ** argv) {

	Graph *graph = newGraph();

// TEST CASE 1: 2 paths ----------------------
	// addVertex(graph, 1);
	// addVertex(graph, 2);
	// addVertex(graph, 3);
	// addVertex(graph, 4);
	// addVertex(graph, 5);

	// addEdge(graph, 1, 2, 1);
	// addEdge(graph, 1, 3, 1);
	// addEdge(graph, 2, 4, 1);
	// addEdge(graph, 3, 4, 1);
	// addEdge(graph, 4, 5, 1);

	// vertex *v = getVertex(graph, 5);
	// vertex *u = getVertex(graph, 1);

// TEST CASE 2: 8 paths ---------------------
	// addVertex(graph, 1);
	// addVertex(graph, 2);
	// addVertex(graph, 3);
	// addVertex(graph, 4);
	// addVertex(graph, 5);
	// addVertex(graph, 6);
	// addVertex(graph, 7);
	// addVertex(graph, 8);

	// addEdge(graph, 1, 2, 1);
	// addEdge(graph, 1, 3, 1);

	// addEdge(graph, 2, 4, 1);
	// addEdge(graph, 2, 5, 1);

	// addEdge(graph, 3, 4, 1);
	// addEdge(graph, 3, 5, 1);

	// addEdge(graph, 4, 6, 1);
	// addEdge(graph, 4, 7, 1);

	// addEdge(graph, 5, 6, 1);
	// addEdge(graph, 5, 7, 1);

	// addEdge(graph, 6, 8, 1);
	// addEdge(graph, 7, 8, 1);

	// vertex *v = getVertex(graph, 8);
	// vertex *u = getVertex(graph, 1);

// ------------------------------------------

// TEST CASE 3: 4 paths ---------------------

	// addVertex(graph, 1);
	// addVertex(graph, 2);
	// addVertex(graph, 3);
	// addVertex(graph, 4);
	// addVertex(graph, 5);
	// addVertex(graph, 6);

	// addEdge(graph, 1, 2, 1);
	// addEdge(graph, 1, 3, 1);
	// addEdge(graph, 1, 4, 1);
	// addEdge(graph, 1, 5, 1);

	// addEdge(graph, 2, 6, 1);
	// addEdge(graph, 3, 6, 1);
	// addEdge(graph, 4, 6, 1);
	// addEdge(graph, 5, 6, 1);

	// vertex *v = getVertex(graph, 6);
	// vertex *u = getVertex(graph, 1);

// ------------------------------------------

	TAILQ_INIT(&head);
	path_count(graph, u, v);

	return 0;
}
