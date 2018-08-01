all : dfs mst

dfs : dfs.c
	gcc -Wall -std=gnu99 -o dfs dfs.c -lm `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0`

mst : mst.c
	gcc -Wall -std=gnu99 -o mst mst.c -lm `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0`