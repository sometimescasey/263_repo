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

vertex * newVertex(int value) {
	// initialize a new vertex with empty neighbours array
	// TODO: check vertex doesn't already exist
	vertex *v = g_malloc(sizeof(vertex));
	v->value = value;
	v->neighbours = malloc(MAXADJ * sizeof(int));
	v->list_len = 0;

	printf("Created vertex with value %d\n", v->value);
	return v;
}

int addVertex(Graph *graph, vertex *v) {
	// add vertex to graph
	// WARNING: doesn't check for dupes. Will simply overwrite
	// TODO: dupe check
	g_hash_table_insert(graph->adj_list, GINT_TO_POINTER(v->value), v);
	printf("Inserted vertex w value %d\n", v->value);
	// returns 0 if successful
	return 0;
}

int addEdge(Graph *graph, int from_vertex, int to_vertex) {
	// amends the neighbours array of the relevant vertex

	// TODO check: should fail if one or both vertices do not exist in the graph

	// search for from_vertex
	vertex *found_vertex = g_hash_table_lookup(graph->adj_list, GINT_TO_POINTER(from_vertex));

	found_vertex->neighbours[found_vertex->list_len] = to_vertex;
	found_vertex->list_len += 1;

	printf("Successfully added edge between %d and %d\n", from_vertex, to_vertex);
	// returns 0 if edge was successfully added
	return 0;
}

void printListRow(gpointer key, gpointer value, gpointer userdata)
{
    vertex *deref = ((vertex*) value);
    printf("key: %d | neighbours: ", GPOINTER_TO_INT(key));
    for (int i = 0; i < deref->list_len; i++) {
    	printf("%d, ", deref->neighbours[i]);
    }
    printf("\n");
}

void printAdjList(Graph * graph) {
	// print this graph's adjacency table.
	g_hash_table_foreach(graph->adj_list, printListRow, NULL);
}

int main() {
	Graph *graph = newGraph();

	vertex *v1 = newVertex(1);
	vertex *v2 = newVertex(2);
	vertex *v3 = newVertex(3);
	vertex *v4 = newVertex(4);
	vertex *v5 = newVertex(5);

	addVertex(graph, v1);
	addVertex(graph, v2);
	addVertex(graph, v3);
	addVertex(graph, v4);
	addVertex(graph, v5);

	addEdge(graph, 1, 2);
	addEdge(graph, 1, 3);
	addEdge(graph, 1, 4);
	addEdge(graph, 4, 5);

	printAdjList(graph);
	

	return 0;
}
