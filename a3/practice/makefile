all : g_bfs

bfs : bfs.c
	gcc --std=gnu99 -Wall -o bfs bfs.c `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0`

g_bfs : g_bfs.c
	gcc --std=gnu99 -Wall -o g_bfs g_bfs.c `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0`