all : bfs

bfs : bfs.c
	gcc --std=gnu99 -Wall -o bfs `pkg-config --cflags glib-2.0` `pkg-config --libs glib-2.0` bfs.c