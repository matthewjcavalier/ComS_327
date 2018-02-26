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
  bool endGame = FALSE;

  int** tunnelingMap;
  int** openSpaceMap;
  Character* placementMap[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH];
  int currentTurn = 0;
  int i;
  Player pc;
  bool printMaps = FALSE;
  MinHeap* turnQueue = initHeap(setup.numMonsters + 1);
  Character* currentChar = malloc(sizeof(*currentChar));
  NPC* currentNPC;
  Character* pcCharacter = malloc(sizeof(*pcCharacter));

  // clean placement Map
  cleanPlacementMap(placementMap);

  // setup PC
  randomlyPlace(&pcCharacter->coord, dun);
  pcCharacter->symbol = PC_CHAR;
  pcCharacter->speed = 10;
  pcCharacter->npc = NULL;
  pcCharacter->pc = &pc;
  pcCharacter->nextEventTime = 1000/pcCharacter->speed + currentTurn;



  addToHeap(turnQueue, pcCharacter);

  placementMap[pcCharacter->coord.row][pcCharacter->coord.col] = pcCharacter;

  // add monsters
  for(i = 0; i < setup.numMonsters; i++) {
    currentNPC = malloc(sizeof(*currentNPC));
    currentNPC->characteristics = 0;
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
    currentNPC->hasSeenPC = FALSE;
//    currentNPC->characteristics = 0b0000;

    currentChar = malloc(sizeof(*currentChar));
    currentChar->coord = getEmptySpot(dun, placementMap);
    currentChar->symbol = getSymbol(currentNPC->characteristics);
    currentChar->pc = NULL;
    currentChar->npc = currentNPC;
    currentChar->speed = rand() % 15 + 5;
    currentChar->nextEventTime = currentTurn + 1000/currentChar->speed;
    placementMap[currentChar->coord.row][currentChar->coord.col] = currentChar;
    addToHeap(turnQueue, currentChar);
  }

  // get the map for tunneling creatures
  tunnelingMap = getPathMapEverywhere(&pcCharacter->coord, dun);
  // get the map for the non-tunneling creatures
  openSpaceMap = getPathMapOnlyOpenArea(&pcCharacter->coord, dun);

  // print the dungeon
  //printDungeon(dun, setup, placementMap);
  
  // screen startup routine
  startup();

  drawDungeon(dun, setup);

  drawEntities(placementMap);

  do {
    // do move sets
    currentChar = removeFromHeap(turnQueue);

    // if is an NPC
    if(currentChar->pc == NULL) {
      monster_routine(currentChar, turnQueue, dun, placementMap, openSpaceMap, tunnelingMap, pcCharacter);
    }
    // else is npc
    else {
      pc_routine(currentChar, turnQueue, dun, placementMap);
      // generate the new maps
      tunnelingMap = getPathMapEverywhere(&pcCharacter->coord, dun);
      // get the map for the non-tunneling creatures
      openSpaceMap = getPathMapOnlyOpenArea(&pcCharacter->coord, dun);

      if(printMaps) {
        printPathMap(openSpaceMap, &pc);
        
        printPathMap(tunnelingMap, &pc);
      }
      
      usleep(3);
    }
    // print the dungeon

    
    currentChar->nextEventTime = currentChar->nextEventTime + 1000/currentChar->speed;
    addToHeap(turnQueue, currentChar);

    if(getIndexOfPC(turnQueue) == -1) {
      endGame = TRUE;
      tearDown();
      printf("\n\n\nYOU LOSE\n\n\n");
    } else if(turnQueue->size == 1) {
      endGame = TRUE;
      tearDown();
      printf("\n\n\nYOU WIN\n\n\n");
    }
  } while(!endGame);
}

bool canSeePC(Character* pc, Character* monster, Dungeon* dun) {
  Coordinate currentCoord = monster->coord;
  do {
    currentCoord = moveToward(currentCoord, pc->coord);
    if(dun->map[currentCoord.row][currentCoord.col].hardness != 0) {
      return FALSE;
    }
  } while(currentCoord.row != pc->coord.row && currentCoord.col != pc->coord.col);
  // going to go in a straight line
  return TRUE;
}

Coordinate moveToward(Coordinate from, Coordinate to) {
  Coordinate ret = from;
  
  if(from.row > to.row) {
    ret.row -= 1;
  }
  else if(from.row < to.row) {
    ret.row += 1;
  }

  if(from.col > to.col) {
    ret.col -= 1;
  }
  else if(from.col < to.col) {
    ret.col += 1;
  }
  return ret;
}
Coordinate getNextPlacementTunneling(int** map, Coordinate coord, Dungeon* dun) {
  Coordinate next = coord;
  int min = map[coord.row][coord.col];
  // up left
  if(min > map[coord.row - 1][coord.col -1] || 
      (min == map[coord.row - 1][coord.col -1] && dun->map[next.row][next.col].hardness > 0 &&dun->map[coord.row -1][coord.col -1].hardness == 0)) {
    next = coord;
    min = map[next.row--][next.col--];
  }
  // up
  if(min > map[coord.row - 1][coord.col] || 
      (min == map[coord.row - 1][coord.col] && dun->map[next.row][next.col].hardness > 0 &&dun->map[coord.row -1][coord.col].hardness == 0)) {
    next = coord;
    min = map[next.row--][next.col];
  }
  // up right
  if(min > map[coord.row - 1][coord.col +1] || 
      (min == map[coord.row - 1][coord.col +1] && dun->map[next.row][next.col].hardness > 0 &&dun->map[coord.row -1][coord.col +1].hardness == 0)) {
    next = coord;
    min = map[next.row--][next.col++];
  }
  // right
  if(min > map[coord.row][coord.col +1] || 
      (min == map[coord.row ][coord.col +1] && dun->map[next.row][next.col].hardness > 0 &&dun->map[coord.row ][coord.col +1].hardness == 0)) {
    next = coord;
    min = map[next.row][next.col++];
  }
  // down right
  if(min > map[coord.row +1][coord.col +1] || 
      (min == map[coord.row +1][coord.col +1] && dun->map[next.row][next.col].hardness > 0 &&dun->map[coord.row +1][coord.col +1].hardness == 0)) {
    next = coord;
    min = map[next.row++][next.col++];
  }
  // down
  if(min > map[coord.row +1][coord.col] || 
      (min == map[coord.row + 1][coord.col] && dun->map[next.row][next.col].hardness > 0 &&dun->map[coord.row +1][coord.col].hardness == 0)) {
    next = coord;
    min = map[next.row++][next.col];
  }
  // down left
  if(min > map[coord.row +1][coord.col -1] || 
      (min == map[coord.row + 1][coord.col -1] && dun->map[next.row][next.col].hardness > 0 &&dun->map[coord.row +1][coord.col -1].hardness == 0)) {
    next = coord;
    min = map[next.row++][next.col--];
  }
  // left
  if(min > map[coord.row][coord.col -1] || 
      (min == map[coord.row ][coord.col -1] && dun->map[next.row][next.col].hardness > 0 && dun->map[coord.row ][coord.col -1].hardness == 0)) {
    next = coord;
    min = map[next.row][next.col--];
  }
  return next;
}
Coordinate getNextPlacement(int** map, Coordinate coord) {
  Coordinate next = coord;
  int min = map[coord.row][coord.col];
  // up left
  if(min > map[coord.row - 1][coord.col -1]) {
    min = map[next.row--][next.col--];
  }
  // up
  if(min > map[coord.row - 1][coord.col]) {
    min = map[next.row--][next.col];
  }
  // up right
  if(min > map[coord.row - 1][coord.col +1]) {
    min = map[next.row--][next.col++];
  }
  // right
  if(min > map[coord.row][coord.col +1]) {
    min = map[next.row][next.col++];
  }
  // down right
  if(min > map[coord.row +1][coord.col +1]) {
    min = map[next.row++][next.col++];
  }
  // down
  if(min > map[coord.row +1][coord.col]) {
    min = map[next.row++][next.col];
  }
  // down left
  if(min > map[coord.row +1][coord.col -1]) {
    min = map[next.row++][next.col--];
  }
  // left
  if(min > map[coord.row][coord.col -1]) {
    min = map[next.row][next.col--];
  }
  return next;
}

void nonEraticMovment(Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH], int** openSpaceMap, int** tunnelingMap, Character* pc, char characteristics) {
  switch(character->npc->characteristics) {
      case 0b0000:
        moveRandomly(character, turnQueue, dun, map, FALSE, NULL);     
        break;
      // is intelligent
      case 0b0001:
        // check if monster can see PC
        if(canSeePC(pc, character, dun)) {
          character->npc->hasSeenPC = TRUE;
          character->npc->lastKnowPCLoc = pc->coord;
        }

        if(character->npc->hasSeenPC) {
          moveCharacterNoTunnel(getNextPlacement(getPathMapOnlyOpenArea(&character->npc->lastKnowPCLoc, dun) , character->coord), character, turnQueue, dun, map);
        } else {
          moveRandomly(character, turnQueue, dun, map, FALSE, NULL);     
        }
        break;
      
      // is telepathic
      case 0b0010:
        moveCharacterNoTunnel(moveToward(character->coord, pc->coord), character, turnQueue, dun, map);
        break;

      // is telepathic and intelligent
      case 0b0011:
        
        moveCharacterNoTunnel(getNextPlacement(openSpaceMap, character->coord), character, turnQueue, dun, map);
        break;

      // can tunnel
      case 0b0100:
        moveRandomly(character, turnQueue, dun, map, TRUE, tunnelingMap);     
        // get the map for tunneling creatures
        tunnelingMap = getPathMapEverywhere(&pc->coord, dun);
        // get the map for the non-tunneling creatures
        openSpaceMap = getPathMapOnlyOpenArea(&pc->coord, dun);

        break;
      
      // can tunnel and is intelligent
      case 0b0101:
        if(canSeePC(pc, character, dun)) {
          character->npc->hasSeenPC = TRUE;
          character->npc->lastKnowPCLoc = pc->coord;
        }
              if(character->npc->hasSeenPC) {
          moveCharacterTunnel(getNextPlacementTunneling(getPathMapEverywhere(&character->npc->lastKnowPCLoc, dun) , character->coord, dun), character, turnQueue, dun, map, tunnelingMap);
        } else {
        moveRandomly(character, turnQueue, dun, map, FALSE, NULL);     
        }
        // get the map for tunneling creatures
        tunnelingMap = getPathMapEverywhere(&pc->coord, dun);
        // get the map for the non-tunneling creatures
        openSpaceMap = getPathMapOnlyOpenArea(&pc->coord, dun);

        break;

      // can tunnel and is telepathic
      case 0b0110:
        if(canSeePC(pc, character, dun)) {
          character->npc->hasSeenPC = TRUE;
          character->npc->lastKnowPCLoc = pc->coord;
        }
              if(character->npc->hasSeenPC) {
          moveCharacterTunnel(moveToward(character->coord, pc->coord), character, turnQueue, dun, map, tunnelingMap);
        } else {
        moveRandomly(character, turnQueue, dun, map, FALSE, NULL);     
        }
        // get the map for tunneling creatures
        tunnelingMap = getPathMapEverywhere(&pc->coord, dun);
        // get the map for the non-tunneling creatures
        openSpaceMap = getPathMapOnlyOpenArea(&pc->coord, dun);

        break;

      // can tunnel and is telepathic and is intelligent
      case 0b0111:
        if(canSeePC(pc, character, dun)) {
          character->npc->hasSeenPC = TRUE;
          character->npc->lastKnowPCLoc = pc->coord;
        }
              if(character->npc->hasSeenPC) {
          moveCharacterTunnel(getNextPlacementTunneling(getPathMapEverywhere(&pc->coord, dun) , character->coord, dun), character, turnQueue, dun, map, tunnelingMap);
        } else {
        moveRandomly(character, turnQueue, dun, map, FALSE, NULL);     
        }
        // get the map for tunneling creatures
        tunnelingMap = getPathMapEverywhere(&pc->coord, dun);
        // get the map for the non-tunneling creatures
        openSpaceMap = getPathMapOnlyOpenArea(&pc->coord, dun);

        break;
  }
  }

void monster_routine(Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH], int** openSpaceMap, int** tunnelingMap, Character* pc) {
    switch(character->npc->characteristics) {
    case 0b0000:
    case 0b0001:
    case 0b0010:
    case 0b0011:
    case 0b0100:
    case 0b0101:
    case 0b0110:
    case 0b0111:
      nonEraticMovment(character, turnQueue, dun, map, openSpaceMap, tunnelingMap, pc, character->npc->characteristics);
      break;
    // is erratic
    case 0b1000:
      moveRandomly(character, turnQueue, dun, map, FALSE, NULL);
      break;

    // is erratic and intelligent
    default:
      if(rand() % 2 == 0) {
        moveRandomly(character, turnQueue, dun, map, FALSE, NULL);
      } else {
        nonEraticMovment(character, turnQueue, dun, map, openSpaceMap, tunnelingMap, pc, character->npc->characteristics & 0b0111);
      }
  }
}

bool canTunnel(Character* character) {
  if(character->npc->characteristics & TUNNELING_BIT) {
    return TRUE;
  }
  return FALSE;
}

bool isTelepathic(Character* character) {
  if(character->npc->characteristics & TELEPATHY_BIT) {
    return TRUE;
  }
  return FALSE;
}

bool isInteligent(Character* character) {
  if(character->npc->characteristics & INTELLIGENCE_BIT) {
    return TRUE;
  }
  return FALSE;
}

bool isErratic(Character* character) {
  if(character->npc->characteristics & ERRATIC_BIT) {
    return TRUE;
  }
  return FALSE;
}

void pc_routine(Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]) {
  // TODO: add user control
  
  moveRandomly(character, turnQueue, dun, map, FALSE, NULL);

  sleep(1);
}

int moveRandomly(Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH], bool canTunnel, int** hardnessMap) {
  bool notPlaced = TRUE;
  Coordinate movingTo;
  Coordinate movingFrom = character->coord;

  while(notPlaced) {
    switch(rand() % 8) {
      // move up left
      case 0:
        movingTo = character->coord;
        movingTo.row = movingTo.row -1;
        movingTo.col = movingTo.col -1;
        if(canTunnel) {
          notPlaced = moveCharacterTunnel(movingTo, character, turnQueue, dun, map, hardnessMap);
        } else {
          notPlaced = moveCharacterNoTunnel(movingTo, character, turnQueue, dun, map);
        }
        break;

      // move up
      case 1:
        movingTo = character->coord;
        movingTo.row = movingTo.row -1;
        if(canTunnel) {
          notPlaced = moveCharacterTunnel(movingTo, character, turnQueue, dun, map, hardnessMap);
        } else {
          notPlaced = moveCharacterNoTunnel(movingTo, character, turnQueue, dun, map);
        }
        break;

      // move up right
      case 2:
        movingTo = character->coord;
        movingTo.row = movingTo.row -1;
        movingTo.col = movingTo.col +1;
        if(canTunnel) {
          notPlaced = moveCharacterTunnel(movingTo, character, turnQueue, dun, map, hardnessMap);
        } else {
          notPlaced = moveCharacterNoTunnel(movingTo, character, turnQueue, dun, map);
        }
        break;

      // move right
      case 3:
        movingTo = character->coord;
        movingTo.col = movingTo.col +1;
        if(canTunnel) {
          notPlaced = moveCharacterTunnel(movingTo, character, turnQueue, dun, map, hardnessMap);
        } else {
          notPlaced = moveCharacterNoTunnel(movingTo, character, turnQueue, dun, map);
        }
        break;

      // move down right
      case 4:
        movingTo = character->coord;
        movingTo.row = movingTo.row +1;
        movingTo.col = movingTo.col +1;
        if(canTunnel) {
          notPlaced = moveCharacterTunnel(movingTo, character, turnQueue, dun, map, hardnessMap);
        } else {
          notPlaced = moveCharacterNoTunnel(movingTo, character, turnQueue, dun, map);
        }
        break;
       
      // move down
      case 5:
        movingTo = character->coord;
        movingTo.row = movingTo.row +1;
        if(canTunnel) {
          notPlaced = moveCharacterTunnel(movingTo, character, turnQueue, dun, map, hardnessMap);
        } else {
          notPlaced = moveCharacterNoTunnel(movingTo, character, turnQueue, dun, map);
        }
        break;
       
      // move down left
      case 6:
        movingTo = character->coord;
        movingTo.row = movingTo.row +1;
        movingTo.col = movingTo.col -1;
        if(canTunnel) {
          notPlaced = moveCharacterTunnel(movingTo, character, turnQueue, dun, map, hardnessMap);
        } else {
          notPlaced = moveCharacterNoTunnel(movingTo, character, turnQueue, dun, map);
        }
        break;
       
      // move left
      default:
        movingTo = character->coord;
        movingTo.col = movingTo.col -1;
        if(canTunnel) {
          notPlaced = moveCharacterTunnel(movingTo, character, turnQueue, dun, map, hardnessMap);
        } else {
          notPlaced = moveCharacterNoTunnel(movingTo, character, turnQueue, dun, map);
        }
        break;
    }
  }
  drawCharacter(movingFrom.row + 1, movingFrom.col, getDunChar(dun->map[movingFrom.row][movingFrom.col], movingFrom.row, movingFrom.col));
  drawCharacter(movingTo.row + 1, movingTo.col, character->symbol);
  return 0;
}

bool moveCharacterNoTunnel(Coordinate movingTo, Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]) {

  if(isEmptySpace(movingTo, dun)) {
    // move the character
    map[character->coord.row][character->coord.col] = NULL;
   drawCharacter(character->coord.row + 1, character->coord.col, 
          getDunChar(dun->map[character->coord.row][character->coord.col],character->coord.row,character->coord.col));
    if(map[movingTo.row][movingTo.col] != NULL) {
      deleteFromHeap(turnQueue, map[movingTo.row][movingTo.col]);
    }
    character->coord = movingTo;
    map[movingTo.row][movingTo.col] = character;
    drawCharacter(movingTo.row + 1, movingTo.col, character->symbol);   
    return FALSE;
  }
  return TRUE;
}

bool moveCharacterTunnel(Coordinate movingTo, Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH], int** hardnessMap) {

  if(dun->map[movingTo.row][movingTo.col].isBorder == FALSE && movingTo.row > 0 && movingTo.row < MAX_DUNGEON_HEIGHT && movingTo.col > 0 && movingTo.col < MAX_DUNGEON_WIDTH) {
    if(dun->map[movingTo.row][movingTo.col].hardness > 0) {
      dun->map[movingTo.row][movingTo.col].hardness -= 85;
    }
    if(dun->map[movingTo.row][movingTo.col].hardness <= 0){
      dun->map[movingTo.row][movingTo.col].hardness = 0;
      if(dun->map[movingTo.row][movingTo.col].isRoom == FALSE) {
        dun->map[movingTo.row][movingTo.col].isHallway = TRUE;
      }
      // move the character
      map[character->coord.row][character->coord.col] = NULL;
      drawCharacter(character->coord.row + 1, character->coord.col, 
          getDunChar(dun->map[character->coord.row][character->coord.col],character->coord.row,character->coord.col));
      if(map[movingTo.row][movingTo.col] != NULL) {
        deleteFromHeap(turnQueue, map[movingTo.row][movingTo.col]);
      }
      character->coord = movingTo;
      map[movingTo.row][movingTo.col] = character;
      drawCharacter(movingTo.row + 1, movingTo.col, character->symbol);
      return FALSE;
    }
  }
  return TRUE;
}

bool isEmptySpace(Coordinate coord, Dungeon* dun) {
  if(dun->map[coord.row][coord.col].hardness == 0 ) {
    return TRUE;
  }
  return FALSE;
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
  bool notPlaced = TRUE;
  int row, col;
  
  // get a new seed based upon the room placement
  updateSeed(dun);

  while(notPlaced) {
    row = rand() % MAX_DUNGEON_HEIGHT + 1;
    col = rand() % MAX_DUNGEON_WIDTH + 1;
    if(row >= 0 && row < MAX_DUNGEON_HEIGHT && col >= 0 && col < MAX_DUNGEON_WIDTH && dun->map[row][col].hardness == 0) {
      coord->row = row;
      coord->col =  col;
      notPlaced = FALSE;
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
  ret.useCoolChars = FALSE;
  ret.save = FALSE;
  ret.load = FALSE;
  ret.numMonsters = 0;
  
  // look for flags
  for(i = 0; i < argc; i++) {
    if(strcmp(argv[i], "-s") == 0) {
      if(i + 1 < argc){
        ret.seed = atoi(argv[i+1]);
      }
    }
    else if(strcmp(argv[i], "-cool") == 0) {
      ret.useCoolChars = TRUE;
    }
    else if(strcmp(argv[i], "--save") == 0) {
      ret.save = TRUE;
    }
    else if(strcmp(argv[i], "--load") == 0) {
      ret.load = TRUE;
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
  bool notPlaced = TRUE;
  int row, col;
  Coordinate coord;
  
  while(notPlaced) {
    row = rand() % MAX_DUNGEON_HEIGHT + 1;
    col = rand() % MAX_DUNGEON_WIDTH + 1;
    if(row >= 0 && row < MAX_DUNGEON_HEIGHT && col >= 0 && col < MAX_DUNGEON_WIDTH && dun->map[row][col].hardness == 0 && placementMap[row][col] == NULL) {
      coord.row = row;
      coord.col =  col;
      notPlaced = FALSE;
    }
  } 
  return coord;
}

char getSymbol(char ristics) {
  char ret = '!';
  switch(ristics) {
    case 0b0:
      ret = '0';
      break;
    case 0b1:
      ret = '1';
      break;
    case 0b10:
      ret = '2';
      break;
    case 0b11:
      ret = '3';
      break;
    case 0b100:
      ret = '4';
      break;
    case 0b101:
      ret = '5';
      break;
    case 0b110:
      ret = '6';
      break;
    case 0b111:
      ret = '7';
      break;
    case 0b1000:
      ret = '8';
      break;
    case 0b1001:
      ret = '9';
      break;
    case 0b1010:
      ret = 'A';
      break;
    case 0b1011:
      ret = 'B';
      break;
    case 0b1100:
      ret = 'C';
      break;
    case 0b1101:
      ret = 'D';
      break;
    case 0b1110:
      ret = 'E';
      break;
    default:
      ret = 'F';
      break;
  }
  return ret;
}
