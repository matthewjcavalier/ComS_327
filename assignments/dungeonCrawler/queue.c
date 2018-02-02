#include "queue.h"

CoordQueue* initCoordQueue() {
  CoordQueue* queue;
  queue = malloc(sizeof(*queue));
  queue->front = NULL;
  queue->back = NULL;
  queue->size = 0;
  return queue;
}

void enqueue(CoordQueue* queue, Coordinate coord) {
  CoordQueueNode* node = (CoordQueueNode*)malloc(sizeof(*node));
  node->coord = coord;

  if(isEmpty(queue)) {
    queue->front = node;
    queue->back = node;
  } else {
    node->ahead = queue->back;
    queue->back->behind = node;
    queue->back = node;
  }
  queue->size++;
}

void dequeue(CoordQueue* queue, Coordinate* coord) {
  CoordQueueNode* node = queue->front;
  queue->front = queue->front->behind;
  queue->size--;
  *coord = node->coord;
  free(node);
}

boolean isEmpty(CoordQueue* queue) {
  if(queue->size == 0) {
    return true;
  }
  return false;
}

void peak(CoordQueue* queue, Coordinate* coord) {
  Coordinate temp;
  temp = queue->front->coord;
  *coord = temp;
}

/*
void main() {
  CoordQueue* q;
  Coordinate* coord = malloc(sizeof(coord));
  int count;

  q = initCoordQueue();

  for(count = 0; count < 10; count++) {
    coord->row = count;
    coord->col = count;
    enqueue(q, *coord);
  }
  while(!isEmpty(q)) {
    peak(q, coord);
    printf("peaking\t\t row: %d, col: %d\n", coord->row, coord->col);
    
    dequeue(q, coord);
    printf("dequeueing\t row: %d, col: %d\n", coord->row, coord->col);
  }
}
*/
