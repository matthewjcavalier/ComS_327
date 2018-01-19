#include "generator.h"

/**
 * @brief   Performs the dungeon generatation steps to 
 *          create a new dungeon from scratch
 * 
 * @return Dungeon* 
 */
Dungeon* genDungeon() {
  Dungeon* newDungeon;
  newDungeon = malloc(sizeof(Dungeon));

  zeroOutDungeon(newDungeon);
  
  addBorders(newDungeon);

  placeRooms(newDungeon);

  placeHallways(newDungeon);

  setHardnesses(newDungeon);

  return newDungeon;
}

/**
 * @brief   Cleans the dungeon and sets all tiles in the
 *          map to a starting state
 * 
 * @param dun 
 */
void zeroOutDungeon(Dungeon* dun) {
  int row, col;
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      dun->map[row][col].hardness = -1;
      dun->map[row][col].isBorder = false;
      dun->map[row][col].isHallway = false;
      dun->map[row][col].isRoom = false;
    }
  }
}

/**
 * @brief   Adds a border to the dungeon. Each tile in
 *          the border has its isBorder flag turned on.
 * 
 * @param dun   the dungeon being worked on
 */
void addBorders(Dungeon* dun) {
  int row, col;

  // add borders to the first row and last row
  for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
    dun->map[0][col].isBorder = true;
    dun->map[MAX_DUNGEON_HEIGHT - 1][col].isBorder = true;
  }
  
  // add borders to the first and last column
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    dun->map[row][0].isBorder = true;
    dun->map[row][MAX_DUNGEON_WIDTH - 1].isBorder = true;
  }
}

/**
 * @brief   Iterates throught he dungeon and sets the
 *          hardness of all tiles to something in the
 *          range 0 to 255.
 * @param dun   the dungeon being worked on
 */
void setHardnesses(Dungeon* dun) {
  int row, col;

  for(row = 1; row < MAX_DUNGEON_HEIGHT - 1; row++) {
      for(col = 1; col < MAX_DUNGEON_WIDTH - 1; col ++) {
        if(dun->map[row][col].isHallway == false && dun->map[row][col].isRoom == false) {
          dun->map[row][col].hardness = rand() % MAX_ROCK_HARDNESS + 1;
        }
      }
  }
}

/**
 * @brief   Places between 5 to 10 rooms in the dungeon.
 * 
 * @param dun   the dungeon being worked on
 */
void placeRooms(Dungeon* dun) {
  int roomNum, xLoc, yLoc; 
  int totalRoomsPlaced = 0;

  // set room sizes
  for(roomNum = 0; roomNum < EXPECTED_ROOM_COUNT; roomNum++) {
    dun->rooms[roomNum].height = MIN_ROOM_HEIGHT + (rand() % 10);
    dun->rooms[roomNum].width = MIN_ROOM_WIDTH + (rand() % 10);
  }

  while(totalRoomsPlaced <= MIN_ROOM_COUNT) {
    totalRoomsPlaced = 0;
    clearRooms(dun);
    for(roomNum = 0; roomNum < EXPECTED_ROOM_COUNT; roomNum++) {
      // minus 2 for the two borders, plus 1 to start at col 1
      xLoc = rand() % (MAX_DUNGEON_WIDTH - 2) + 1;
      // minus 2 for the two borders, plus 1 to start at row 1
      yLoc = rand() % (MAX_DUNGEON_HEIGHT - 2) + 1;
      
      if(isValidRoomPlacement(xLoc, yLoc, dun, dun->rooms[roomNum])) {
        dun->rooms[roomNum].xPos = xLoc;
        dun->rooms[roomNum].yPos = yLoc;
        
        addRoomToDungeon(dun, dun->rooms[roomNum]);
        totalRoomsPlaced++;
      } else {
        break;
      }
    }
  }
  dun->numRooms = totalRoomsPlaced;
}

/**
 * @brief   Cleans the dungeon rooms list as well as removes
 *          the placed rooms from the dungeon.
 * 
 * @param dun   the dungeon being worked on
 */
void clearRooms(Dungeon* dun) {
  int roomNum, row, col;
  // loop through and clear room coords
  for(roomNum = 0; roomNum < EXPECTED_ROOM_COUNT; roomNum++) {
    dun->rooms[roomNum].xPos = -1;
    dun->rooms[roomNum].yPos = -1;
  }

  // loop through and clear the dungeon map
  for(row = 1; row < MAX_DUNGEON_HEIGHT - 1; row++) {
    for(col = 1; col < MAX_DUNGEON_WIDTH - 1; col++) {
      dun->map[row][col].isRoom = false;
    }
  }
}

/**
 * @brief   Checks to see if placing the newRoom at the coordinates
 *          violates any room placement rules.
 * 
 * @param xLoc      the x coordinate for the new room
 * @param yLoc      the y coordinate for the new room 
 * @param dun       a pointer to the dungeon being worked on 
 * @param newRoom   the new room that is looking to be placed
 * @return boolean  if the placment is valid true is returned, else
 *                  false is returned
 */
boolean isValidRoomPlacement(int xLoc, int yLoc, Dungeon* dun, Room newRoom) {
  int col, row;
  boolean ret = true;
  for(row = yLoc - 1; row < yLoc + newRoom.height + 1; row++) {
    for(col = xLoc - 1; col < xLoc + newRoom.width + 1; col++) {
      if(dun->map[row][col].isRoom || dun->map[row][col].isBorder) {
        ret = false;
        break;
      }
    }
  }
  return ret;
}

/**
 * @brief   Adds the new room to the dungeon.
 * 
 * @param dun       the dungeon being worked on
 * @param newRoom   the new room that needs placement
 */
void addRoomToDungeon(Dungeon* dun, Room newRoom) {
  int row, col;
  for(row = newRoom.yPos; row < newRoom.height + newRoom.yPos; row++) {
    for(col = newRoom.xPos; col < newRoom.width + newRoom.xPos; col++) {
      dun->map[row][col].isRoom = true;
    }
  }
}

void placeHallways(Dungeon* dun) {
  int xPos1, xPos2, yPos1, yPos2, roomNum;
  
  for(roomNum = 0; roomNum < dun->numRooms - 1; roomNum++) {
    xPos1 = dun->rooms[roomNum].xPos;
    yPos1 = dun->rooms[roomNum].yPos;

    xPos2 = dun->rooms[roomNum + 1].xPos;
    yPos2 = dun->rooms[roomNum + 1].yPos;
    
    makePathToRoom(yPos1, xPos1, yPos2, xPos2, dun);
  }
  
}

/**
 * @brief   Places hallways in the dungeon ensuring that
 *          all rooms are connected in some fashion.
 * 
 * @param dun   the dungeon being worked on
 */
void makePathToRoom(int row1, int col1, int row2, int col2, Dungeon* dun){
  int xMult, yMult;

  xMult = (col1 > col2) ? -1 : 1;

  yMult = (row1 > row2) ? -1 : 1;

  while(row1 != row2 || col1 != col2) {
    // if even work on row
    if(rand() % 2 == 0) {
      if(row1 != row2) {
        row1 += yMult;
      }
    } else if(col1 != col2) {
      col1 += xMult;
    }
    placeHallTile(row1, col1, dun);
  }
}

/**
 * @brief   attempts to place a halltile at the givine coordinate,
 *          if there is found to be a room tile already present
 *          at that coordinate, then the tile is not updated
 * 
 * @param row   the y location for the coordinate pair
 * @param col   the x locaiton for the coordinate pair
 * @param dun   the dungeon being worked on
 */
void placeHallTile(int row, int col, Dungeon* dun) {
  if(!dun->map[row][col].isRoom) {
    dun->map[row][col].isHallway = true;
  }
}
