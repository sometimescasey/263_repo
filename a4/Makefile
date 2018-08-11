all : offline offline_pointers_for_k kruskal

offline : offline.c disjoint.h
	gcc -Wall -std=gnu99 -o offline offline.c

offline_pointers_for_k : offline_pointers_for_k.c disjoint.h
	gcc -Wall -std=gnu99 -o offline_pointers_for_k offline_pointers_for_k.c

disjoint_unittest : disjoint_unittest.c
	gcc -Wall -std=gnu99 -o disjoint_unittest disjoint_unittest.c -lm `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0`

kruskal : kruskal.c graph_util.h disjoint.h
	gcc -Wall -std=gnu99 -o kruskal kruskal.c graph_util.c -lm `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0`
