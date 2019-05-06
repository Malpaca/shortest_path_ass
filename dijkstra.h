#ifndef DIJKSTRA_H
#define DIJKSTRA_H

#include "heap.h"
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int vertex;
    int weight;
} Edge;

typedef struct {
    Edge **edges;
    int e_len;
    int e_size;
    int score;
    int prev;
    int e_to_next;
    int visited;
} Vertex;

typedef struct {
    Vertex **vertices;
} Graph;

void add_vertex (Graph *g, int i);
void add_edge (Graph *g, int a, int b, int w);
Graph * generate_graph(int total);
void dijkstra (Graph *g, int a, int b, int total);
void free_dijkstra(Graph *g, int total);
void print_path (Graph *g, int end, char ** indices);

#endif
