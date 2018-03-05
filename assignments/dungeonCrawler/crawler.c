#include "crawler.h"

int maxNumMonsters = 0;


int main(int argc, char* argv[]) {

srand(time(0));

  Setup setup = parseArgs(argc, argv);
  
  printf("Using seed: %d\n", setup.seed);
  printf("Number of Monsters: %d\n", setup.numMonsters);
  maxNumMonsters = setup.numMonsters;
  // seed random num gen
  srand(setup.seed);

  // set up the dungeon
  Dungeon* dungeon = malloc(sizeof(*dungeon));
  if(setup.load){
    loadDungeon(setup.saveLoadLocation, dungeon);
  } else {
    genDungeon(dungeon);
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
  Coordinate stairLoc;
  //bool rebuild = FALSE;

  // clean placement Map
  cleanPlacementMap(placementMap);

  // setup PC
  randomlyPlace(&pcCharacter->coord, dun);
  pcCharacter->symbol = PC_CHAR;
  pcCharacter->speed = 10;
  pcCharacter->npc = NULL;
  pcCharacter->pc = &pc;
  pcCharacter->nextEventTime = 1000/pcCharacter->speed + currentTurn;


  // place the stairs
  stairLoc = getEmptySpot(dun, placementMap);

  dun->map[stairLoc.row][stairLoc.col].isDownstairs = TRUE;

  stairLoc = getEmptySpot(dun, placementMap);

  dun->map[stairLoc.row][stairLoc.col].isUpstairs = TRUE;

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
    // else is pc
    else {
      endGame = (pc_routine(currentChar, turnQueue, dun, placementMap, setup) == 1) ? TRUE : FALSE;
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

    if(endGame) {
      tearDown();
    }

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

/**
 * @brief   checks to see if the monster has line of sight to the PC
 * 
 * @param pc  pointer to Player character
 * @param monster   pointer to monster
 * @param dun   current dungeon
 * @return true   if the monster has line of sight
 * @return false  if the monster doesn't have line of sight
 */
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

/**
 * @brief get the coordinate that is one tile away when
 *        trying to move to a coordinate from the other coordinate
 * 
 * @param from  where we are moving from
 * @param to    where we are moving to
 * @return Coordinate   the coordinate to move to
 */
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

/**
 * @brief Look at the hardness map and decide where to move next (character can tunnel)
 * 
 * @param map     hardness map to PC
 * @param coord   current coordinate
 * @param dun     the current dungeon
 * @return Coordinate   where the character is going to move to
 */
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

/**
 * @brief Look at the distance map and decide where the character will go next (can't tunnel)
 * 
 * @param map     distance map to PC
 * @param coord   current coordinate
 * @return Coordinate where the character will move to 
 */
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

/**
 * @brief All non-erratic movement routines for the monsters
 * 
 * @param character   the monster who's turn it is
 * @param turnQueue   the queue with all of the characters
 * @param dun         the dungeon that is being used
 * @param map           the map that has all of the characters in it
 * @param openSpaceMap  the map with the distances for the non tunneling monsters
 * @param tunnelingMap  the map with the distances for the tunneling monsters
 * @param pc            a pointer to the player character
 * @param characteristics   the character bits that indicated the characteristics of the monster
 */
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

/**
 * @brief main monster routine that will make the monster do an action
 *        depending on the monster's characteristics
 * 
 * @param character     the monster character
 * @param turnQueue     the queue that has all of the characters
 * @param dun           the dungeon being worked on
 * @param map           the map that has all of the the characters
 * @param openSpaceMap  a distance map for nontunneling monsters
 * @param tunnelingMap  a distance map for tunneling monsters
 * @param pc            a pointer to the PC
 */
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

/**
 * @brief Gets the string that can describe the monster location
 * 
 * @param ydiff   difference in the y axis between the monster and PC (positive indicates north, negative indicates south)
 * @param xdiff   difference in the x axis between the monster and PC (positive indicates west, negative indicates east)
 * @param sym     symbol to use for the monster
 * @return char*  the string that was requested
 */
char* getMonsterLocString(int ydiff, int xdiff, char sym) {
  char* ret = malloc(sizeof(char) * 50);
  // if on same y axis
  if(ydiff == 0) {
    sprintf(ret, "%c is %d to the %s", sym, abs(xdiff), (xdiff > 0) ? "West" : "East");
  }
  // if on same x axis
  else if(xdiff == 0) {
    sprintf(ret, "%c is %d to the %s", sym, abs(ydiff), (ydiff > 0) ? "North" : "South");
  } else {
    sprintf(ret, "%c is %d to the %s and %d to the %s", sym, abs(ydiff), (ydiff > 0) ? "North" : "South", abs(xdiff), (xdiff > 0) ? "West" : "East");
  }
  return ret;
}

/**
 * @brief The routine for showing the monster list as an overlay on the
 *        dungeon game
 * 
 * @param pc    pointer to the PC
 * @param map   the map with all of the characters
 * @return int  success indicator
 */
int monsterList_routine(Character* pc, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]) {
  bool notEnd = TRUE;
  char* monsterLocs[maxNumMonsters];
  int topLeft_x = 10;
  int topLeft_y = 5;
  int vertBorderWidth = 2;
  int horBorderWith = 5;
  int stringAreaWidth = 50;
  int xdiff, ydiff;
  int currentTopMonst = 0;
  int row, col, i;
  int numMonstersShowing = 5;
  int monstersFound = 0;
  int userPressed;

  // init monster list
  for(i = 0; i < maxNumMonsters; i++) {
    monsterLocs[i] = malloc(sizeof(char) * stringAreaWidth);
    monsterLocs[i] = NULL;
  }
  
  // get monster list
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      if(map[row][col] != NULL && map[row][col]->npc != NULL) {
        xdiff = pc->coord.col - map[row][col]->coord.col;
        ydiff = pc->coord.row - map[row][col]->coord.row;
        monsterLocs[monstersFound] = getMonsterLocString(ydiff, xdiff, map[row][col]->symbol);
        monstersFound++;
      }
    }
  }

  do {
    // draw cleared box
    for(row = topLeft_y; row < topLeft_y + horBorderWith + numMonstersShowing - 1; row++) {
      for(col = topLeft_x; col < topLeft_x + vertBorderWidth + stringAreaWidth - 1; col++) {
        if(row == topLeft_y || row == topLeft_y + horBorderWith + numMonstersShowing - 2) {
          drawCharacter(row + 1, col, '_');
        } else if(col == topLeft_x || col == topLeft_x + vertBorderWidth + stringAreaWidth - 2) {
          drawCharacter(row + 1, col, '|');
        } else {
          drawCharacter(row + 1, col, ' ');
        }
      }
    }
    for(i = currentTopMonst; i < monstersFound && i < currentTopMonst + numMonstersShowing; i++) {
      drawString(topLeft_y + i - currentTopMonst + vertBorderWidth + 1, topLeft_x + horBorderWith, monsterLocs[i]);
      fflush(stdout);
    }
    do {
      userPressed = getch();
      if(userPressed != ERR) {
        // wait a microsecond and if no other characters came we know esc was pressed
          if(userPressed == 2) {
            drawString(0,0, "DOWN pressed");
            if(monstersFound > numMonstersShowing) {
              currentTopMonst = (currentTopMonst < monstersFound - numMonstersShowing)? currentTopMonst + 1 : monstersFound - numMonstersShowing;
            }
          } else if(userPressed == 3) {
            drawString(0,0, "UP pressed");
            if(monstersFound > numMonstersShowing) {
              currentTopMonst = (currentTopMonst > 0)? currentTopMonst - 1 : 0;
            }
          } else if(userPressed == 27) {
            drawString(0,0, "ESC pressed");
            notEnd = FALSE;
          }
      }
    } while(userPressed == ERR);
  } while(notEnd);
  drawString(0,0,"                                            ");
  return 0;
}

/**
 * @brief The rules for what happens during a player's turn
 * 
 * @param character   the PC
 * @param turnQueue   the queue with the characters
 * @param dun         the dungeon being worked with
 * @param map         the map that has all characters
 * @param setup       the struct that has startup settings
 * @return int        success indicator
 */
int pc_routine(Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH], Setup setup) {
  char userPressed;
  bool commandComplete = FALSE;
  
  do {
    userPressed = getch();
    drawCharacter(0,0, userPressed);
    // move up left
    if(userPressed == '7' || userPressed == 'y'){
      commandComplete = !moveUpLeft(character, turnQueue, dun, map, FALSE, NULL);
    }
    // move up
    if(userPressed == '8' || userPressed == 'k'){
      commandComplete = !moveUp(character, turnQueue, dun, map, FALSE, NULL);
    }
    // move up right
    if(userPressed == '9' || userPressed == 'u'){
      commandComplete = !moveUpRight(character, turnQueue, dun, map, FALSE, NULL);
    }
    // move down right
    if(userPressed == '3' || userPressed == 'n'){
      commandComplete = !moveDownRight(character, turnQueue, dun, map, FALSE, NULL);
    }
    // move down
    if(userPressed == '2' || userPressed == 'j'){
      commandComplete = !moveDown(character, turnQueue, dun, map, FALSE, NULL);
    }
    // move down left
    if(userPressed == '1' || userPressed == 'b'){
      commandComplete = !moveDownLeft(character, turnQueue, dun, map, FALSE, NULL);
    }
    // move left
    if(userPressed == '4' || userPressed == 'h'){
      commandComplete = !moveLeft(character, turnQueue, dun, map, FALSE, NULL);
    }
    if(userPressed == '6' || userPressed == 'l') {
      commandComplete = !moveRight(character, turnQueue, dun, map, FALSE, NULL);
    }
    // try to go down stairs
    if(userPressed == '>'){
      if(dun->map[character->coord.row][character->coord.col].isDownstairs) {
         drawString(0,0,"trying to go downstairs");       
         changeLevel(character, turnQueue, dun, map,setup);
      }
    }
    // try to go up stairs
    if(userPressed == '<'){
      if(dun->map[character->coord.row][character->coord.col].isUpstairs) {
        drawString(0,0,"trying to go upstairs");
         changeLevel(character, turnQueue, dun, map,setup);
      }
    }
    // rest for a turn
    if(userPressed == '5' || userPressed == ' '){
      commandComplete = TRUE;
    }
    // show monster list
    if(userPressed == 'm'){
      monsterList_routine(character, map);
      drawDungeon(dun, setup);
      drawEntities(map);
    }
    // quit game
    if(userPressed == 'Q'){
      commandComplete = TRUE;
      return 1;
    }
  } while(!commandComplete);
  return 0;
}

/**
 * @brief Make a new dungeon level placeing the monsters and PC in it
 * 
 * @param character the PC
 * @param turnQueue the queue with all characters (still populated)
 * @param dun       the dungeon being used
 * @param map       the map that has all characters (still populated)
 * @param setup     the setup struct from the program startup
 */
void changeLevel(Character* character, MinHeap* turnQueue, Dungeon* dun,Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH], Setup setup) {
  Character* currentChar;
  Coordinate stairLoc;
  int i;
  NPC* currentNPC;
  int currentTurn = character->nextEventTime;

  // clear out the turn queue
  while(!isHeapEmpty(turnQueue)) {
    currentChar = removeFromHeap(turnQueue);

    // deconstruct character
    free(currentChar->npc);
    free(currentChar);
  }
  
  // generate the new dungeon level
  genDungeon(dun);
  cleanPlacementMap(map);
  randomlyPlace(&character->coord, dun);

  // place the stairs
  stairLoc = getEmptySpot(dun, map);

  dun->map[stairLoc.row][stairLoc.col].isDownstairs = TRUE;

  stairLoc = getEmptySpot(dun, map);

  dun->map[stairLoc.row][stairLoc.col].isUpstairs = TRUE;

  addToHeap(turnQueue, character);

  map[character->coord.row][character->coord.col] = character;
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

    currentChar = malloc(sizeof(*currentChar));
    currentChar->coord = getEmptySpot(dun, map);
    currentChar->symbol = getSymbol(currentNPC->characteristics);
    currentChar->pc = NULL;
    currentChar->npc = currentNPC;
    currentChar->speed = rand() % 15 + 5;
    currentChar->nextEventTime = currentTurn + 1000/currentChar->speed;
    map[currentChar->coord.row][currentChar->coord.col] = currentChar;
    addToHeap(turnQueue, currentChar);
  }

  drawDungeon(dun, setup);

  drawEntities(map);
}

/**
 * @brief Attempt to move the character up left
 * 
 * @param character   the character being worked on
 * @param turnQueue   the turn queue that has all characters
 * @param dun         the dungeon being used
 * @param map         the map that has all characters
 * @param canTunnel   if the character can tunnel
 * @param hardnessMap the hardness map of the dungeon
 * @return true       if the character was unable to move
 * @return false      if the character was able to move
 */
bool moveUpLeft(Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH], bool canTunnel, int** hardnessMap) {
  bool notPlaced = TRUE;
  Coordinate movingTo;

  movingTo = character->coord;
  movingTo.row = movingTo.row -1;
  movingTo.col = movingTo.col -1;
  if(canTunnel) {
    notPlaced = moveCharacterTunnel(movingTo, character, turnQueue, dun, map, hardnessMap);
  } else {
    notPlaced = moveCharacterNoTunnel(movingTo, character, turnQueue, dun, map);
  }
  return notPlaced;
}

/**
 * @brief Attempt to move the character up
 * 
 * @param character   the character being worked on
 * @param turnQueue   the turn queue that has all characters
 * @param dun         the dungeon being used
 * @param map         the map that has all characters
 * @param canTunnel   if the character can tunnel
 * @param hardnessMap the hardness map of the dungeon
 * @return true       if the character was unable to move
 * @return false      if the character was able to move
 */
bool moveUp(Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH], bool canTunnel, int** hardnessMap) {
  bool notPlaced = TRUE;
  Coordinate movingTo;

  movingTo = character->coord;
  movingTo.row = movingTo.row -1;
  if(canTunnel) {
    notPlaced = moveCharacterTunnel(movingTo, character, turnQueue, dun, map, hardnessMap);
  } else {
    notPlaced = moveCharacterNoTunnel(movingTo, character, turnQueue, dun, map);
  }
  return notPlaced;
}

/**
 * @brief Attempt to move the character up right
 * 
 * @param character   the character being worked on
 * @param turnQueue   the turn queue that has all characters
 * @param dun         the dungeon being used
 * @param map         the map that has all characters
 * @param canTunnel   if the character can tunnel
 * @param hardnessMap the hardness map of the dungeon
 * @return true       if the character was unable to move
 * @return false      if the character was able to move
 */
bool moveUpRight(Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH], bool canTunnel, int** hardnessMap) {
  bool notPlaced = TRUE;
  Coordinate movingTo;

  movingTo = character->coord;
  movingTo.row = movingTo.row -1;
  movingTo.col = movingTo.col +1;
  if(canTunnel) {
    notPlaced = moveCharacterTunnel(movingTo, character, turnQueue, dun, map, hardnessMap);
  } else {
    notPlaced = moveCharacterNoTunnel(movingTo, character, turnQueue, dun, map);
  } 
  return notPlaced;
}

/**
 * @brief Attempt to move the character right
 * 
 * @param character   the character being worked on
 * @param turnQueue   the turn queue that has all characters
 * @param dun         the dungeon being used
 * @param map         the map that has all characters
 * @param canTunnel   if the character can tunnel
 * @param hardnessMap the hardness map of the dungeon
 * @return true       if the character was unable to move
 * @return false      if the character was able to move
 */
bool moveRight(Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH], bool canTunnel, int** hardnessMap) {
  bool notPlaced = TRUE;
  Coordinate movingTo;

  movingTo = character->coord;
  movingTo.col = movingTo.col +1;
  if(canTunnel) {
    notPlaced = moveCharacterTunnel(movingTo, character, turnQueue, dun, map, hardnessMap);
  } else {
    notPlaced = moveCharacterNoTunnel(movingTo, character, turnQueue, dun, map);
  }
  return notPlaced;
}

/**
 * @brief Attempt to move the character down right
 * 
 * @param character   the character being worked on
 * @param turnQueue   the turn queue that has all characters
 * @param dun         the dungeon being used
 * @param map         the map that has all characters
 * @param canTunnel   if the character can tunnel
 * @param hardnessMap the hardness map of the dungeon
 * @return true       if the character was unable to move
 * @return false      if the character was able to move
 */
bool moveDownRight(Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH], bool canTunnel, int** hardnessMap) {
  bool notPlaced = TRUE;
  Coordinate movingTo;

  movingTo = character->coord;
  movingTo.row = movingTo.row +1;
  movingTo.col = movingTo.col +1;
  if(canTunnel) {
    notPlaced = moveCharacterTunnel(movingTo, character, turnQueue, dun, map, hardnessMap);
  } else {
    notPlaced = moveCharacterNoTunnel(movingTo, character, turnQueue, dun, map);
  }
  return notPlaced;
}

/**
 * @brief Attempt to move the character down
 * 
 * @param character   the character being worked on
 * @param turnQueue   the turn queue that has all characters
 * @param dun         the dungeon being used
 * @param map         the map that has all characters
 * @param canTunnel   if the character can tunnel
 * @param hardnessMap the hardness map of the dungeon
 * @return true       if the character was unable to move
 * @return false      if the character was able to move
 */
bool moveDown(Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH], bool canTunnel, int** hardnessMap) {
  bool notPlaced = TRUE;
  Coordinate movingTo;

  movingTo = character->coord;
  movingTo.row = movingTo.row +1;
  if(canTunnel) {
    notPlaced = moveCharacterTunnel(movingTo, character, turnQueue, dun, map, hardnessMap);
  } else {
    notPlaced = moveCharacterNoTunnel(movingTo, character, turnQueue, dun, map);
  }
  return notPlaced;
}

/**
 * @brief Attempt to move the character down left
 * 
 * @param character   the character being worked on
 * @param turnQueue   the turn queue that has all characters
 * @param dun         the dungeon being used
 * @param map         the map that has all characters
 * @param canTunnel   if the character can tunnel
 * @param hardnessMap the hardness map of the dungeon
 * @return true       if the character was unable to move
 * @return false      if the character was able to move
 */
bool moveDownLeft(Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH], bool canTunnel, int** hardnessMap) {
  bool notPlaced = TRUE;
  Coordinate movingTo;

  movingTo = character->coord;
  movingTo.row = movingTo.row +1;
  movingTo.col = movingTo.col -1;
  if(canTunnel) {
    notPlaced = moveCharacterTunnel(movingTo, character, turnQueue, dun, map, hardnessMap);
  } else {
    notPlaced = moveCharacterNoTunnel(movingTo, character, turnQueue, dun, map);
  }
  return notPlaced;
}
/**
 * @brief Attempt to move the character left
 * 
 * @param character   the character being worked on
 * @param turnQueue   the turn queue that has all characters
 * @param dun         the dungeon being used
 * @param map         the map that has all characters
 * @param canTunnel   if the character can tunnel
 * @param hardnessMap the hardness map of the dungeon
 * @return true       if the character was unable to move
 * @return false      if the character was able to move
 */
bool moveLeft(Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH], bool canTunnel, int** hardnessMap) {
  bool notPlaced = TRUE;
  Coordinate movingTo;

  movingTo = character->coord;
  movingTo.col = movingTo.col -1;
  if(canTunnel) {
    notPlaced = moveCharacterTunnel(movingTo, character, turnQueue, dun, map, hardnessMap);
  } else {
    notPlaced = moveCharacterNoTunnel(movingTo, character, turnQueue, dun, map);
  }
  return notPlaced;
}

/**
 * @brief try to move the character in a random direction until the character
 *        successfully does something
 * 
 * @param character the character being worked on
 * @param turnQueue the turn queue that has all characters
 * @param dun   the dungeon being used
 * @param map   the map that has all characters
 * @param canTunnel   if the character can tunnel
 * @param hardnessMap the hardness map of the dungeon
 * @return int return indicator
 */
int moveRandomly(Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH], bool canTunnel, int** hardnessMap) {
  bool notPlaced = TRUE;
  Coordinate movingTo;
  Coordinate movingFrom = character->coord;

  while(notPlaced) {
    switch(rand() % 8) {
      case 0:
        notPlaced = moveUpLeft(character, turnQueue, dun, map, canTunnel, hardnessMap);
        break;
      case 1:
        notPlaced = moveUp(character, turnQueue, dun, map, canTunnel, hardnessMap);
        break;
      case 2:
        notPlaced = moveUpRight(character, turnQueue, dun, map, canTunnel, hardnessMap);
        break;
      case 3:
        notPlaced = moveRight(character, turnQueue, dun, map, canTunnel, hardnessMap);
        break;
      case 4:
        notPlaced = moveDownRight(character, turnQueue, dun, map, canTunnel, hardnessMap);
        break;
      case 5:
        notPlaced = moveDown(character, turnQueue, dun, map, canTunnel, hardnessMap);
        break;
      case 6:
        notPlaced = moveDownLeft(character, turnQueue, dun, map, canTunnel, hardnessMap);
        break;
      default:
        notPlaced = moveLeft(character, turnQueue, dun, map, canTunnel, hardnessMap);
        break;
    }
  }
  movingTo = character->coord;
  drawCharacter(movingFrom.row + 1, movingFrom.col, getDunChar(dun->map[movingFrom.row][movingFrom.col], movingFrom.row, movingFrom.col));
  drawCharacter(movingTo.row + 1, movingTo.col, character->symbol);
  return 0;
}

/**
 * @brief Attempt to move the character to the given coodinate
 * 
 * @param movingTo  the coordinate the character is moving to
 * @param character   the character being moved
 * @param turnQueue   the turn queue for the characters
 * @param dun   the dungeon being used
 * @param map   the map that contains all characters
 * @return true if the character was able to move
 * @return false if the character was unable to move
 */
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

/**
 * @brief Attempt to move the character to the given Coordinate, allowing
 *        for the character to try to tunnel through rock
 * 
 * @param movingTo  the coordinate the character is moving to
 * @param character   the character being moved
 * @param turnQueue   the turn queue for the characters
 * @param dun   the dungeon being used
 * @param map   the map that contains all characters
 * @param hardnessMap   the hardness map of the dungeon
 * @return true   if the character was able to move in that direction or start digging
 * @return false  if the character was unable to do andything
 */
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

/**
 * @brief checkes if the dungeon has an open spot at the given coordinate
 * 
 * @param coord  the coordinate being looked at
 * @param dun   the dungeon being looked at
 * @return true   if the spot is open
 * @return false  if the spot is not open
 */
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
/**
 * @brief Cleans the input map by seeting all indicies to NULL
 * 
 * @param map  the map to be cleaned
 */
void cleanPlacementMap(Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]) {
   int row, col;
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      map[row][col] = NULL;
    }
  }
 }
/**
 * @brief Looks through both the dungeon and Character placment map to
 *        find an unocupied spot that is in an open space (room or hall)
 * 
 * @param dun  the dungeon being worked with
 * @param placementMap a 2d array that has all characters
 * @return Coordinate the coordinate that is open
 */
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
/**
 * @brief This function gets the sysmbol that represents a monster
 *        based upon the characteristics that the monster poseses
 * 
 * @param ristics the characteristics of the monster
 * @return char the character that represents the mosnster
 */
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
