#include "bellman.h"
#include "dijkstra.h"
#include "ctype.h"
#include <time.h>

void print_progress(size_t current, size_t user_min, size_t user_max){
  double count = 100*((double)current - (double)user_min)/((double)user_max-(double)user_min);
	const char prefix[] = "Progress: [";
	const char suffix[] = "]";
	const size_t prefix_length = sizeof(prefix) - 1;
	const size_t suffix_length = sizeof(suffix) - 1;
	char *buffer = calloc(100 + prefix_length + suffix_length + 1, 1); // +1 for \0
	size_t i = 0;

	strcpy(buffer, prefix);
	for (; i < 100; ++i)
	{
		buffer[prefix_length + i] = i < count ? '#' : ' ';
	}

	strcpy(&buffer[prefix_length + i], suffix);
	printf("\r%s progress:%.1f%%", buffer, count);
	fflush(stdout);
	free(buffer);
}

int name_map(char* name){
    int a = (name[0]-'A')*26*26 + (name[1]-'A')*26 + (name[2]-'A');
    // printf("%d\n",a);
    return a;
}

char** map_index(char * filename, int total){
    char ** indices = calloc(total, sizeof(char*));
    FILE * fp;
    fp = fopen(filename, "r");
    if(fp == NULL) printf("Error opening file %s\n",filename);
    char buf[50];
    int count = 0;
    while(fgets(buf, 50, fp) != NULL) {
        char * name = calloc(4, sizeof(char));
        strncpy(name, buf, 3);
        name[3] = '\0';
        indices[count] = name;
        count++;
    }
    fclose(fp);
    return indices;
}

void free_indices(char ** indices, int total){
    for(int i=0; i < total; i++){
        free(indices[i]);
    }
    free(indices);
}

int extract_vertex(char * filename, int * arr){
    for(int i = 0; i < 26*26*26; i++){
        arr[i] = -1;
    }
    FILE * fp;
    fp = fopen(filename, "r");
    if(fp == NULL) printf("Error opening file %s\n",filename);
    char buf[50];
    int count = 0;
    while(fgets(buf, 50, fp) != NULL) {
        // printf("%s",buf);
        buf[3] = '\0';
        arr[name_map(buf)] = count;
        count++;
    }
    fclose(fp);
    return count;
}

void airport(char * filename){
    FILE * fp;
    fp = fopen(filename, "r");
    if(fp == NULL) printf("Error opening file %s\n",filename);
    //read from file and print
    char buf[50];
    while(fgets(buf, 50, fp) != NULL) {
        printf("%s",buf);
    }
    printf("\n");
    fclose(fp);
}

void compare(char * filename, int total, int * names, char ** indices){
    //open file and extract undirected flight path as edge and translate into graph data structure
    FILE * fp;
    fp = fopen(filename, "r");
    if(fp == NULL) printf("Error opening file %s\n",filename);
    //initialize graph to record vertex and edge
    Graph * g = generate_graph(total);
    //read from file and add edge to the structure
    int edge = 0;
    int dist; char depart[4], arrival[4];
    while(fscanf(fp, "%s %s %d", depart, arrival, &dist) != EOF) {
        add_edge(g, names[name_map(depart)], names[name_map(arrival)], dist);
        add_edge(g, names[name_map(arrival)], names[name_map(depart)], dist);
        edge+=2;
    }
    fclose(fp);

    Graph_b * g_b = createGraph(total, edge);
    fp = fopen(filename, "r");
    if(fp == NULL) printf("Error opening file %s\n",filename);
    int edge_add = 0;
    while(fscanf(fp, "%s %s %d", depart, arrival, &dist) != EOF) {
        g_b->edges[edge_add].src = names[name_map(depart)];
        g_b->edges[edge_add].dest = names[name_map(arrival)];
        g_b->edges[edge_add].w = dist;
        g_b->edges[edge_add+1].src = names[name_map(arrival)];
        g_b->edges[edge_add+1].dest = names[name_map(depart)];
        g_b->edges[edge_add+1].w = dist;
        edge_add+=2;
    }
    fclose(fp);



    fp = fopen("performance.csv", "w+");
    fprintf(fp,"N,time_bellman,time_dijkstra\n");
    double time_bellman, time_dijkstra;
    for(int i = 0; i < total; i ++){
        for(int j = 0; j < total; j++){
            clock_t s, e;
            s = clock();
              bellman(g_b, i, j, total);
            e = clock();
            time_bellman = (double)(e-s) / CLOCKS_PER_SEC;

            s = clock();
              dijkstra(g, i, j, total);
            e = clock();
            time_dijkstra = (double)(e-s) / CLOCKS_PER_SEC;

            fprintf(fp,"%d,%f,%f\n",j+i*total,time_bellman,time_dijkstra);
            print_progress(j+i*total, 0, total*total-1);
        }
    }

    // printf("Bellman:%f Dijkstra:%f\n", time_bellman, time_dijkstra);
    free_bellman(g_b);
    free_dijkstra(g, total);
}

int main(int argc, char ** argv){
    if (argc < 3) {
        printf("Usage: ./fta <airport file name> <flight file name>\n");
        return 1;
    }
    //map airport name to unique num
    int names[26*26*26];
    int total = extract_vertex("large-airports.txt", names);
    char ** indices = map_index("large-airports.txt", total);
    //read user input and process input
    compare("large-dists.txt", total, names, indices);
    free_indices(indices, total);
    return 0;
}
