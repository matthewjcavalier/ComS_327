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

  initDungeon(newDungeon);
  
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
void initDungeon(Dungeon* dun) {
  int row, col;
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      dun->map[row][col].hardness = -1;
      dun->map[row][col].isBorder = false;
      dun->map[row][col].isHallway = false;
      dun->map[row][col].isRoom = false;
    }
  }
  dun->rooms = initList();
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
    dun->map[0][col].isRoom = false;
    dun->map[0][col].isHallway = false;
    dun->map[0][col].hardness = MAX_ROCK_HARDNESS;

    dun->map[MAX_DUNGEON_HEIGHT - 1][col].isBorder = true;
    dun->map[MAX_DUNGEON_HEIGHT - 1][col].isRoom = false;
    dun->map[MAX_DUNGEON_HEIGHT - 1][col].isHallway = false;
    dun->map[MAX_DUNGEON_HEIGHT - 1][col].hardness = MAX_ROCK_HARDNESS;
  }
  
  // add borders to the first and last column
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    dun->map[row][0].isBorder = true;
    dun->map[row][0].isRoom = false;
    dun->map[row][0].isHallway = false;
    dun->map[row][0].hardness = MAX_ROCK_HARDNESS;

    dun->map[row][MAX_DUNGEON_WIDTH - 1].isBorder = true;
    dun->map[row][MAX_DUNGEON_WIDTH - 1].isRoom = false;
    dun->map[row][MAX_DUNGEON_WIDTH - 1].isHallway = false;
    dun->map[row][MAX_DUNGEON_WIDTH - 1].hardness = MAX_ROCK_HARDNESS;
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
  Room* newRoom;

  while(dun->rooms->length <= MIN_ROOM_COUNT) {
    clearRooms(dun);

    for(roomNum = 0; roomNum < EXPECTED_ROOM_COUNT; roomNum++) {
      newRoom = malloc(sizeof(*newRoom));
      newRoom->height = MIN_ROOM_HEIGHT + (rand() % 10);
      newRoom->width = MIN_ROOM_WIDTH + (rand() % 10);

      // minus 2 for the two borders, plus 1 to start at col 1
      xLoc = rand() % (MAX_DUNGEON_WIDTH - 2) + 1;
      // minus 2 for the two borders, plus 1 to start at row 1
      yLoc = rand() % (MAX_DUNGEON_HEIGHT - 2) + 1;
      
      if(isValidRoomPlacement(xLoc, yLoc, dun, *newRoom)) {
        newRoom->xPos = xLoc;
        newRoom->yPos = yLoc;
        
        addRoomToDungeon(dun, *newRoom);
        listAdd(newRoom, dun->rooms);
      } else {
        free(newRoom);
        break;
      }
    }
  }
}

/**
 * @brief   Cleans the dungeon rooms list as well as removes
 *          the placed rooms from the dungeon.
 * 
 * @param dun   the dungeon being worked on
 */
void clearRooms(Dungeon* dun) {
  int row, col;
  // loop through and free the mem that each room took up in the list
  while(dun->rooms->length > 0) {
    listRemove(0, dun->rooms);
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
      if(row >= MAX_DUNGEON_HEIGHT || row < 0 || col >= MAX_DUNGEON_WIDTH || col < 0 || dun->map[row][col].isRoom == true || dun->map[row][col].isBorder == true) {
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
      dun->map[row][col].isHallway = false;
      dun->map[row][col].isBorder = false;
      dun->map[row][col].hardness = MIN_ROCK_HARDNESS;
    }
  }
}

void placeHallways(Dungeon* dun) {
  int xPos1, xPos2, yPos1, yPos2, roomNum;
  Node* currentNode;
  Room currentRoom, nextRoom;

  for(roomNum = 0; roomNum < dun->rooms->length - 1; roomNum++) {
    if(roomNum == 0) {
      currentNode = dun->rooms->head;
    } else {
        currentNode = currentNode->next;
    }

    currentRoom = *(Room*) currentNode->dataPtr;
    nextRoom = *(Room*) currentNode->next->dataPtr;

    xPos1 = currentRoom.xPos;
    yPos1 = currentRoom.yPos;

    xPos2 = nextRoom.xPos;
    yPos2 = nextRoom.yPos;
    
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
    dun->map[row][col].isBorder = false;
    dun->map[row][col].isRoom = false;
    dun->map[row][col].hardness = MIN_ROCK_HARDNESS;
  }
}

void saveDungeon(Dungeon* dun, char* saveLoc) {
  FILE* file = fopen(saveLoc, "w");
  char str[12] = {'R', 'L', 'G', '3', '2', '7', '-', 'S', '2', '0', '1', '8'};
  uint32_t version = 0;
  uint32_t roomStorageSize = 4; // 4 bytes
  uint32_t fileSize = 1700 + (roomStorageSize * dun->rooms->length);
  int row, col;
  Node* currentNode;
  int index;
  Room currentRoom;


  fileSize = endianSwap_uInt(fileSize);

  fwrite(str, sizeof(str), 1, file);
  fwrite(&version, sizeof(uint32_t), 1, file);
  fwrite(&fileSize, sizeof(uint32_t), 1, file);

  // save dungeon map
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      fwrite(&dun->map[row][col].hardness, sizeof(uint8_t), 1, file);
    }
  }

  // save the rooms
  currentNode = dun->rooms->head;
  for(index = 0; index < dun->rooms->length; index++){
    currentRoom = *(Room*)currentNode->dataPtr;
    fwrite(&currentRoom.yPos, sizeof(uint8_t), 1, file);
    fwrite(&currentRoom.xPos, sizeof(uint8_t), 1, file);
    fwrite(&currentRoom.height, sizeof(uint8_t), 1, file);
    fwrite(&currentRoom.width, sizeof(uint8_t), 1, file);
    currentNode = currentNode->next;
  }
}

Dungeon* loadDungeon(char* loadLoc) {
  printf("loading\n");
  Dungeon* newDungeon;
  char* header = malloc(sizeof(char) * 12);
  FILE* file = fopen(loadLoc, "r");
  uint32_t versionMarker;
  uint32_t fileSize;
  int row, col;
  Room* currentRoom;
  int roomNum;

  newDungeon = malloc(sizeof(Dungeon));

  initDungeon(newDungeon);

  // read the header
  fread(header, 12, 1, file);
  
  // read the version
  fread(&versionMarker, sizeof(uint32_t), 1, file);
  
  // get the total file size
  fread(&fileSize, sizeof(uint32_t), 1, file);
  fileSize = endianSwap_uInt(fileSize);

  // get the tile info
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      fread(&newDungeon->map[row][col].hardness, sizeof(uint8_t), 1, file);
    }
  }

  // get the rooms
  for(roomNum = 0; roomNum < (fileSize - 1700)/4; roomNum++) {
    currentRoom = malloc(sizeof(Room));
    fread(&currentRoom->yPos, sizeof(uint8_t), 1, file);
    fread(&currentRoom->xPos, sizeof(uint8_t), 1, file);
    fread(&currentRoom->height, sizeof(uint8_t), 1, file);
    fread(&currentRoom->width, sizeof(uint8_t), 1, file);
    addRoomToDungeon(newDungeon, *currentRoom);
    listAdd(currentRoom, newDungeon->rooms);
  }

  // define remaining tiles
  defineTiles(newDungeon);

  return newDungeon;
}

uint32_t endianSwap_uInt(uint32_t input) {
  uint32_t leftMost = (input << 8 * 3) & 0xff000000;
  uint32_t left = (input << 8) & 0x00ff0000;
  uint32_t right = (input >> 8) & 0x0000ff00;
  uint32_t rightMost = (input >> 8 * 3) & 0x000000ff;
  uint32_t ret = leftMost + left + right + rightMost;
  
  return ret;
}

void defineTiles(Dungeon* dun) {
  int row, col;
  
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      if(dun->map[row][col].hardness == MAX_ROCK_HARDNESS) {
        dun->map[row][col].isBorder = true;
      } else if(dun->map[row][col].hardness == MIN_ROCK_HARDNESS && 
          !dun->map[row][col].isRoom) {
        dun->map[row][col].isHallway = true;
      }
    }
  }
}
