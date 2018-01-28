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


List* initList();

Node* makeNode(void* dataPtr);

void listAdd(void* dataPtr, List* list);

void listRemove(int index, List* list);