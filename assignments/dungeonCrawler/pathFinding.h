#ifndef DUNGEON_H
  #define DUNGEON_H
  #include "dungeon.h"
#endif
#ifndef STDLIB_H
  #define STDLIB_H
  #include <stdlib.h>
#endif
#ifndef LIMITS_H
  #define LIMITS_H
  #include<limits.h>
#endif
#ifndef QUEUE_H
  #define QUEUE_H
  #include "queue.h"
#endif

/**
 * @brief   Generates a distance map starting from
 *          the input coordinates and reaching out to
 *          every part of the dungeon, only moving through
 *          the rooms and hallways
 * 
 * @param row   the row that the player character is at
 * @param col   the column that the player character is at
 * @param dun   the dungeon that the player character is in
 * @return int** the array that has the distances
 */
int** getPathMapOnlyOpenArea(Coordinate* cord, Dungeon* dun);

/**
 * @brief     Generates a distance map starting from
 *            the input coordinates and reaching out to
 *            every part of the dungeon, moving through
 *            everywhere including rock
 * 
 * @param row   the row that the player character is at
 * @param col   the column that the player character is at
 * @param dun   the dungeon that the player character is in
 * @return int** the array that has the distances
 */
int** getPathMapEverywhere(Coordinate* cord, Dungeon* dun);

/**
 * @brief           An application of Dijkstra's algorithm
 *                  filling out the distance map
 * 
 * @param dun       The dungeon that is being worked on
 * @param map       The map being filled out
 * @param queue     The queue of the nodes in the map to visit
 * @param canBurrow If the map is being generated with the ability
 *                  to burrow through rock
 */
void fillMap(Dungeon* dun, int** map, CoordQueue* queue, bool canBurrow);

/**
 * @brief           The logic to decide if the value at certain
 *                  map locations needs to be updated and if
 *                  a map location should be added to the queue
 * 
 * @param dun       The dungeon that is being worked on
 * @param map       The map being filled out
 * @param queue     The queue of the nodes in the map to visit
 * @param canBurrow If the map is being generated with the ability
 *                  to burrow through rock
 * @param neighbor  The neighbor vertex that we are currently looking
 *                  at
 * @param current   The current node that is bing looked at
 */
void fillLogic(Dungeon* dun, int** map, CoordQueue* queue, bool canBurrow, Coordinate neighbor, Coordinate current);
/**
 * @brief   Allocates memory for a map and returns a pointer to it.
 * 
 * @return int**     a pointer to the map
 */

int** genMap(Coordinate* cord, Dungeon* dun, int** map, bool onlyOpenAreas);

/**
 * @brief Initializes a map that is MAX_DUNGEON_HEIGHT BY MAX_DUNGEON_WIDTH
 *  the reference to the map
 * @return int** 
 */
int** initMap();

