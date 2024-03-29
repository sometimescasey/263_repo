/* 
Summer 2018
CSC263 Assignment 4 Question 2
CLRS 22-4

Let G = (V,E) be a directed graph in which each vertex u in V is labeled with
a unique integer L(u) from the set {1, 2, ...|V|}. For each vertex u in V , let
R(u) = {v in V: u has a path to v} be the set of vertices that are reachable from u. 

Define min(u) to be the vertex in R(u) whose label is minimum, i.e., min(u) is the vertex v such that L(v) = min{L(w): w in R(u)}. Give an O(V+E) algorithm that
computes min(u) for all vertices u in V.

Compilation:
	gcc -Wall -std=gnu99 -o reachable reachable.c -lm `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0`
*/

#include <stdio.h>
#include <stdlib.h>
#include <glib.h> // for the hash table, O(1) avg lookup
#include <math.h>

#define MAXV 1000 // for malloc() purposes
#define MAXADJ 10 // for malloc() purposes
#define POS_INF 2147483647

int dfs_time;

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
	vertex *pi;
	
	int t_d; // discovery time
	int t_f; // finish time

	int minlabel;
};

int sortedInsert(int *array, int arr_len, int v) {
	// insert v into sorted array

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

typedef struct Graph {
	GHashTable *adj_list;
	int *sortedVertices; // alpha sorting for textbook example
	int vertexCount;

	int *topoSort;
	int topoCount;

	int **minlabels;

} Graph;

// helper to make a new graph. Returns pointer to the graph
Graph * newGraph() {
	Graph *newGraph = malloc(sizeof(Graph));
	// adjacency list
	newGraph->adj_list = g_hash_table_new_full(g_direct_hash, g_direct_equal, NULL, g_free);
	newGraph->sortedVertices = malloc(MAXV * sizeof(int));
	newGraph->vertexCount = 0;

	newGraph->topoSort = malloc(MAXV * sizeof(int));
	newGraph->topoCount = 0;

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
	v->neighbours = malloc(MAXADJ * sizeof(int));
	v->list_len = 0;

	g_hash_table_insert(graph->adj_list, GINT_TO_POINTER(v->value), v);

	sortedInsert(graph->sortedVertices, graph->vertexCount, value); // O(|V|)
	graph->vertexCount += 1;

	// return pointer to vertex
	return v;
}

void addNeighbour(Graph *graph, vertex *vertex, int neighbour) { // O(2|E|)
	// check that neighbour isn't already in list
	for (int i = 0; i < vertex->list_len; i++) {
		if (vertex->neighbours[i] == neighbour) {
			fprintf(stderr, "%d already has %d as neighbour\n", vertex->value, neighbour);
			return;
		}
	}
	
	// now we do in alpha order!
	sortedInsert(vertex->neighbours, vertex->list_len, neighbour); 
	// O(|E'|) of any individual vertex
	
	// vertex->neighbours[vertex->list_len] = neighbour;
	vertex->list_len += 1;
}

void addEdge(Graph *graph, int from_vertex, int to_vertex, int undirected) {
	// Add edge. If undirected != 0, adds an edge back in the other direction too.
	// Amends the neighbours array of the relevant vertex/vertices.

	// check that both vertices exist
	vertex *from_exists = g_hash_table_lookup(graph->adj_list, GINT_TO_POINTER(from_vertex));
	vertex *to_exists = g_hash_table_lookup(graph->adj_list, GINT_TO_POINTER(to_vertex));

	if (from_exists && to_exists) {
		// search for from_vertex
		addNeighbour(graph, from_exists, to_vertex);

		if (undirected != 0) {
			addNeighbour(graph, to_exists, from_vertex);
			// printf("%d<->%d\n", from_vertex, to_vertex);
		} else {
			// printf("%d-->%d\n", from_vertex, to_vertex);
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

// alpha behaviour
void printAdjList(Graph * graph) {
	// print this graph's adjacency table.
	for (int i = 0; i < graph->vertexCount; i++) {
		int value = graph->sortedVertices[i];
		printf("vertex: %d | neighbours: ", value);
		vertex *v;
		v = getVertex(graph, value);
		for (int j = 0; j < v->list_len; j++) {
			printf("%d ", v->neighbours[j]);
		}
		printf("\n");
	}
}

void initialize_dfs(gpointer key, gpointer value, gpointer userdata) {
	vertex *deref = ((vertex*) value);
	deref->vertex_color = white;
	deref->t_d = 0;
	deref->t_f = 0;
	deref->pi = NULL;
}

void dfs_visit(Graph *graph, vertex *deref) {
	dfs_time += 1;
	deref->t_d = dfs_time; // set discovery time
	deref->vertex_color = gray;
	int nbr;
	vertex *v;
	for (int i = 0; i < deref->list_len; i++) { 
	// iterate thru all neighbours, which will be in alpha order
		nbr = deref->neighbours[i];
		v = getVertex(graph, nbr);
		if (v->vertex_color == gray) {
			// printf("Found a back edge: %d, %d\n", deref->value, v->value);
		}
		if (v->vertex_color == white) {
			v->pi = deref;
			dfs_visit(graph, v);
		}
	}
	deref->vertex_color = black;
	graph->topoSort[graph->topoCount] = deref->value;
	graph->topoCount++;
	// printf("finished %d\n", deref->value);
	dfs_time += 1;
	deref->t_f = dfs_time;
}

void onEachVertex(gpointer key, gpointer value, gpointer userdata) {
	Graph *graph = ((Graph*) userdata);
	vertex *deref = ((vertex*) value);
	if (deref->vertex_color == white) {
		dfs_visit(graph, deref);
	}
}

void alpha_dfs(Graph * graph) {
	// alphabetical dfs
	g_hash_table_foreach(graph->adj_list, initialize_dfs, NULL);
	// set all vertices to white and t_d, t_f values to 0
	dfs_time = 0;
	vertex *current;
	for (int i = 0; i < graph->vertexCount; i++) {
		current = getVertex(graph, graph->sortedVertices[i]);
		// printf("starting on vertex %d\n", current->value);
		if (current->vertex_color == white) {
			dfs_visit(graph, current);
		}
	}
}

int* topoPrint(Graph *graph) {
	// note that this prints backwards
	// socks must come BEFORE everything else
	// so they FINISH last
	printf("Topological sort: ");
	for (int i = graph->topoCount - 1; i >= 0; i--) {
		printf("%d ", graph->topoSort[i]);
	}
	printf("\n");

	return graph->topoSort;
}

int min(int x, int y) {
	if (x <= y) { return x; }
	else return y;
}

void scc_dfs(Graph *graph_transpose, vertex *u, Graph *original) {
	// recursive dfs run to identify strongly connected components
	u->vertex_color = gray;
	if (u->pi) {
		// if it has a parent in the transpose graph, it's part of a scc, 
		// so it should have the same minlabel as its parent
		original->minlabels[u->value] = original->minlabels[u->pi->value];
		// if child is smaller than the current pointed-to value, then change the value
		if (u->value <  *(original->minlabels[u->value])) {
			*(original->minlabels[u->value]) = u->value;
		}
	} else {
		
		// doesn't have a parent in transpose, is dead end; initialize minlabel to itself
		int *current_min = malloc(sizeof(int*));
		original->minlabels[u->value] = current_min;
		*current_min = u->value;
	}
	
	int nbr;
	vertex *v;
	for (int j = 0; j < u->list_len; j++) {
		nbr = u->neighbours[j];
		v = getVertex(graph_transpose, nbr);
		// if (v->vertex_color == gray) {
		// 	printf("Found a back edge: %d, %d\n", u->value, v->value);
		// }
		if (v->vertex_color == white) {
			v->pi = u;
			scc_dfs(graph_transpose, v, original);
		}
	}
	u->vertex_color = black;
}

void scc(Graph *graph, Graph *graph_transpose) {
	// given a graph on which DFS has been run which has a topo order, and a graph transpose, 
	// run DFS in topo order on the transpose in order to identify strongly connected components
	// and populate the graph's minlabel array of pointers

	graph->minlabels = malloc(sizeof(int*) * graph->vertexCount);

	int topoCount = graph->topoCount;
	int *topoOrder = graph->topoSort;

	vertex *current;

	for (int i = topoCount; i > 0; i--) {
		// graph_transpose vertices are all white
		current = getVertex(graph_transpose, topoOrder[i-1]);
		// printf("current is: %d\n", current->value);
		scc_dfs(graph_transpose, current, graph);
	}
}

Graph* transpose(Graph *graph) {
	// returns the transpose of a graph in O(|V|+|E|) time
	// a little wasteful bc it constructs the nodes again, but it's still O(V+E)
	Graph *new = newGraph();

	// recreate vertices
	for (int i = 0; i < graph->vertexCount; i++) {
		addVertex(new, graph->sortedVertices[i]);
	}

	// recreate edges but reversed
	for (int i = 0; i < graph->vertexCount; i++) {
		int value = graph->sortedVertices[i];
		vertex *v;
		v = getVertex(graph, value);
		for (int j = 0; j < v->list_len; j++) {
			addEdge(new, v->neighbours[j], v->value, 0);
		}
	}
	return new;
}

void min_reachable(Graph *graph) {
	// run DFS to populate topo-sort array
	alpha_dfs(graph);

	Graph *transp = transpose(graph);
	scc(graph, transp);
	// populate each vertex with itself as its initial minlabel, 
	// but set all connected components to same pointer, pointing to lowest value in SCC

	// now copy minlabels from the graph onto the vertices
	vertex *current;
	for (int i = 0; i < graph->vertexCount; i++) {
		current = getVertex(graph, graph->sortedVertices[i]);
		current->minlabel = *(graph->minlabels[current->value]);
	}

	int topoCount = graph->topoCount;
	int *topoOrder = graph->topoSort;

	for (int i = 0; i < topoCount; i++) {
		current = getVertex(graph, topoOrder[i]);
		vertex *parent;
		parent = current->pi;

		if (parent) {
			parent->minlabel = min(parent->minlabel, current->minlabel);
		}	
	}
}

void printMinLabels(Graph *graph) {
	vertex *current;
	for (int i = 0; i < graph->vertexCount; i++) {
		current = getVertex(graph, graph->sortedVertices[i]);
		printf("vertex: %d | min(u): %d \n", current->value, current->minlabel);
	}

}

int main() {

	Graph *graph = newGraph();

	// test case 1: simple ---------------
	// for (int i = 1; i <= 3; i++) {
	// 	addVertex(graph, i);	
	// }

	// addEdge(graph, 1, 2, 0);
	// addEdge(graph, 2, 3, 0);

	// test case 2 --------------

	for (int i = 1; i <= 3; i++) {
		addVertex(graph, i);
	}

	addVertex(graph, 5);

	addVertex(graph, 7);
	addVertex(graph, 4);
	addVertex(graph, 9);

	addEdge(graph, 1, 2, 0);
	addEdge(graph, 2, 3, 0);
	addEdge(graph, 3, 1, 0);

	addEdge(graph, 1, 5, 0);

	addEdge(graph, 3, 7, 0);
	addEdge(graph, 7, 4, 0);
	addEdge(graph, 7, 9, 0);


	// end test ------------------------------------

	printf("---- Adjacency list: -----\n");
	printAdjList(graph);

	printf("---- Minimum reachable from each vertex: -----\n");
	min_reachable(graph);
	printMinLabels(graph);

	return 0;
	
}