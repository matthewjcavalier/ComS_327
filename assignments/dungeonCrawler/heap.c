#include "heap.h"

/**
 * @brief initializes a heap and returns a pointer to it
 * 
 * @param size    the max size of the heap
 * @return MinHeap*   the pointer to the new heap
 */
MinHeap* initHeap(int size) {
  MinHeap* minHeap = malloc(sizeof(*minHeap));

  minHeap->maxSize = size;
  minHeap->size = 0;
  minHeap->arr = malloc(sizeof(Character) * size);
  
  return minHeap;
}

/*
 * @brief Add a character to the heap
 * 
 * @param minHeap   the heap being added to 
 * @param character   the character being added
 * @return int      status indicator
 */
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
/**
 * @brief remove and return a pointer to the character at the top of the heap
 * 
 * @param heap    heap being worked on  
 * @return Character*   the character that was at the top of the heap
 */
Character* removeFromHeap(MinHeap* heap) {
  Character* character = heap->arr[0];
  heap->arr[0] = heap->arr[heap->size -1];
  heap->arr[heap->size -1] = NULL;
  heap->size--;
  bubbleUp(heap);
  return character;
}

/**
 * @brief 
 * 
 * @param heap 
 * @param toRemove 
 * @return int 
 */
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

/**
 * @brief gets the index of the PC in the heap
 * 
 * @param heap 
 * @return int  index of the pc
 */
int getIndexOfPC(MinHeap* heap) {
  int index;
  for(index = 0; index < heap->size; index++) {
    if(heap->arr[index]->pc != NULL) {
      return index;
    }
  }
  return -1;
}

/**
 * @brief bubble up the heap until no swaps are made to make the heap a proper min-heap
 * 
 * @param minHeap 
 * @return int 
 */
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

/**
 * @brief returns if the heap is empty
 * 
 * @param heap 
 * @return true 
 * @return false 
 */
bool isHeapEmpty(MinHeap* heap) {
  if(heap->size == 0) {
    return TRUE;
  }
  return FALSE;
}

/**
 * @brief returns if the heap is full
 * 
 * @param heap 
 * @return true 
 * @return false 
 */
bool isFull(MinHeap* heap) {
  if(heap->size == heap->maxSize) {
    return TRUE;
  }
  return FALSE;
}
