#ifndef DUNGEON_H
  #define DUNGEON_H
  #include "dungeon.h"
#endif
#ifndef STDLIB_H
  #define STDLIB_H
  #include <stdlib.h>
#endif
#ifndef STDIO_H
  #define STDIO_H
  #include <stdio.h>
#endif

typedef struct CoordQueueNode {
  struct Coordinate coord;
  struct CoordQueueNode* ahead;
  struct CoordQueueNode* behind;
} CoordQueueNode;

typedef struct CoordQueue {
  struct CoordQueueNode* front;
  struct CoordQueueNode* back;
  int size;
} CoordQueue;

CoordQueue* initCoordQueue();

void enqueue(CoordQueue* queue, Coordinate coord);

void dequeue(CoordQueue* queue, Coordinate* coord);

boolean isEmpty(CoordQueue* queue);

void peak(CoordQueue* queue, Coordinate* coord);
