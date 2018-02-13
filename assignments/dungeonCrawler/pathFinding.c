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
  
  fillMap(dun, map, queue, false);
  
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
  
  fillMap(dun, map, queue, true); 
  //dijkstra(dun, map);

  free(queue);
  return map;
}

void dijkstra(Dungeon* dun, int** map) {
  int** shortestPathBools = initMap();
  int row, col;
  int count;
  Coordinate currentCoord;
  Coordinate upLeft;
  Coordinate up;
  Coordinate upRight;
  Coordinate right;
  Coordinate downRight;
  Coordinate down;
  Coordinate downLeft;
  Coordinate left;

  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      shortestPathBools[row][col] = 0;
    }
  }

  for(count = 0; count < MAX_DUNGEON_WIDTH * MAX_DUNGEON_HEIGHT; count++) {
    currentCoord = minDist(map, shortestPathBools);

    shortestPathBools[currentCoord.row][currentCoord.col] = 1;

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

    if(isValidCoord(upLeft) && shortestPathBools[upLeft.row][upLeft.col] == 0 &&
            map[currentCoord.row][currentCoord.col] != INT_MAX &&
            (1 + dun->map[currentCoord.row][currentCoord.col].hardness / 85 + map[currentCoord.row][currentCoord.col] < map[upLeft.row][upLeft.col])) {
      map[upLeft.row][upLeft.col] = 1 + dun->map[currentCoord.row][currentCoord.col].hardness / 85 + map[currentCoord.row][currentCoord.col];
    }
    if(isValidCoord(up) && shortestPathBools[up.row][up.col] == 0 &&
            map[currentCoord.row][currentCoord.col] != INT_MAX &&
            (1 + dun->map[currentCoord.row][currentCoord.col].hardness / 85 + map[currentCoord.row][currentCoord.col] < map[up.row][up.col])) {
      map[up.row][up.col] = 1 + dun->map[currentCoord.row][currentCoord.col].hardness / 85 + map[currentCoord.row][currentCoord.col];
    }
    if(isValidCoord(upRight) && shortestPathBools[upRight.row][upRight.col] == 0 &&
            map[currentCoord.row][currentCoord.col] != INT_MAX &&
            (1 + dun->map[currentCoord.row][currentCoord.col].hardness / 85 + map[currentCoord.row][currentCoord.col] < map[upRight.row][upRight.col])) {
      map[upRight.row][upRight.col] = 1 + dun->map[currentCoord.row][currentCoord.col].hardness / 85 + map[currentCoord.row][currentCoord.col];
    }
    if(isValidCoord(right) && shortestPathBools[right.row][right.col] == 0 &&
            map[currentCoord.row][currentCoord.col] != INT_MAX &&
            (1 + dun->map[currentCoord.row][currentCoord.col].hardness / 85 + map[currentCoord.row][currentCoord.col] < map[right.row][right.col])) {
      map[right.row][right.col] = 1 + dun->map[currentCoord.row][currentCoord.col].hardness / 85 + map[currentCoord.row][currentCoord.col];
    }
    if(isValidCoord(downRight) && shortestPathBools[downRight.row][downRight.col] == 0 &&
            map[currentCoord.row][currentCoord.col] != INT_MAX &&
            (1 + dun->map[currentCoord.row][currentCoord.col].hardness / 85 + map[currentCoord.row][currentCoord.col] < map[downRight.row][downRight.col])) {
      map[downRight.row][downRight.col] = 1 + dun->map[currentCoord.row][currentCoord.col].hardness / 85 + map[currentCoord.row][currentCoord.col];
    }
    if(isValidCoord(down) && shortestPathBools[down.row][down.col] == 0 &&
            map[currentCoord.row][currentCoord.col] != INT_MAX &&
            (1 + dun->map[currentCoord.row][currentCoord.col].hardness / 85 + map[currentCoord.row][currentCoord.col] < map[down.row][down.col])) {
      map[down.row][down.col] = 1 + dun->map[currentCoord.row][currentCoord.col].hardness / 85 + map[currentCoord.row][currentCoord.col];
    }
    if(isValidCoord(downLeft) && shortestPathBools[downLeft.row][downLeft.col] == 0 &&
            map[currentCoord.row][currentCoord.col] != INT_MAX &&
            (1 + dun->map[currentCoord.row][currentCoord.col].hardness / 85 + map[currentCoord.row][currentCoord.col] < map[downLeft.row][downLeft.col])) {
      map[downLeft.row][downLeft.col] = 1 + dun->map[currentCoord.row][currentCoord.col].hardness / 85 + map[currentCoord.row][currentCoord.col];
    }
    if(isValidCoord(left) && shortestPathBools[left.row][left.col] == 0 &&
            map[currentCoord.row][currentCoord.col] != INT_MAX &&
            (1 + dun->map[currentCoord.row][currentCoord.col].hardness / 85 + map[currentCoord.row][currentCoord.col] < map[left.row][left.col])) {
      map[left.row][left.col] = 1 + dun->map[currentCoord.row][currentCoord.col].hardness / 85 + map[currentCoord.row][currentCoord.col];
    }

  }
}

boolean isValidCoord(Coordinate coord) {
  if(coord.row > 0 && coord.row < MAX_DUNGEON_HEIGHT && coord.col > 0 && coord.col < MAX_DUNGEON_WIDTH) {
    return true;
  }
  return false;
}

Coordinate minDist(int** map, int** shortestPathBools) {
    int min = INT_MAX;
    int row, col;
    Coordinate min_coord;
    for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
      for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
        if(shortestPathBools[row][col] == 0 && map[row][col] <= min) {
          min = map[row][col];
          min_coord.row = row;
          min_coord.col = col;
        }
      }
    }
    return min_coord;
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
void fillMap(Dungeon* dun, int** map, CoordQueue* queue, boolean canBurrow) {
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
void fillLogic(Dungeon* dun, int** map, CoordQueue* queue, boolean canBurrow, Coordinate neighbor, Coordinate current) {
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
