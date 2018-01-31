#include "list.h"

/**
 * @brief Initalizes a new generic list and returns it
 * 
 * @return List* a pointer to the new list
 */
List* initList() {
  List* newList;
  newList = malloc(sizeof(*newList));
  newList->head = NULL;
  newList->tail = NULL;
  newList->length = 0;

  return newList;
}

/**
 * @brief   creates a new data node
 * 
 * @param dataPtr  a pointer to the data that will be stored in the node
 * @return Node*   a pointer to the new node
 */
Node* makeNode(void* dataPtr) {
  Node* newNode = malloc(sizeof(*newNode));

  newNode->dataPtr = dataPtr;
  newNode->next = NULL;
  newNode->prev = NULL;

  return newNode;
}

/**
 * @brief   adds new data to the list
 * 
 * @param dataPtr   a pointer to the data being stored
 * @param list      a pointer to the list being added to
 */
void listAdd(void* dataPtr, List* list) {
  Node* newNode = makeNode(dataPtr);

  if(list->length == 0) {
    list->head = newNode;
    list->tail = newNode;
  } else {
    newNode->prev = list->tail;
    list->tail->next = newNode;
    list->tail = newNode;
  }
  list->length++;
}

/**
 * @brief removes the data node at the given index of the list
 * 
 * @param index   where the node to be removed is at
 * @param list    the list being updated
 */
void listRemove(int index, List* list) {
  Node* current;
  int i;

  if(index >= list->length || index < 0) {
    exit(INVALID_INDEX_EXCEPTION);
  }

  current = list->head;
  if(list->length == 1) {
    list->head = NULL;
    list->tail = NULL;
  } else {
    for(i = 0; i < index; i++) {
      current = current->next;
    }

    if(index > 0) {
      current->prev->next = current->next;
    } else {
      list->head = current->next;
    }

    if(index < list->length - 1) {
      current->next->prev = current->prev;
    } else {
      list->tail = current->prev;
    }
  }
  free(current->dataPtr);
  free(current);
  list->length--;
}
