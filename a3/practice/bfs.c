#include <stdio.h>
#include <stdlib.h>
#include <glib.h> // for the hash table, O(1) avg lookup

#define MAXVERTICES 100

typedef struct Vertex {
	int value;
} Vertex;

// undirected edges
typedef struct Edge {
	Vertex *v1;
	Vertex *v2;
} Edge;

typedef struct Graph {
	Vertex *vertices;
	Edge *edges;
	GHashTable *adj_list;
} Graph;

// helper to make a new graph. Returns pointer to the graph
Graph * newGraph() {
	int max_edges = MAXVERTICES * (MAXVERTICES - 1);

	Graph *newGraph = malloc(sizeof(Graph));

	newGraph->vertices = malloc(MAXVERTICES * sizeof(Vertex));
		
	// fixed size array of edges, v * (v-1) (no loops, no edges to self)
	newGraph->edges = malloc(max_edges * sizeof(Edge));

		// adjacency list
	newGraph->adj_list = g_hash_table_new(NULL, NULL); // use default functions

	return newGraph;
}

void addVertex(Graph* graph, Vertex v) {
	;
}

Vertex* newVertex(int value) {
	Vertex *new_v = malloc(sizeof(Vertex));
	new_v->value = value;
	return new_v;
}

Edge* newEdge(Vertex *v1, Vertex *v2) {
	Edge *new_e = malloc(sizeof(Edge)); 
	new_e->v1 = v1;
	new_e->v2 = v2;
	return new_e;
}

void printEdge(Edge * e) {
	printf("(%d, %d)\n", e->v1->value, e->v2->value);
}

int main() {
	Vertex* v1 = newVertex(1);
	Vertex* v2 = newVertex(2);

	Edge* edge = newEdge(v1, v2);

	printEdge(edge);

	return 0;
}
