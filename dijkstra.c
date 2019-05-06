#include "dijkstra.h"

void add_vertex (Graph *g, int i) {
    //insert if does not exist
    if (!g->vertices[i]) {
        g->vertices[i] = calloc(1, sizeof (Vertex));
        g->vertices[i]->e_len = 0;
        g->vertices[i]->e_size = 0;
    }
}

void add_edge (Graph *g, int a, int b, int w) {
    add_vertex(g, a);
    add_vertex(g, b);

    Vertex *v = g->vertices[a];
    //does not add it edge already exist
    for (int i = 0; i < v->e_len; i++){
        if ((v->edges[i])->vertex == b) return;
    }
    //dynamically rescale size of edges
    if (v->e_len >= v->e_size) {
        v->e_size = v->e_size ? v->e_size * 2 : 4;
        v->edges = realloc(v->edges, v->e_size * sizeof (Edge *));
    }
    //record edge
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
    //record edge to next, basically reverse the prev field
    while(v->score != 0){
        v = g->vertices[v->prev];
        //find edge to next
        for(int i = 0; i < v->e_len; i++){
            if (v->edges[i]->vertex == last){
                v->e_to_next = i;
                last = g->vertices[v->edges[i]->vertex]->prev;
            }
        }
    }
    Vertex * v_next;
    //print path
    while(v->e_to_next != -1){
        v_next = g->vertices[v->edges[v->e_to_next]->vertex];
        printf("%s->%s:%d\n", indices[v_next->prev], indices[v->edges[v->e_to_next]->vertex], v->edges[v->e_to_next]->weight);
        v = v_next;
    }
    printf("Total Distance: %d\n",v->score);
}

void dijkstra (Graph *g, int start, int end, int total) {
    //initialize all vertex
    for (int i = 0; i < total; i++) {
        Vertex *v = g->vertices[i];
        v->score = INT_MAX;
        v->visited = 0;
        v->prev = -1;
        v->e_to_next = -1;
    }
    Vertex *v_curr = g->vertices[start];
    v_curr->score = 0;
    //initialize heap
    MinHeap * h = createMinHeap(total);
    insert(h, start, v_curr->score);
    //main dijkstra
    while (h->count) {
        int i = popmin(h);
        if (g->vertices[i]->visited){
            continue;
        }
        if (i == end)
            break;
        v_curr = g->vertices[i];
        v_curr->visited = 1;
        //go through all edge
        for (int j = 0; j < v_curr->e_len; j++) {
            Edge *e = v_curr->edges[j];
            Vertex *v_next = g->vertices[e->vertex];
            if (!v_next->visited && v_curr->score + e->weight <= v_next->score) {
                v_next->score = v_curr->score + e->weight;
                v_next->prev = i;
                insert(h, e->vertex, v_next->score);
            }
        }
    }
    free_heap(h);
}
