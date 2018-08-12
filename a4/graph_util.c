#include <stdio.h>
#include <stdlib.h>
#include <glib.h> // for the hash table, O(1) avg lookup
#include <math.h>

#include "graph_util.h"

#define MAXV 1000 // for malloc() purposes
#define MAXADJ 10 // for malloc() purposes
#define POS_INF 2147483647

int dfs_time; // global var; see if you can make scope smaller later

// typedef enum vertex_color 
//         { 
//           white, 
//           gray,
//           black 
//         } vertex_color; 

// typedef struct wEdge wEdge;
// // weighted directed edge

// struct wEdge {
// 	int to;
// 	int from;
// 	int weight;
// };

// typedef struct vertex vertex;

// struct vertex {
// 	int value;
// 	wEdge **neighbours; // array of pointers to wEdge
// 	int list_len;
	
// 	vertex_color vertex_color;
// 	vertex *pi;
// };

// typedef struct Graph Graph;

// struct Graph {
// 	GHashTable *adj_list;
// 	int *sortedVertices; // alpha sorting for textbook example
// 	int vertexCount;
// 	wEdge **edgeArray;
// 	int edgeIndex;

// };

int sortedInsert(int *array, int arr_len, int v) {
	// insert v into sorted array
	// running time O(n) where n == arr_len, so either O(|V|) or O(|E|) depending
	// on what we run on. therefore still linear
	if (arr_len == 0) { // empty: just insert
		array[0] = v;
	} else {
		int *pointer = array + (arr_len - 1); // start at last item
		while (v < *pointer) {
			// move everything over 1
			*(pointer+1) = *pointer;
			pointer--;
		}
		pointer++;
		*pointer = v;
	}
	
	// return updated arr_len
	return arr_len + 1;
}

int sortedInsertwEdge(wEdge **array, int arr_len, int from, int v, int weight, Graph *graph) {
	// insert v into sorted array
	// running time O(n) where n == arr_len, so either O(|V|) or O(|E|) depending
	// on what we run on. therefore still linear
	if (arr_len == 0) { // empty: just insert
		array[0] = malloc(sizeof(wEdge));
		array[0]->to = v;
		array[0]->from = from;
		array[0]->weight = weight;
	} else {
		int i = arr_len - 1;
		array[i+1] = malloc(sizeof(wEdge));
		
		// printf("pointer at array start: %p\n", array);
		// printf("pointer at last item: %p\n", array[i]);
		
		// start at last item
		while (i >= 0 && v < (array[i])->to) {
			// move everything over 1
			(array[i+1])->to = (array[i])->to;
			(array[i+1])->weight = (array[i])->weight;
			i--;
		}
		i++;
		(array[i])->to = v;
		(array[i])->weight = weight;

	}

	// return updated arr_len
	return arr_len + 1;
}

// helper to make a new graph. Returns pointer to the graph
Graph * newGraph() {
	Graph *newGraph = malloc(sizeof(Graph));
	// adjacency list
	newGraph->adj_list = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_free);
	newGraph->sortedVertices = malloc(MAXV * sizeof(int));
	newGraph->vertexCount = 0;
	newGraph->edgeIndex = 0;

	return newGraph;
}

vertex * getVertex(Graph *graph, int n) {
	// given int n, gets the pointer to the vertex with that value
	vertex *v_pointer = g_hash_table_lookup(graph->adj_list, GINT_TO_POINTER(n));
	return v_pointer;
}

vertex * addVertex(Graph *graph, int value) { // called exactly |V| times
	// make a vertex and add it to the Graph
	vertex *v = g_malloc(sizeof(vertex));
	v->value = value;
	v->neighbours = malloc(MAXADJ * sizeof(wEdge*));
	v->list_len = 0;

	g_hash_table_insert(graph->adj_list, GINT_TO_POINTER(v->value), v);

	sortedInsert(graph->sortedVertices, graph->vertexCount, value); // O(|V|)
	graph->vertexCount += 1;
	printf("Inserted %d\n", v->value);

	// return pointer to vertex
	return v;
}

void addNeighbour(Graph *graph, vertex *vertex, int neighbour, int weight) { // O(2|E|)
	// check that neighbour isn't already in list
	for (int i = 0; i < vertex->list_len; i++) {
		if (vertex->neighbours[i]->to == neighbour) {
			fprintf(stderr, "%d already has %d as neighbour\n", vertex->value, neighbour);
			return;
		}
	}
	
	// now we do in alpha order!
	// printf("Inserting %d into adj list of vertex %d\n", neighbour, vertex->value);
	sortedInsertwEdge(vertex->neighbours, vertex->list_len, vertex->value, neighbour, weight, graph); 
	// O(|E'|) of any individual vertex
	
	// vertex->neighbours[vertex->list_len] = neighbour;
	vertex->list_len += 1;
}

void addEdge(Graph *graph, int from_vertex, int to_vertex, int weight, int undirected) {
	// Add edge. If undirected != 0, adds an edge back in the other direction too.
	// Amends the neighbours array of the relevant vertex/vertices.

	// check that both vertices exist
	vertex *from_exists = g_hash_table_lookup(graph->adj_list, GINT_TO_POINTER(from_vertex));
	vertex *to_exists = g_hash_table_lookup(graph->adj_list, GINT_TO_POINTER(to_vertex));

	if (from_exists && to_exists) {
		// search for from_vertex
		addNeighbour(graph, from_exists, to_vertex, weight);

		// also update Graph's edge array
		int index = graph->edgeIndex;
		graph->edgeArray[index] = malloc(sizeof(wEdge));

		(graph->edgeArray[index])->to = to_vertex;
		graph->edgeArray[index]->from = from_vertex;
		graph->edgeArray[index]->weight = weight;
		graph->edgeIndex++;

		if (undirected != 0) {
			addNeighbour(graph, to_exists, from_vertex, weight);
			printf("%d<->%d, weight=%d\n", from_vertex, to_vertex, weight);
		} else {
			printf("%d<->%d, weight=%d\n", from_vertex, to_vertex, weight);
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
    	printf("(%d ", deref->neighbours[i]->to);
    	printf("w=%d) ", deref->neighbours[i]->to);
    }
    printf("\n");
}

// alpha behaviour
void printAdjList(Graph * graph) {
	printf("\n------- ADJ LIST ----------\n");
	// print this graph's adjacency table.
	for (int i = 0; i < graph->vertexCount; i++) {
		int value = graph->sortedVertices[i];
		printf("key: %c | neighbours: ", getLetter(value));
		vertex *v;
		v = getVertex(graph, value);
		for (int j = 0; j < v->list_len; j++) {
			printf("(%c ", getLetter(v->neighbours[j]->to));
			printf("w=%d) ", v->neighbours[j]->weight);
		}
		printf("\n");
	}
}

char getLetter(int n) { // dumb but it works
	// simple conversion tool for number to letter.
	char c;
	switch (n) {
		case 1:
			c = 'a';
			break;
		case 2:
			c = 'b';
			break;
		case 3:
			c = 'c';
			break;
		case 4:
			c = 'd';
			break;
		case 5:
			c = 'e';
			break;
		case 6:
			c = 'f';
			break;
		case 7:
			c = 'g';
			break;
		case 8:
			c = 'h';
			break;
		case 9:
			c = 'i';
			break;
	}

	return c;
}

int compareWeights(const void *p, const void *q)
{

	const wEdge **e1 = (const wEdge **) p;
	const wEdge **e2 = (const wEdge **) q;

	return ((*e1)->weight - (*e2)->weight);
}

void sortEdgeArray(wEdge **edgeArray, int edgeCount) {
	// take graph's edgeArray and return a sorted copy
	wEdge ** sorted = malloc(sizeof(wEdge*) * edgeCount);
	for (int i = 0; i < edgeCount; i++) {
		sorted[i] = malloc(sizeof(wEdge));
	}

	qsort(edgeArray, edgeCount, sizeof(wEdge*), &compareWeights);
}



