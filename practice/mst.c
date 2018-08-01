#include <stdio.h>
#include <stdlib.h>
#include <glib.h> // for the hash table, O(1) avg lookup
#include <math.h>

// DFS stack - we actually haven't used this
#define push(sp, n) (*((sp)++) = (n)) // push value, increment pointer to next empty space
#define pop(sp) (*--(sp)) // decrement pointer, return value

#define MAXV 1000 // for malloc() purposes
#define MAXADJ 10 // for malloc() purposes
#define POS_INF 2147483647

int dfs_time; // global var; see if you can make scope smaller later

typedef enum vertex_color 
        { 
          white, 
          gray,
          black 
        } vertex_color; 

typedef struct wEdge wEdge;
// weighted directed edge

struct wEdge {
	int to;
	int weight;
};

typedef struct vertex vertex;

struct vertex {
	int value;
	wEdge **neighbours; // array of pointers to wEdge
	int list_len;
	
	vertex_color vertex_color;
	vertex *pi;
	
	int t_d; // discovery time
	int t_f; // finish time
};

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

int sortedInsertwEdge(wEdge **array, int arr_len, int v, int weight) {
	// insert v into sorted array
	// running time O(n) where n == arr_len, so either O(|V|) or O(|E|) depending
	// on what we run on. therefore still linear
	if (arr_len == 0) { // empty: just insert
		array[0] = malloc(sizeof(wEdge));
		array[0]->to = v;
		array[0]->weight = weight;
	} else {
		int i = arr_len - 1;
		array[i+1] = malloc(sizeof(wEdge));
		
		printf("pointer at array start: %p\n", array);

		printf("pointer at last item: %p\n", array[i]);
		
		// start at last item
		while (i >= 0 && v < (array[i])->to) {
			printf("(array[i])->to: %d\n", (array[i])->to);
			// move everything over 1
			printf("fine\n");
			(array[i+1])->to = (array[i])->to;
			printf("fine\n");
			(array[i+1])->weight = (array[i])->weight;
			printf("fine\n");
			i--;
			printf("i: %d\n", i);
		}
		i++;
		printf("i: %d\n", i);
		(array[i])->to = v;
		printf("fine\n");
		(array[i])->weight = weight;

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
	printf("vertex->list_len: %d\n", vertex->list_len);
	// check that neighbour isn't already in list
	for (int i = 0; i < vertex->list_len; i++) {
		if (vertex->neighbours[i]->to == neighbour) {
			fprintf(stderr, "%d already has %d as neighbour\n", vertex->value, neighbour);
			return;
		}
	}
	
	// now we do in alpha order!
	printf("Inserting %d into adj list of vertex %d\n", neighbour, vertex->value);
	sortedInsertwEdge(vertex->neighbours, vertex->list_len, neighbour, weight); 
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
	// print this graph's adjacency table.
	for (int i = 0; i < graph->vertexCount; i++) {
		int value = graph->sortedVertices[i];
		printf("key: %d | neighbours: ", value);
		vertex *v;
		v = getVertex(graph, value);
		for (int j = 0; j < v->list_len; j++) {
			printf("(%d ", v->neighbours[j]->to);
			printf("w=%d) ", v->neighbours[j]->weight);
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
		nbr = deref->neighbours[i]->to;
		v = getVertex(graph, nbr);
		if (v->vertex_color == gray) {
			printf("Found a back edge: %d, %d\n", deref->value, v->value);
		}
		if (v->vertex_color == white) {
			v->pi = deref;
			dfs_visit(graph, v);
		}
	}
	deref->vertex_color = black;
	graph->topoSort[graph->topoCount] = deref->value;
	graph->topoCount++;
	printf("finished %d\n", deref->value);
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

void dfs(Graph * graph) {
	g_hash_table_foreach(graph->adj_list, initialize_dfs, NULL);
	// set all vertices to white and t_d, t_f values to 0
	dfs_time = 0;
	g_hash_table_foreach(graph->adj_list, onEachVertex, graph);
}

void alpha_dfs(Graph * graph) {
	// alphabetical dfs
	g_hash_table_foreach(graph->adj_list, initialize_dfs, NULL);
	// set all vertices to white and t_d, t_f values to 0
	dfs_time = 0;
	vertex *current;
	for (int i = 0; i < graph->vertexCount; i++) {
		current = getVertex(graph, graph->sortedVertices[i]);
		printf("starting on vertex %d\n", current->value);
		if (current->vertex_color == white) {
			dfs_visit(graph, current);
		}
	}
}

void post_dfs_info(Graph *graph) {
	// given a graph with DFS run on it, print info
	vertex *current;
	printf("%d\n", graph->vertexCount);
	for (int i = 0; i < graph->vertexCount; i++) {
		// printf("graph->sortedVertices[i]: %d\n", graph->sortedVertices[i]);
		current = getVertex(graph, graph->sortedVertices[i]);
		
		int pi;
		if (current->pi) {
			pi = current->pi->value;
		} else {
			// handle case of starting node which has no parent
			pi = -POS_INF;
		}

		printf("Vertex: %d\t| t_d: %d\t| t_f: %d\t| pi: %d\n", 
			current->value,
			current->t_d,
			current->t_f,
			pi
			);
	
	}

}

void topoPrint(Graph *graph) {
	// note that this prints backwards
	// socks must come BEFORE everything else
	// so they FINISH last
	printf("Topological sort: ");
	for (int i = graph->topoCount - 1; i >= 0; i--) {
		printf("%d ", graph->topoSort[i]);
	}
	printf("\n");
}

void findAllMST(Graph *graph, vertex *start) {
	// starting from a certain start vertex, find all valid MSTs


}

void MST(Graph *graph, vertex *start) {
	// starting from a certain vertex, run MST, Prim's


	}

int main() {

	Graph *graph = newGraph();

	// test case 1: clrs page 611 ---------------
	for (int i = 10; i >=1; i--) {
		addVertex(graph, i);	
	}

	addEdge(graph, 1, 4, 1, 1);
	addEdge(graph, 4, 9, 5, 1);
	addEdge(graph, 2, 9, 1, 1);
	addEdge(graph, 5, 9, 1, 1);
	addEdge(graph, 2, 5, 1, 1);

	addEdge(graph, 3, 6, 1, 1);
	addEdge(graph, 7, 3, 1, 1);
	addEdge(graph, 6, 7, 2, 1);
	addEdge(graph, 1, 3, 1, 1);
	addEdge(graph, 1, 7, 1, 1);

	addEdge(graph, 4, 8, 1, 1);
	addEdge(graph, 8, 10, 1, 1);
	addEdge(graph, 10, 8, 1, 1);
	addEdge(graph, 9, 1, 1, 1);

	printAdjList(graph);

	// ----------------------

	return 0;
	
}