#include "heap.h"

MinHeap * createMinHeap(int capacity){
    MinHeap *h = (MinHeap *) calloc(1,sizeof(MinHeap)); //one is number of heap
    //check if memory allocation is fails
    if(h == NULL){
        printf("Memory Error!");
        return NULL;
    }
    h->count=0;
    h->arr = (HNode *) calloc(capacity, sizeof(HNode)); //size in bytes
    h->index = calloc(capacity, sizeof(int)); //size in bytes
    for (int i = 0; i < capacity; i++){
        h->index[i] = -1;
    }
    //check if allocation succeed
    if (h->arr == NULL){
        printf("Memory Error!");
        return NULL;
    }
    return h;
}

void insert(MinHeap * h, int name, int score){
    if (h->index[name] == -1){
        HNode data = {name, score};
        h->arr[h->count] = data;
        h->index[name]=h->count;
        siftup(h, h->count);
        h->count++;
    }else{
        if ((h->arr[h->index[name]]).score > score){
            (h->arr[h->index[name]]).score = score;
            siftup(h, h->index[name]);
        }
    }
}

void siftup(MinHeap * h, int i){
    HNode temp;
    int tmp;
    int pi = (i - 1)/2;

    while((h->arr[i]).score < (h->arr[pi]).score){
        tmp = h->index[(h->arr[pi]).name];
        h->index[(h->arr[pi]).name] = h->index[(h->arr[i]).name];
        h->index[(h->arr[i]).name] = tmp;
        temp = h->arr[pi];
        h->arr[pi] = h->arr[i];
        h->arr[i] = temp;
        i = pi;
        pi = (pi - 1)/2;
    }
}

void siftdown(MinHeap * h, int i){
    int left = i*2+1;
    int right = i*2+2;
    int min=i;
    int tmp;
    HNode temp;

    if (left < h->count && (h->arr[left]).score < (h->arr[i]).score)
        min = left;
    if (right < h->count && (h->arr[right]).score < (h->arr[min]).score)
        min = right;

    if(min != i){
        tmp = h->index[(h->arr[min]).name];
        h->index[(h->arr[min]).name] = h->index[(h->arr[i]).name];
        h->index[(h->arr[i]).name] = tmp;
        temp = h->arr[min];
        h->arr[min] = h->arr[i];
        h->arr[i] = temp;
        // recursive  call
        siftdown(h, min);
    }
}

int popmin(MinHeap * h){
    if (!h->count){
        printf("Heap is empty\n");
        return -1;
    }
    HNode min = h->arr[0];
    h->index[min.name] = -1;
    h->arr[0] = h->arr[h->count-1];
    h->count--;
    siftdown(h, 0);
    return min.name;
}

void free_heap(MinHeap *h){
    free(h->index);
    free(h->arr);
    free(h);
}

void print_heap(MinHeap *h){
    int i;
    printf("____________Print Heap_____________\n");
    for(i=0;i< h->count;i++){
        printf("-> %d ",(h->arr[i]).score);
    }
    printf("->__/\\__\n");
}
