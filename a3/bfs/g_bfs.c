#include <stdio.h>
#include <stdlib.h>
#include <glib.h> // for the hash table, O(1) avg lookup

#define MAXADJ 10

typedef struct vertex {
	int value;
	int *neighbours; // array of adjacent ints
	int list_len;
} vertex;

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

void printAdjList(Graph * graph) {
	// print this graph's adjacency table.
	g_hash_table_foreach(graph->adj_list, printListRow, NULL);
}

int main() {
	Graph *graph = newGraph();

	addVertex(graph, 1);
	addVertex(graph, 2);
	addVertex(graph, 3);
	addVertex(graph, 4);
	addVertex(graph, 5);

	addEdge(graph, 1, 2, 1);
	addEdge(graph, 1, 3, 1);
	addEdge(graph, 1, 4, 1);
	addEdge(graph, 4, 5, 1);
	addEdge(graph, 6, 5, 1);
	addEdge(graph, 12, 13, 1);

	printAdjList(graph);
	

	return 0;
}
