#ifndef BELLMAN_H
#define BELLMAN_H

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int src,dest,w;
} Edge_b;

typedef struct {
    int v,e;
    Edge_b *edges;
} Graph_b;

void bellman (Graph_b *g, int a, int b, int total);
Graph_b * createGraph(int V, int E);
void free_bellman(Graph_b * g);

#endif
