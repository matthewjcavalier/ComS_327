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

void fillMap(Dungeon* dun, int** map, CoordQueue* queue, boolean canBurrow);

void fillLogic(Dungeon* dun, int** map, CoordQueue* queue, boolean canBurrow, Coordinate neighbor, Coordinate current);
/**
 * @brief   Allocates memory for a map and returns a pointer to it.
 * 
 * @return int**     a pointer to the map
 */

int** genMap(Coordinate* cord, Dungeon* dun, int** map, boolean onlyOpenAreas);
int** initMap();
