#include <stdio.h>
#include <stdlib.h>
#include "dungeon.h"
#include <stdint.h>
#include <stdlib.h>

#define HEAP_OVERFLOW 1;
/*
typedef struct HeapNode {
  void* dataPtr;
  struct Node* parent;
  struct Node* leftChild;
  struct Node* rightChild;
} HeapNode;

typedef struct Heap {
  struct HeapNode* top;
  int size;
} Heap;
*/

typedef struct MinHeap {
  struct Character** arr;
  int size;
  int maxSize;
} MinHeap;

/**
 * NOTE: could possibly represent the heap as an array, and in the init just say how many member will be in the
 *       heap, since there are supposedly a static number of monsters
 */

MinHeap* initHeap(int size);

int addToHeap(MinHeap* minHeap, Character* character);

Character* removeFromHeap(MinHeap* minHeap);

int bubbleUp(MinHeap* minHeap);

boolean isHeapEmpty(MinHeap* heap);

boolean isFull(MinHeap* heap);

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


/*
int addToHeap(MinHeap* heap, void* data, int (*compare)(void*, void*)) {
  HeapNode* newNode = malloc(sizeof(*newNode));
  HeapNode* current;
  boolean newNodePlaced = false;
  boolean currentIsLeftChild = false;

  newNode.parent = NULL;
  newNode.leftChild = NULL;
  newNode.rightChild = NULL;
  newNode.data = data;

  if(isHeapEmpty(heap) {
    heap.top = newNode;
  } else {
    do {
      current = heap->top;
      // if the new node's value is less than the current node's data
      if(compare(newNode->data, current->data) <= 0) {
        newNode->leftChild = current;
        newNode->parent = current->parent;
        
        // if the current node has a parent
        if(current->parent != NULL) {
          if(isLeftChild(current)) {
            current->parent->leftChild = newNode;
          } else {
            current->parent->rightChild = newNode;
          }
        }
      } else {
        
      }
    } while (!newNodePlaced);
  }
  heap->size++;
  return 0;
}
boolean isLeftChild(MinHeapNode* node) {
  return false;
}

int removeFromHeap(MinHeap*, void* retLoc, int(*compare)(void*, void*)) {

  return 0;
}

boolean isHeapEmpty(MinHeap* heap) {
  if(heap->size = 0) {
    return true;
  }
  return false;
}
*/
