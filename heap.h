#ifndef HEAP_H
#define HEAP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

typedef struct HNode{
  int name;
  int score;
}HNode;

typedef struct MinHeap{
  HNode *arr;
  int *index;
  int count;
}MinHeap;

MinHeap * createMinHeap(int capacity);
void insert(MinHeap * heap, int name, int score);
void siftup(MinHeap * heap, int index);
void siftdown(MinHeap * heap, int index);
int popmin(MinHeap * h);
void free_heap(MinHeap *h);
void print_heap(MinHeap *h);

#endif
