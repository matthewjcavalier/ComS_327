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

#define HEAP_OVERFLOW 1;

typedef struct MinHeap {
  struct Character** arr;
  int size;
  int maxSize;
} MinHeap;

MinHeap* initHeap(int size);

int addToHeap(MinHeap* minHeap, Character* character);

Character* removeFromHeap(MinHeap* minHeap);

int bubbleUp(MinHeap* minHeap);

boolean isHeapEmpty(MinHeap* heap);

boolean isFull(MinHeap* heap);