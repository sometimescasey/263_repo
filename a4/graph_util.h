#ifndef _GRAPH_UTIL_H_
#define _GRAPH_UTIL_H_

typedef struct Item Item;

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
	int from;
	int weight;
};

typedef struct vertex vertex;

struct vertex {
	int value;
	wEdge **neighbours; // array of pointers to wEdge
	int list_len;
	
	vertex_color vertex_color;
	vertex *pi;
	Item *item; // pointer to disjoint set item
};

typedef struct Graph Graph;

struct Graph {
	GHashTable *adj_list;
	int *sortedVertices; // alpha sorting for textbook example
	int vertexCount;
	wEdge **edgeArray;
	int edgeIndex;

};

int sortedInsert(int *array, int arr_len, int v);
int sortedInsertwEdge(wEdge **array, int arr_len, int from, int v, int weight, Graph *graph);

Graph * newGraph();

vertex * getVertex(Graph *graph, int n);

vertex * addVertex(Graph *graph, int value);

void addNeighbour(Graph *graph, vertex *vertex, int neighbour, int weight);

void addEdge(Graph *graph, int from_vertex, int to_vertex, int weight, int undirected);

char getLetter(int n);

void printListRow(gpointer key, gpointer value, gpointer userdata);

void printAdjList(Graph * graph);

int compareWeights(const void *p, const void *q);
void sortEdgeArray(wEdge **edgeArray, int edgeCount);

#endif