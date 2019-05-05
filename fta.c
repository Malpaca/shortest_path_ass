#include "dijkstra.h"

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

void distance(char * filename, int total, char a[4], char b[4], int * names, char ** indices){
    //start and end string mapping recorded
    int start = names[name_map(a)];
    int end = names[name_map(b)];
    //open file and extract undirected flight path as edge and translate into graph data structure
    FILE * fp;
    fp = fopen(filename, "r");
    if(fp == NULL) printf("Error opening file %s\n",filename);
    //initialize graph to record vertex and edge
    Graph * g = generate_graph(total);
    //read from file and add edge to the structure
    int dist; char depart[4], arrival[4];
    while(fscanf(fp, "%s %s %d", depart, arrival, &dist) != EOF) {
        // printf("%d: %s:%d, %s:%d, %d\n", total, depart, names[name_map(depart)], arrival, names[name_map(arrival)],dist);
        add_edge(g, names[name_map(depart)], names[name_map(arrival)], dist);
        // printf("%d,%d,%d,%d\n",
        //                     g->vertices[names[name_map(depart)]]->e_len,
        //                     g->vertices[names[name_map(depart)]]->e_size,
        //                     g->vertices[names[name_map(depart)]]->dist,
        //                     g->vertices[names[name_map(depart)]]->visited);
        add_edge(g, names[name_map(arrival)], names[name_map(depart)], dist);
        // if (!strcmp(depart, "CLT") && dist == 613) break;
        // printf("%d,%d,%d,%d\n", g->vertices[names[name_map(arrival)]]->e_len,
        //                     g->vertices[names[name_map(arrival)]]->e_size,
        //                     g->vertices[names[name_map(arrival)]]->dist,
        //                     g->vertices[names[name_map(arrival)]]->visited);
    }
    fclose(fp);
    //run dijkstra
    dijkstra(g, start, end, total, indices);
    free_all(g, total);
    // print_path(g, end);
}

int main(int argc, char ** argv){
    if (argc < 3) {
        printf("Usage: ./fta <airport file name> <flight file name>\n");
        return 1;
    }
    //map airport name to unique num
    int names[26*26*26] = {-1};
    int total = extract_vertex(argv[1], names);
    char ** indices = map_index(argv[1], total);
    //read user input and process input
    char input[20];
    char *start, *end, *command;
    printf("Enter Command > ");
    while(fgets(input, 20, stdin)) {
        command = strtok(input, " \n");
        start = strtok(NULL," \n");
        end = strtok(NULL," \n");
        //quiting
        if (!strcasecmp(command, "quit")) {
            printf("Exiting\n");
            break;
        }
        //help command
        else if (!strcasecmp(command, "help")) {
            printf("Help Command\n\n");
        }
        //print airport
        else if (!strcasecmp(command, "airports")) {
            airport(argv[1]);
        }
        //distance
        else if (!strcasecmp(command, "distance")) {
            distance(argv[2], total, start, end, names, indices);
        }
        else{
            printf("Invalid input, use \"help\" to see avaliable command\n\n");
        }
        printf("Enter Command > ");
    }
    free_indices(indices, total);
    return 0;
}
