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

/**
 * @brief   initializes the queue
 * 
 * @return CoordQueue*  a reference to the new queue
 */
CoordQueue* initCoordQueue();

/**
 * @brief Addes a new coord to the queue
 * 
 * @param queue   the queue bieng appended to
 * @param coord   the coord being added
 */
void enqueue(CoordQueue* queue, Coordinate coord);

/**
 * @brief Removes a node from the front of the queue
 * 
 * @param queue   the queue being worked on
 * @param coord   a pointer that will store the removed
 *                node value
 */
void dequeue(CoordQueue* queue, Coordinate* coord);

/**
 * @brief returns a boolean indicateing if the queue is
 *        empty or not
 *  
 * @param queue     the queue being checked
 * @return boolean  if the queue is empty
 */
boolean isEmpty(CoordQueue* queue);

/**
 * @brief   Peaks at the value stored in the front of the queue
 * 
 * @param queue    the queue being looked at
 * @param coord    a pointer to where the value will be stored
 */
void peak(CoordQueue* queue, Coordinate* coord);
