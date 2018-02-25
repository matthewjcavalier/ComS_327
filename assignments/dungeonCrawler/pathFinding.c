#ifndef PATHFINDING_H
  #define PATHFINDING_H
  #include "pathFinding.h"
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
int** getPathMapOnlyOpenArea(Coordinate* pc, Dungeon* dun){
  int** map = initMap();
  CoordQueue* queue = initCoordQueue();

  // put the pc in the dungeon
  map[pc->row][pc->col] = 0;
  enqueue(queue, *pc);
  
  fillMap(dun, map, queue, FALSE);
  
  free(queue);
  
  return map;
}

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
int** getPathMapEverywhere(Coordinate* pc, Dungeon* dun) {
  int** map = initMap();
  CoordQueue* queue = initCoordQueue();
 
   // put the pc in the dungeon
  map[pc->row][pc->col] = 0;
  enqueue(queue, *pc);
  
  fillMap(dun, map, queue, TRUE); 

  free(queue);
  return map;
}

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
void fillMap(Dungeon* dun, int** map, CoordQueue* queue, bool canBurrow) {
  Coordinate currentCoord;
  Coordinate upLeft;
  Coordinate up;
  Coordinate upRight;
  Coordinate right;
  Coordinate downRight;
  Coordinate down;
  Coordinate downLeft;
  Coordinate left;

  while(!isEmpty(queue)) {
    // dequeue first coord
    dequeue(queue, &currentCoord);
    
    upLeft.row = currentCoord.row -1;
    upLeft.col = currentCoord.col -1;

    up.row = currentCoord.row -1;
    up.col = currentCoord.col;

    upRight.row = currentCoord.row -1;
    upRight.col = currentCoord.col +1;

    right.row = currentCoord.row;
    right.col = currentCoord.col +1;

    downRight.row = currentCoord.row +1;
    downRight.col = currentCoord.col +1;

    down.row = currentCoord.row + 1;
    down.col = currentCoord.col;

    downLeft.row = currentCoord.row +1;
    downLeft.col = currentCoord.col -1;

    left.row = currentCoord.row;
    left.col = currentCoord.col -1;

    
    // for every neighbor of the coord
    fillLogic(dun, map, queue, canBurrow, upLeft, currentCoord); 
    fillLogic(dun, map, queue, canBurrow, up, currentCoord); 
    fillLogic(dun, map, queue, canBurrow, upRight, currentCoord); 
    fillLogic(dun, map, queue, canBurrow, right, currentCoord); 
    fillLogic(dun, map, queue, canBurrow, downRight, currentCoord); 
    fillLogic(dun, map, queue, canBurrow, down, currentCoord); 
    fillLogic(dun, map, queue, canBurrow, downLeft, currentCoord); 
    fillLogic(dun, map, queue, canBurrow, left, currentCoord); 
  }
}

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
void fillLogic(Dungeon* dun, int** map, CoordQueue* queue, bool canBurrow, Coordinate neighbor, Coordinate current) {
  int currentMapVal = map[current.row][current.col];
  int neighborMapVal = map[neighbor.row][neighbor.col];
  Tile currentTile = dun->map[current.row][current.col];
  Tile neighborTile = dun->map[neighbor.row][neighbor.col];
  int distance = 1;

  if(!neighborTile.isBorder) {
    if(neighborTile.hardness == 0) {
      if(neighborMapVal > currentMapVal + distance) {
        map[neighbor.row][neighbor.col] = currentMapVal + distance;
        enqueue(queue, neighbor);
      }
    } else if(canBurrow) {
      if(neighborMapVal > currentMapVal + (currentTile.hardness / 85) + distance) {
        map[neighbor.row][neighbor.col] = currentMapVal + (currentTile.hardness / 85) + distance;
        enqueue(queue, neighbor);
      }
    }
  }
}

/**
 * @brief Initializes a map that is MAX_DUNGEON_HEIGHT BY MAX_DUNGEON_WIDTH
 * * @return int**  the reference to the map
 */
int** initMap() {
  int** newMap = malloc(MAX_DUNGEON_HEIGHT * sizeof(int*));
  int row, col;

  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    newMap[row] = malloc(MAX_DUNGEON_WIDTH * sizeof(int));
  }
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      newMap[row][col] = INT_MAX;
    }
  }
  return newMap;
};
