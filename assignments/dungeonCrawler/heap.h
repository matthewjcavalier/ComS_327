#ifndef STDLIB_H
  #define STDLIB_H
  #include <stdlib.h>
#endif
#ifndef DUNGEON_H
  #define DUNGEON_H
  #include "dungeon.h"
#endif
#ifndef STDINT_H
  #define STDINT_H
  #include <stdint.h>
#endif
#ifndef STDLIB_H
  #define STDLIB_H
  #include <stdlib.h>
#endif

#include <stdio.h>

#define HEAP_OVERFLOW 1
#define ELEMENT_NOT_FOUND 2

typedef struct MinHeap {
  struct Character** arr;
  int size;
  int maxSize;
} MinHeap;

MinHeap* initHeap(int size);

int addToHeap(MinHeap* minHeap, Character* character);

Character* removeFromHeap(MinHeap* minHeap);

int bubbleUp(MinHeap* minHeap);

bool isHeapEmpty(MinHeap* heap);

bool isFull(MinHeap* heap);

int deleteFromHeap(MinHeap* heap, Character* toRemove);

int getIndexOfPC(MinHeap* heap);
