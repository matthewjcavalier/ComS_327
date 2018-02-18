#include "heap.h"
/*
int main() {
  MinHeap* heap = initHeap(8);
  Character* newChar;
  Character* currChar; 
  Coordinate coord;
  
  for(int i = 0; i < heap->maxSize; i++) {
    newChar = malloc(sizeof(*newChar));
    coord.row = i;
    coord.col = i;
    newChar->coord = coord;
    newChar->symbol = 'M';
    newChar->nextEventTime = rand() % 15 + 5;
    newChar->speed = 15;
    printf("%d\n", newChar->speed);
    addToHeap(heap, newChar);
  }
  printf("Removing stuff now\n");
  while(!isHeapEmpty(heap)) {
    currChar = removeFromHeap(heap);
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
  heap->size--;
  bubbleUp(heap);
  return character;
}

int bubbleUp(MinHeap* minHeap) {
  boolean swapsPerformed = true;
  Character* swapper;
  Character* parent;
  Character* current;
  int index;
  
  while(swapsPerformed) {
    swapsPerformed = false;
    // start at last element
    for(index = minHeap->size - 1; index >= 0; index--) {
      current = minHeap->arr[index];
      parent = minHeap->arr[(index - 1) / 2];
      if(parent->nextEventTime > current->nextEventTime) {
        swapsPerformed = true;
        swapper = parent;
        minHeap->arr[(index -1) / 2] = current;
        minHeap->arr[index] = swapper;
      }
    }
  }
  return 0;
}

boolean isHeapEmpty(MinHeap* heap) {
  if(heap->size == 0) {
    return true;
  }
  return false;
}

boolean isFull(MinHeap* heap) {
  if(heap->size == heap->maxSize) {
    return true;
  }
  return false;
}
