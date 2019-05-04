#include "heap.h"

MinHeap * createMinHeap(int capacity){
  MinHeap *h = (MinHeap *) calloc(1,sizeof(MinHeap)); //one is number of heap
  //check if memory allocation is fails
  if(h == NULL){
      printf("Memory Error!");
      return NULL;
  }
  h->count=0;
  h->capacity = capacity;
  h->arr = (HNode *) calloc(capacity, sizeof(HNode)); //size in bytes
  //check if allocation succeed
  if (h->arr == NULL){
      printf("Memory Error!");
      return NULL;
  }
  return h;
}

void insert(MinHeap * h, int name, int score){
  HNode data = {name, score};
  h->arr[h->count] = data;
  siftup(h, h->count);
  h->count++;
}

void siftup(MinHeap * h, int index){
  HNode temp;
  int parentindex = (index - 1)/2;

  while((h->arr[index]).score < (h->arr[parentindex]).score){
    temp = h->arr[index];
    h->arr[index] = h->arr[parentindex];
    h->arr[parentindex] = temp;
    index = parentindex;
    parentindex = (index - 1)/2;
  }
}

void siftdown(MinHeap * h, int index){
  int left = index*2+1;
  int right = index*2+2;
  int min;
  HNode temp;

  if(left >= h->count || left <0)
      left = 0;
  if(right >= h->count || right <0)
      right = 0;

  if(left && (h->arr[left]).score < (h->arr[index]).score)
      min=left;
  else
      min =index;
  if(right  && (h->arr[right]).score < (h->arr[min]).score)
      min = right;

  if(min != index){
      temp = h->arr[min];
      h->arr[min] = h->arr[index];
      h->arr[index] = temp;

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
  h->arr[0] = h->arr[h->count-1];
  h->count--;
  siftdown(h, 0);
  return min.name;
}

void free_heap(MinHeap *h){
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
