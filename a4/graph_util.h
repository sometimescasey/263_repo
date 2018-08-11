#ifndef _GRAPH_UTIL_H_
#define _GRAPH_UTIL_H_


typedef struct vertex vertex;
typedef struct Graph Graph;
typedef struct wEdge wEdge;

int sortedInsert(int *array, int arr_len, int v);
int sortedInsertwEdge(wEdge **array, int arr_len, int v, int weight);

Graph * newGraph();

vertex * getVertex(Graph *graph, int n);

vertex * addVertex(Graph *graph, int value);

void addNeighbour(Graph *graph, vertex *vertex, int neighbour, int weight);

void addEdge(Graph *graph, int from_vertex, int to_vertex, int weight, int undirected);

char getLetter(int n);

void printListRow(gpointer key, gpointer value, gpointer userdata);

void printAdjList(Graph * graph);

#endif