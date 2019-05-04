CC = gcc
CFLAGS = -Wall -std=c99

all: fta

dijkstra.o: dijkstra.h dijkstra.c
	$(CC) $(CFLAGS) -c dijkstra.c

heap.o: heap.c heap.h
	$(CC) $(CFLAGS) -c heap.c

fta.o: fta.c heap.h
	$(CC) $(CFLAGS) -c fta.c

fta: fta.o heap.o dijkstra.o
	$(CC) $(CFLAGS) -o fta fta.o heap.o dijkstra.o

clean:
	rm *.o fta
