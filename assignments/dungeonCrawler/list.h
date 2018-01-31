#ifndef STDLIB_H
  #define STDLIB_H
  #include <stdlib.h>
#endif
#ifndef STDIO_H
  #define STDIO_H
  #include <stdio.h>
#endif

#define INVALID_INDEX_EXCEPTION 1

typedef struct Node{
  void* dataPtr;
  struct Node* next;
  struct Node* prev;
} Node;

typedef struct List{
  struct Node* head;
  struct Node* tail;
  int length;
} List;

/**
 * @brief Initalizes a new generic list and returns it
 * 
 * @return List* a pointer to the new list
 */
List* initList();

/**
 * @brief   creates a new data node
 * 
 * @param dataPtr  a pointer to the data that will be stored in the node
 * @return Node*   a pointer to the new node
 */
Node* makeNode(void* dataPtr);

/**
 * @brief   adds new data to the list
 * 
 * @param dataPtr   a pointer to the data being stored
 * @param list      a pointer to the list being added to
 */
void listAdd(void* dataPtr, List* list);

/**
 * @brief removes the data node at the given index of the list
 * 
 * @param index   where the node to be removed is at
 * @param list    the list being updated
 */
void listRemove(int index, List* list);
