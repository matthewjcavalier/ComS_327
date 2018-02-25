#include "heap.h"
/*
int main() {
  MinHeap* turnQueue = initHeap(4);
  Character* newChar;
  Character* currChar; 
  Coordinate coord;
  
  for(int i = 0; i < turnQueue->maxSize; i++) {
    newChar = malloc(sizeof(*newChar));
    coord.row = i;
    coord.col = i;
    newChar->coord = coord;
    newChar->symbol = 'M';
    newChar->nextEventTime = rand() % 15 + 5;
    newChar->speed = 15;
    printf("%d\n", newChar->speed);
    addToHeap(turnQueue, newChar);
  }
  newChar = malloc(sizeof(*newChar));
  coord.row = 3;
  coord.col = 3;
  newChar->coord = coord;
  newChar->symbol = 'M';
  newChar->nextEventTime = rand() % 15 + 5;
  newChar->speed = 15;

  deleteFromHeap(turnQueue, newChar);

  printf("Removing stuff now\n");
  while(!isHeapEmpty(turnQueue)) {
    currChar = removeFromHeap(turnQueue);
    printf("%d\n", currChar->nextEventTime);
  }
  return 0;
}
*/

MinHeap* initHeap(int size) {
  MinHeap* minHeap = malloc(sizeof(*minHeap));

  minHeap->maxSize = size;
  minHeap->size = 0;
  minHeap->arr = malloc(sizeof(Character) * size);
  
  return minHeap;
}

int addToHeap(MinHeap* minHeap, Character* character) {

  if(isFull(minHeap)) {
    // error occurred
    return HEAP_OVERFLOW;
  }
  
  minHeap->arr[minHeap->size] = character;

  minHeap->size++;

  bubbleUp(minHeap);

  // bubble up to the correct position

  return 0;
}

Character* removeFromHeap(MinHeap* heap) {
  Character* character = heap->arr[0];
  heap->arr[0] = heap->arr[heap->size -1];
  heap->arr[heap->size -1] = NULL;
  heap->size--;
  bubbleUp(heap);
  return character;
}

int deleteFromHeap(MinHeap* heap, Character* toRemove) {
  int index;
  for(index = 0; index < heap->size; index++) {
    if(heap->arr[index]->coord.row == toRemove->coord.row && heap->arr[index]->coord.col == toRemove->coord.col) {
      heap->arr[index] = heap->arr[heap->size -1];
      heap->arr[heap->size -1] = NULL;
      heap->size--;
      bubbleUp(heap);
      return 0;
    }
  }
  return ELEMENT_NOT_FOUND;
}

int getIndexOfPC(MinHeap* heap) {
  int index;
  for(index = 0; index < heap->size; index++) {
    if(heap->arr[index]->pc != NULL) {
      return index;
    }
  }
  return -1;
}

int bubbleUp(MinHeap* minHeap) {
  bool swapsPerformed = TRUE;
  Character* swapper;
  Character* parent;
  Character* current;
  int index;
  
  while(swapsPerformed) {
    swapsPerformed = FALSE;
    // start at last element
    for(index = minHeap->size - 1; index >= 0; index--) {
      current = minHeap->arr[index];
      parent = minHeap->arr[(index - 1) / 2];
      if(parent->nextEventTime > current->nextEventTime) {
        swapsPerformed = TRUE;
        swapper = parent;
        minHeap->arr[(index -1) / 2] = current;
        minHeap->arr[index] = swapper;
      }
    }
  }
  return 0;
}

bool isHeapEmpty(MinHeap* heap) {
  if(heap->size == 0) {
    return TRUE;
  }
  return FALSE;
}

bool isFull(MinHeap* heap) {
  if(heap->size == heap->maxSize) {
    return TRUE;
  }
  return FALSE;
}
