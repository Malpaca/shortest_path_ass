#include "dijkstra.h"

void add_vertex (Graph *g, int i) {
    if (!g->vertices[i]) {
        // printf("%d\n",i);
        g->vertices[i] = calloc(1, sizeof (Vertex));
        g->vertices[i]->e_len = 0;
        g->vertices[i]->e_size = 0;
        g->vertices[i]->dist = INT_MAX;
        g->vertices[i]->visited = 0;
    }
}

void add_edge (Graph *g, int a, int b, int w) {
    add_vertex(g, a);
    // printf("%d:v1\n",a);
    add_vertex(g, b);
    // printf("%d:v2\n",b);
    Vertex *v = g->vertices[a];
    if (v->e_len >= v->e_size) {
        v->e_size = v->e_size ? v->e_size * 2 : 4;
        v->edges = realloc(v->edges, v->e_size * sizeof (Edge *));
    }
    Edge *e = malloc(sizeof (Edge));
    e->vertex = b;
    e->weight = w;
    v->edges[v->e_len++] = e;
}

Graph * generate_graph(int total){
  Graph * g = (Graph * ) calloc(1,sizeof(Graph));
  g->vertices = (Vertex **)calloc(total, sizeof (Vertex *));
  return g;
}

void free_all(Graph * g, int total, MinHeap * h){
    free_heap(h);
    for (int i = 0; i < total; i++){
        if (g->vertices[i]) {
            for (int j = 0; j < g->vertices[i]->e_len; j++){
                free(g->vertices[i]->edges[j]);
            }
            free(g->vertices[i]->edges);
        }
        free(g->vertices[i]);
    }
    free(g->vertices);
    free(g);
}

void dijkstra (Graph *g, int a, int b, int total) {
    int i, j;
    for (i = 0; i < total; i++) {
        Vertex *v = g->vertices[i];
        if (!g->vertices){
            printf("%d\n",i);
            continue;
        }
        v->dist = INT_MAX;
        v->visited = 0;
    }
    Vertex *v = g->vertices[a];
    v->dist = 0;
    MinHeap * h = createMinHeap(total);
    insert(h, a, v->dist);
    while (h->count) {
        i = popmin(h);
        if (i == b)
            break;
        v = g->vertices[i];
        v->visited = 1;
        for (j = 0; j < v->e_len; j++) {
            Edge *e = v->edges[j];
            Vertex *u = g->vertices[e->vertex];
            if (!u->visited && v->dist + e->weight <= u->dist) {
                u->dist = v->dist + e->weight;
                insert(h, e->vertex, u->dist);
            }
        }
    }
    printf("Distance:%d\n",g->vertices[b]->dist);
    free_all(g, total, h);
}
