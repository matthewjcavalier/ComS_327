#include "crawler.h"

int main(int argc, char* argv[]) {

  srand(time(0));

  Setup setup = parseArgs(argc, argv);
  
  printf("Using seed: %d\n", setup.seed);
  printf("Number of Monsters: %d\n", setup.numMonsters);
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
  boolean endGame = true;
  int** tunnelingMap;
  int** openSpaceMap;
  Character* placementMap[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH];
  int currentTurn = 0;
  int i;
  Player pc;
  boolean printMaps = false;
  MinHeap* turnQueue = initHeap(setup.numMonsters + 1);
  Character* currentChar = malloc(sizeof(*currentChar));
  NPC* currentNPC;

  // clean placement Map
  cleanPlacementMap(placementMap);

  // setup PC
  randomlyPlace(&pc.coord, dun);
  currentChar->pc = &pc;
  currentChar->coord = pc.coord;
  currentChar->symbol = PC_CHAR;
  currentChar->speed = 10;
  currentChar->npc = NULL;
  currentChar->nextEventTime = 1000/currentChar->speed + currentTurn;

  addToHeap(turnQueue, currentChar);

  placementMap[pc.coord.row][pc.coord.col] = currentChar;

  // add monsters
  for(i = 0; i < setup.numMonsters; i++) {
    currentNPC = malloc(sizeof(*currentNPC));
    currentNPC->characteristics = 0b0000;
    // set intelligence
    if(rand() % 2 == 0) {
      currentNPC->characteristics |= INTELLIGENCE_BIT;
    }
    if(rand() % 2 == 0) {
      currentNPC->characteristics |= TELEPATHY_BIT;
    }
     if(rand() % 2 == 0) {
      currentNPC->characteristics |= TUNNELING_BIT;
    }
      if(rand() % 2 == 0) {
      currentNPC->characteristics |= ERRATIC_BIT;
    }
    currentChar = malloc(sizeof(*currentChar));
    currentChar->coord = getEmptySpot(dun, placementMap);
    currentChar->symbol = getSymbol(currentNPC->characteristics);
    currentChar->pc = NULL;
    currentChar->npc = currentNPC;
    currentChar->speed = rand() % 15 + 5;
    currentChar->nextEventTime = currentTurn + currentChar->speed;
    placementMap[currentChar->coord.row][currentChar->coord.col] = currentChar;
  }


  
  do {
    // get the map for tunneling creatures
    tunnelingMap = getPathMapEverywhere(&pc.coord, dun);
    // get the map for the non-tunneling creatures
    openSpaceMap = getPathMapOnlyOpenArea(&pc.coord, dun);

    // print the dungeon
    printDungeon(dun, setup, placementMap);

    if(printMaps) {
      printPathMap(openSpaceMap, &pc);
      
      printPathMap(tunnelingMap, &pc);
    }

  } while(!endGame);
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
  ret.numMonsters = 0;
  
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
    else if(strcmp(argv[i], "--nummon") == 0) {
      ret.numMonsters = atoi(argv[i + 1]);
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
void printDungeon(Dungeon* dun, Setup setup, Character* placementMap[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]) {
  if(setup.useCoolChars) {
    printCoolDun(dun, placementMap);
  } else {
    printStandardDun(dun, placementMap);
  }
}

/**
 * @brief Prints the dungeon in the standard format
 *        as perscibed in the homework description.
 * 
 * @param dun a pointer to the dungeon to print
 */
void printStandardDun(Dungeon* dun, Character* placementMap[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]) {
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
        if(placementMap[row][col] != NULL) {
          printf("%c", placementMap[row][col]->symbol);
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
void printCoolDun(Dungeon* dun, Character* placementMap[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]) {
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
      if(placementMap[row][col] != NULL) {
          printf("%c", placementMap[row][col]->symbol);
        } else {
          printf("%s", charToPrint);
        }
    }
    printf("\n");
  }
}
 void cleanPlacementMap(Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]) {
   int row, col;
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      map[row][col] = NULL;
    }
  }
 }
Coordinate getEmptySpot(Dungeon* dun,Character* placementMap[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]) {
  boolean notPlaced = true;
  int row, col;
  Coordinate coord;
  
  while(notPlaced) {
    row = rand() % MAX_DUNGEON_HEIGHT + 1;
    col = rand() % MAX_DUNGEON_WIDTH + 1;
    if(row >= 0 && row < MAX_DUNGEON_HEIGHT && col >= 0 && col < MAX_DUNGEON_WIDTH && dun->map[row][col].hardness == 0 && placementMap[row][col] == NULL) {
      coord.row = row;
      coord.col =  col;
      notPlaced = false;
    }
  } 
  return coord;
}

char getSymbol(char ristics) {
  char ret = '!';
  switch(ristics) {
    case 0:
      ret = '0';
      break;
    case 1:
      ret = '1';
      break;
    case 2:
      ret = '2';
      break;
    case 3:
      ret = '3';
      break;
    case 4:
      ret = '5';
      break;
    case 6:
      ret = '6';
      break;
    case 7:
      ret = '7';
      break;
    case 8:
      ret = '8';
      break;
    case 9:
      ret = '9';
      break;
    case 10:
      ret = 'A';
      break;
    case 11:
      ret = 'B';
      break;
    case 12:
      ret = 'C';
      break;
    case 13:
      ret = 'D';
      break;
    case 14:
      ret = 'E';
      break;
    default:
      ret = 'F';
      break;
  }
  return ret;
}
