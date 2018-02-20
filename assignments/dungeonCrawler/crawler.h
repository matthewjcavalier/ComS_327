#ifndef TIME_H
  #define TIME_H
  #include <time.h>
#endif
#ifndef STRING_H
  #define STRING_H
  #include <string.h>
#endif
#ifndef STDLIB_H
  #define STDLIB_H
  #include <stdlib.h>
#endif
#ifndef GENERATOR_H
  #define GENERATOR_H
  #include "generator.h"
#endif
#ifndef PATHFINDING_H
  #define PATHFINDING_H
  #include "pathFinding.h"
#endif
#ifndef HEAP_H
  #define HEAP_H
  #include "heap.h"
#endif
#ifndef UNISTD_H
  #define UNISTD_H
  #include <unistd.h>
#endif


#define BORDER_HOR_CHAR '-'
#define BORDER_VERT_CHAR '|'
#define ROOM_CHAR '.'
#define HALL_CHAR '#'
#define PC_CHAR '@'

//#define COOL_ROOM_CHAR            "\u26c6"  //  ⛆ 
#define COOL_ROOM_CHAR            "\u2593"  //  ▓
//#define COOL_HALL_CHAR            "\u26da"  //  ⛚
#define COOL_HALL_CHAR            "\u2592"  //  ▒
#define COOL_ROCK                 "\u2591"  //  ░
#define COOL_BORDER_TOP_LEFT      "\u2554"  // 	╔
#define COOL_BORDER_TOP_RIGHT     "\u2557"  //  ╗
#define COOL_BORDER_BOTTOM_LEFT   "\u255a"  //  ╚
#define COOL_BORDER_BOTTOM_RIGHT  "\u255d"  //  ╝
#define COOL_BORDER_VERT          "\u2551"  //  ║
#define COOL_BORDER_HOR           "\u2550"  //  ═

typedef struct {
  int seed;
  boolean useCoolChars;
  boolean save;
  boolean load;
  char* saveLoadLocation;
  int numMonsters;
} Setup;

/**
 * @brief The function where the main game loop lives
 * 
 * @param dun     The dungeon that the game will use
 * @param setup   a structure that holds settings
 */
void runGame(Dungeon* dun, Setup setup);

/**
 * @brief   A function that randomly places assigns a
 *          location to the input coord such that the
 *          location is in a hallway or room
 * 
 * @param cord 
 * @param dun 
 */
void randomlyPlace(Coordinate* coord, Dungeon* dun);

/**
 * @brief     Generates a new seed using the positions'
 *            of the rooms in the dungeon
 * 
 * @param dun  The dungeon that is being used to
 *             update the seed
 */
void updateSeed(Dungeon* dun);

/**
 * @brief       Prints the distance map that was input
 *              as well as the player character in the
 *              map
 * 
 * @param map   The map being printed
 * @param pc    The player character
 */
void printPathMap(int** map, Player* pc);

/**
 * @brief   Looks at the startup arguments to set flags
 *          for how the game should run
 * 
 * @param argc    the number of arguments
 * @param argv    the vector containing the arguments
 * @return Setup  a struct that contains the things options set
 */
Setup parseArgs(int argc, char* argv[]);

/**
 * @brief   prints the dungeon to screen
 * 
 * @param dun     the dungeon to print
 * @param setup   a struct containg print info
 */
void printDungeon(Dungeon* dun, Setup setup, Character* placementMap[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]);

/**
 * @brief Prints the dungeon in the standard format
 *        as perscibed in the homework description.
 * 
 * @param dun a pointer to the dungeon to print
 */
void printStandardDun(Dungeon* dun, Character* placementMap[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]);

/**
 * @brief Prints the dungeon in a cool way using
 *        unicode characters for a better looking
 *        dungeon map
 * 
 * @param dun a pointer to the dungeon to print
 */
void printCoolDun(Dungeon* dun, Character* placementMap[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]);

void cleanPlacementMap(Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]);

Coordinate getEmptySpot(Dungeon* dun, Character* placementMap[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]);
char getSymbol(char ristics);

void pc_routine(Character* character, MinHeap* turnQueue, Dungeon* dun, Character* map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]);

boolean isEmptySpace(Coordinate coord, Dungeon* dun, Character* entityMap[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]);
