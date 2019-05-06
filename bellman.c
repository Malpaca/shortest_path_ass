#include "bellman.h"

Graph_b * createGraph(int V, int E){
    Graph_b * g = (Graph_b *)calloc(1,sizeof(Graph_b));
    g->v = V;
    g->e = E;
    g->edges = calloc(E, sizeof(Edge_b));
    return g;
}

void free_bellman(Graph_b * g){
    free(g->edges);
    free(g);
}

void bellman (Graph_b *g, int start, int end, int total) {
    int v = g->v;
    int e = g->e;
    int dist[v];
    for (int i = 0; i < v; i++) {
        dist[i] = INT_MAX;
    }
    dist[start] = 0;

    for(int i = 1; i < v-1; i++){
        for (int j = 0; j < e; j++) {
            int u = g->edges[j].src;
            int v = g->edges[j].dest;
            int weight = g->edges[j].w;
            if (dist[u] != INT_MAX && dist[u] + weight < dist[v])
                dist[v] = dist[u] + weight;
        }
    }
}
