#include "dijkstra.h"
#include "ctype.h"

//map each three capital character string to a unique number
int name_map(char* name){
    int a = (name[0]-'A')*26*26 + (name[1]-'A')*26 + (name[2]-'A');
    return a;
}

//get the reverse map from the unique number back to the 3 character string
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

//read airport and
int extract_vertex(char * filename, int * arr){
    //initialize name map to -1, indicating this name doesn't have a num associated with it
    for(int i = 0; i < 26*26*26; i++){
        arr[i] = -1;
    }
    FILE * fp;
    fp = fopen(filename, "r");
    if(fp == NULL) printf("Error opening file %s\n",filename);
    char buf[50];
    int count = 0;
    //extract airport name into name map
    while(fgets(buf, 50, fp) != NULL) {
        buf[3] = '\0';
        arr[name_map(buf)] = count;
        count++;
    }
    fclose(fp);
    return count;
}

//print all airport
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

//compute the distance using dijkstra after checking valid input.
void distance(char * filename, int total, char a[4], char b[4], int * names, char ** indices){
    //start and end string mapping recorded
    int start = names[name_map(a)];
    int end = names[name_map(b)];
    if (start == -1 || end == -1){
        printf("Error, airport name not in record, please use airports command to see all airports\n");
        return;
    }
    //open file and extract undirected flight path as edge and translate into graph data structure
    FILE * fp;
    fp = fopen(filename, "r");
    if(fp == NULL) printf("Error opening file %s\n",filename);
    //initialize graph to record vertex and edge
    Graph * g = generate_graph(total);
    //read from file and add edge to the structure
    int dist; char depart[4], arrival[4];
    while(fscanf(fp, "%s %s %d", depart, arrival, &dist) != EOF) {
        add_edge(g, names[name_map(depart)], names[name_map(arrival)], dist);
        add_edge(g, names[name_map(arrival)], names[name_map(depart)], dist);
    }
    fclose(fp);
    //run dijkstra
    dijkstra(g, start, end, total);
    print_path(g, end, indices);
    free_dijkstra(g, total);
}

//convert string to upper case
void strupr(char * string){
	int i = 0;
	while (string[i] != '\0'){
    	if (string[i] >= 'a' && string[i] <= 'z') {
        	string[i] = string[i] - 32;
    	}
      	i++;
	}
}

//check if input source and dist exist as vertex
int name_check(char * start, char * end){
    if (strlen(start) != 3 || strlen(end) != 3) return 0;
    for(int i = 0; i < 3; i++){
        if(!isalpha(start[i]) || !isalpha(end[i])) return 0;
    }
    strupr(start);
    strupr(end);
    return 1;
}

int main(int argc, char ** argv){
    if (argc < 3) {
        printf("Usage: ./fta <airport file name> <flight file name>\n");
        return 1;
    }
    //map airport name to unique num
    int names[26*26*26];
    int total = extract_vertex(argv[1], names);
    char ** indices = map_index(argv[1], total);
    //read user input and process input
    char input[20];
    char *start, *end, *command;
    printf("Enter Command > ");
    while(fgets(input, 20, stdin)) {
        command = strtok(input, " \n");
        start = strtok(NULL," \n");
        end = start != NULL ? strtok(NULL," \n") : NULL;
        //quiting
        if (!strcasecmp(command, "quit")) {
            printf("Exiting\n");
            break;
        }
        //help command
        else if (!strcasecmp(command, "help")) {
            printf("Following are the avaliable command:\n\
                    quit:exit the program\n\
                    help: show all avaliable command and format\n\
                    airports: print all avaliable airports\n\
                    distance <depart> <arrival>: print the shortest path and distance from depart to arrival\n\
                    All command are case insensitive\n\n");
        }
        //print airport
        else if (!strcasecmp(command, "airports")) {
            airport(argv[1]);
        }
        //distance
        else if (!strcasecmp(command, "distance")) {
            if (start && end && name_check(start, end)) distance(argv[2], total, start, end, names, indices);
            else printf("Wrong input, airport name must be only 3 english alphabet character.\n");
            printf("\n");
        }
        else{
            printf("Invalid input, use \"help\" to see avaliable command\n\n");
        }
        printf("Enter Command > ");
    }
    free_indices(indices, total);
    return 0;
}
