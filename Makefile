CC = gcc
CFLAGS = -Wall -std=c99

all: fta test

dijkstra.o: dijkstra.h dijkstra.c
	$(CC) $(CFLAGS) -c dijkstra.c

bellman.o: bellman.h bellman.c
	$(CC) $(CFLAGS) -c bellman.c

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) -c heap.c

fta.o: fta.c dijkstra.h
	$(CC) $(CFLAGS) -c fta.c

test.o: test.c bellman.h dijkstra.h
	$(CC) $(CFLAGS) -c test.c

fta: fta.o heap.o dijkstra.o
	$(CC) $(CFLAGS) -o fta fta.o heap.o dijkstra.o

test: test.o heap.o dijkstra.o bellman.o
	$(CC) $(CFLAGS) -o test test.o heap.o dijkstra.o bellman.o

clean:
	rm *.o fta test
