#include "crawler.h"

int main(int argc, char* argv[]) {

  srand(time(0));

  Setup setup = parseArgs(argc, argv);
  
  printf("Using seed: %d\n", setup.seed);
  // seed random num gen
  srand(setup.seed);

  // set up the dungeon
  Dungeon* dungeon;
  if(setup.load){
    dungeon = loadDungeon(setup.saveLoadLocation);
  } else {
    dungeon = genDungeon();
  }
  
  
  // start game loop
  runGame(dungeon, setup);

  if(setup.save) {
    saveDungeon(dungeon, setup.saveLoadLocation);
  }
}

/**
 * @brief The function where the main game loop lives
 * 
 * @param dun     The dungeon that the game will use
 * @param setup   a structure that holds settings
 */

void runGame(Dungeon* dun, Setup setup) {
  boolean continueRunning = false;
  int** tunnelingMap;
  int** openSpaceMap;
  Player pc;

  //randomlyPlace(&pc.coord, dun);
  pc.coord.row = 9;
  pc.coord.col = 54;
  
  do {
    // get the map for tunneling creatures
    tunnelingMap = getPathMapEverywhere(&pc.coord, dun);
    // get the map for the non-tunneling creatures
    openSpaceMap = getPathMapOnlyOpenArea(&pc.coord, dun);

    // print the dungeon
    printDungeon(dun, setup, pc.coord);

    printPathMap(openSpaceMap, &pc);
    
    printPathMap(tunnelingMap, &pc);

  } while(continueRunning);
}

/**
 * @brief   A function that randomly places assigns a
 *          location to the input coord such that the
 *          location is in a hallway or room
 * 
 * @param cord 
 * @param dun 
 */

void randomlyPlace(Coordinate* coord, Dungeon* dun) {
  boolean notPlaced = true;
  int row, col;
  
  // get a new seed based upon the room placement
  updateSeed(dun);

  while(notPlaced) {
    row = rand() % MAX_DUNGEON_HEIGHT + 1;
    col = rand() % MAX_DUNGEON_WIDTH + 1;
    if(row >= 0 && row < MAX_DUNGEON_HEIGHT && col >= 0 && col < MAX_DUNGEON_WIDTH && dun->map[row][col].hardness == 0) {
      coord->row = row;
      coord->col =  col;
      notPlaced = false;
    }
  }
}

/**
 * @brief     Generates a new seed using the positions'
 *            of the rooms in the dungeon
 * 
 * @param dun  The dungeon that is being used to
 *             update the seed
 */
void updateSeed(Dungeon* dun) {
  Node* currentNode = dun->rooms->head;
  Room* currentRoom;
  int newSeed = -1;
  while(currentNode != NULL) {
    currentRoom = (Room*)currentNode->dataPtr;
    if(currentRoom->xPos % 3 == 0) {
      newSeed += currentRoom->xPos;
    } else {
      newSeed -= currentRoom->xPos;
    }
    if(currentRoom->yPos % 2 == 0) {
      newSeed += currentRoom->yPos;
    } else {
      newSeed -= currentRoom->yPos;
    }
    currentNode = currentNode->next;
  }
  srand(newSeed);
}
    
/**
 * @brief       Prints the distance map that was input
 *              as well as the player character in the
 *              map
 * 
 * @param map   The map being printed
 * @param pc    The player character
 */
void printPathMap(int** tunnelingMap, Player* pc) {
  int row, col, tileVal;
  printf("printing path map\n");
  
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    printf("\t");
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      tileVal = tunnelingMap[row][col];
      if(tileVal < INT_MAX) {
        if(tileVal == 0) {
          printf("@");
        } else {
          printf("%d", tunnelingMap[row][col] % 10);
        }
      } else {
        printf(" ");
      }
    }
    printf("\n");
  }
}

/**
 * @brief   Looks at the startup arguments to set flags
 *          for how the game should run
 * 
 * @param argc    the number of arguments
 * @param argv    the vector containing the arguments
 * @return Setup  a struct that contains the things options set
 */
Setup parseArgs(int argc, char* argv[]) {
  Setup ret;
  int i, length;

  ret.seed = rand();
  ret.useCoolChars = false;
  ret.save = false;
  ret.load = false;
  
  // look for flags
  for(i = 0; i < argc; i++) {
    if(strcmp(argv[i], "-s") == 0) {
      if(i + 1 < argc){
        ret.seed = atoi(argv[i+1]);
      }
    }
    else if(strcmp(argv[i], "-cool") == 0) {
      ret.useCoolChars = true;
    }
    else if(strcmp(argv[i], "--save") == 0) {
      ret.save = true;
    }
    else if(strcmp(argv[i], "--load") == 0) {
      ret.load = true;
    }
  }

  if(ret.save || ret.load) {
    length = sizeof(getenv("HOME")) + sizeof("/.rlg327/dungeon") + 1;
    ret.saveLoadLocation = malloc(length);
    ret.saveLoadLocation = strcpy(ret.saveLoadLocation, getenv("HOME"));
    strcat(ret.saveLoadLocation, "/.rlg327/dungeon");
  }
  return ret;
}

/**
 * @brief   prints the dungeon to screen
 * 
 * @param dun     the dungeon to print
 * @param setup   a struct containg print info
 */
void printDungeon(Dungeon* dun, Setup setup, Coordinate pc) {
  if(setup.useCoolChars) {
    printCoolDun(dun, pc);
  } else {
    printStandardDun(dun, pc);
  }
}

/**
 * @brief Prints the dungeon in the standard format
 *        as perscibed in the homework description.
 * 
 * @param dun a pointer to the dungeon to print
 */
void printStandardDun(Dungeon* dun, Coordinate pc) {
  int row, col;
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    printf("\t");
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      // if the tile is a border
      if(dun->map[row][col].isBorder) {
        // if the cell is in the first or last row
        if(row == 0 || row == MAX_DUNGEON_HEIGHT - 1) {
          printf("%c", BORDER_HOR_CHAR);
        }
        // else the cell is one either in the first or last col
        else {
          printf("%c", BORDER_VERT_CHAR);
        }
      }
      // else the tile is part of the normal dungeon
      else {
        if(row == pc.row && col == pc.col) {
          printf("%c", PC_CHAR);
        } else if(dun->map[row][col].isRoom) {
          printf("%c", ROOM_CHAR);
        } else if(dun->map[row][col].isHallway) {
          printf("%c", HALL_CHAR);
        }

        else {
          printf(" ");
        }
      }
    }
    printf("\n");
  }
}

/**
 * @brief Prints the dungeon in a cool way using
 *        unicode characters for a better looking
 *        dungeon map
 * 
 * @param dun a pointer to the dungeon to print
 */
void printCoolDun(Dungeon* dun, Coordinate pc) {
  int row, col;
  char* charToPrint;

  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    printf("\t");
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      if(dun->map[row][col].isBorder) {
        if(row == 0 && col == 0) {
          charToPrint = COOL_BORDER_TOP_LEFT;
        } else if(row == 0 && col == MAX_DUNGEON_WIDTH - 1) {
          charToPrint = COOL_BORDER_TOP_RIGHT;
        } else if(row == MAX_DUNGEON_HEIGHT - 1 && col == 0) {
          charToPrint = COOL_BORDER_BOTTOM_LEFT;
        } else if(row == MAX_DUNGEON_HEIGHT - 1 && col == MAX_DUNGEON_WIDTH - 1) {
          charToPrint = COOL_BORDER_BOTTOM_RIGHT;
        } else if(row == 0 || row == MAX_DUNGEON_HEIGHT -1) {
          charToPrint = COOL_BORDER_HOR;
        } else if(col == 0 || col == MAX_DUNGEON_WIDTH -1){
          charToPrint = COOL_BORDER_VERT;
        }
      } else {
         if(dun->map[row][col].isHallway) {
          charToPrint = COOL_HALL_CHAR;
        } else if(dun->map[row][col].isRoom) {
          charToPrint = COOL_ROOM_CHAR;
        } else {
          charToPrint = COOL_ROCK;
        }
      }
      if(row == pc.row && col == pc.col) {
          printf("%c", PC_CHAR);
        } else {
          printf("%s", charToPrint);
        }
    }
    printf("\n");
  }
}
