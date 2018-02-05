#include "queue.h"

/**
 * @brief   initializes the queue
 * 
 * @return CoordQueue*  a reference to the new queue
 */
CoordQueue* initCoordQueue() {
  CoordQueue* queue;
  queue = malloc(sizeof(*queue));
  queue->front = NULL;
  queue->back = NULL;
  queue->size = 0;
  return queue;
}

/**
 * @brief Addes a new coord to the queue
 * 
 * @param queue   the queue bieng appended to
 * @param coord   the coord being added
 */
void enqueue(CoordQueue* queue, Coordinate coord) {
  CoordQueueNode* node = malloc(sizeof(*node));
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

/**
 * @brief Removes a node from the front of the queue
 * 
 * @param queue   the queue being worked on
 * @param coord   a pointer that will store the removed
 *                node value
 */
void dequeue(CoordQueue* queue, Coordinate* coord) {
  CoordQueueNode* node = queue->front;
  queue->front = queue->front->behind;
  queue->size--;
  *coord = node->coord;
  free(node);
}

/**
 * @brief returns a boolean indicateing if the queue is
 *        empty or not
 *  
 * @param queue     the queue being checked
 * @return boolean  if the queue is empty
 */
boolean isEmpty(CoordQueue* queue) {
  if(queue->size == 0) {
    return true;
  }
  return false;
}

/**
 * @brief   Peaks at the value stored in the front of the queue
 * 
 * @param queue    the queue being looked at
 * @param coord    a pointer to where the value will be stored
 */
void peak(CoordQueue* queue, Coordinate* coord) {
  Coordinate temp;
  temp = queue->front->coord;
  *coord = temp;
}