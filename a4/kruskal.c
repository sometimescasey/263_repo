#include <stdio.h>
#include <stdlib.h>
#include <glib.h> // for the hash table, O(1) avg lookup
#include <math.h>

#include "graph_util.h"
#include "disjoint.h"

int main() {

	Graph *graph = newGraph();

// Aug 17 Q7 b) ----------------------
	int v_count = 9;
	vertex **vertex_array = malloc(v_count * sizeof(vertex*));

	for (int i = 0; i < v_count; i++) {
		vertex_array[i] = addVertex(graph, i+1);	
	}

	int edgeCount = 17; // sad hardcode for now, think of better way to do tmr
	graph->edgeArray = malloc(sizeof(wEdge*) * edgeCount);


	// box 1
	addEdge(graph, 1, 2, 4, 1);
	addEdge(graph, 1, 3, 14, 1);
	addEdge(graph, 2, 3, 12, 1);
	addEdge(graph, 2, 4, 2, 1);
	addEdge(graph, 3, 4, 15, 1);

	// box 2
	addEdge(graph, 3, 5, 11, 1);
	
	addEdge(graph, 4, 5, 1, 1);
	addEdge(graph, 5, 6, 7, 1);
	addEdge(graph, 4, 6, 16, 1);

	// box 3
	addEdge(graph, 5, 7, 13, 1);

	addEdge(graph, 7, 8, 3, 1);
	addEdge(graph, 6, 7, 10, 1);
	addEdge(graph, 6, 8, 8, 1);

	// bottom fin
	addEdge(graph, 2, 9, 5, 1);
	addEdge(graph, 4, 9, 0, 1);
	addEdge(graph, 6, 9, 9, 1);
	addEdge(graph, 8, 9, 6, 1);

	// iterate thru edge array
	for (int i=0; i < edgeCount; i++) {
		printf("Edge (%d->%d, w: %d)\n", 
			graph->edgeArray[i]->to, 
			graph->edgeArray[i]->from, 
			graph->edgeArray[i]->weight);
	}

	// sort it
	printf("SORTING\n");
	sortEdgeArray(graph->edgeArray, edgeCount);

	// iterate again
	for (int i=0; i < edgeCount; i++) {
		printf("Edge (%d->%d, w: %d)\n", 
			graph->edgeArray[i]->to, 
			graph->edgeArray[i]->from, 
			graph->edgeArray[i]->weight);
	}



}