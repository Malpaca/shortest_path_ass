#include "dijkstra.h"

void add_vertex (Graph *g, int i) {
    if (!g->vertices[i]) {
        // printf("%d\n",i);
        g->vertices[i] = calloc(1, sizeof (Vertex));
        g->vertices[i]->e_len = 0;
        g->vertices[i]->e_size = 0;
    }
}

void add_edge (Graph *g, int a, int b, int w) {
    add_vertex(g, a);
    add_vertex(g, b);

    Vertex *v = g->vertices[a];
    for (int i = 0; i < v->e_len; i++){
        if ((v->edges[i])->vertex == b) return;
    }
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

void free_dijkstra(Graph * g, int total){
    for (int i = 0; i < total; i++){
        if (g->vertices[i]) {
            for (int j = 0; j < (g->vertices[i])->e_len; j++){
                free((g->vertices[i])->edges[j]);
            }
            free((g->vertices[i])->edges);
        }
        free(g->vertices[i]);
    }
    free(g->vertices);
    free(g);
}

void print_path(Graph *g, int last, char ** indices){
    Vertex * v = g->vertices[last];
    while(v->score != 0){
        printf("%s:%d,%d\n", indices[v->prev],v->prev,last);
        v = g->vertices[v->prev];
        for(int i = 0; i < v->e_len; i++){
            if (v->edges[i]->vertex == last){
                v->e_to_next = i;
                last = g->vertices[v->edges[i]->vertex]->prev;
            }
        }
    }
    // printf("%d\n",v->e_to_next);
    // printf("%s->%s:%d", indices[start], indices[v->edges[v->e_to_next]->vertex], v->edges[v->e_to_next]->weight);
    Vertex * v_next;
    // v = g->vertices[start];
    while(v->e_to_next != -1){
        v_next = g->vertices[v->edges[v->e_to_next]->vertex];
        printf("%s->%s:%d\n", indices[v_next->prev], indices[v->edges[v->e_to_next]->vertex], v->edges[v->e_to_next]->weight);
        v = v_next;
    }
    printf("Total Distance: %d\n",v->score);
}

void dijkstra (Graph *g, int start, int end, int total) {
    int i, j;
    for (i = 0; i < total; i++) {
        Vertex *v = g->vertices[i];
        v->score = INT_MAX;
        v->visited = 0;
        v->prev = -1;
        v->e_to_next = -1;
    }
    Vertex *v_curr = g->vertices[start];
    v_curr->score = 0;
    MinHeap * h = createMinHeap(total);
    insert(h, start, v_curr->score);
    // print_heap(h);
    while (h->count) {
        i = popmin(h);
        if (g->vertices[i]->visited){
            // printf("duplictate:");
            // printf("v:%d\n",g->vertices[i]->score);
            continue;
        }
        // printf("v:%d\n",g->vertices[i]->score);
        if (i == end)
            break;
        v_curr = g->vertices[i];
        v_curr->visited = 1;
        for (j = 0; j < v_curr->e_len; j++) {
            Edge *e = v_curr->edges[j];
            Vertex *v_next = g->vertices[e->vertex];
            if (!v_next->visited && v_curr->score + e->weight <= v_next->score) {
                v_next->score = v_curr->score + e->weight;
                v_next->prev = i;
                // printf("%d->%d:%d\n",e->vertex, b, u->score);
                insert(h, e->vertex, v_next->score);
            }
        }
        // print_heap(h);
    }
    // printf("Total Distance: %d\n",g->vertices[end]->score);
    free_heap(h);
}
