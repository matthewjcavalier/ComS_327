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


#define BORDER_HOR_CHAR '-'
#define BORDER_VERT_CHAR '|'
#define ROOM_CHAR '.'
#define HALL_CHAR '#'

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
} Setup;

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
void printDungeon(Dungeon* dun, Setup setup);

/**
 * @brief Prints the dungeon in the standard format
 *        as perscibed in the homework description.
 * 
 * @param dun a pointer to the dungeon to print
 */
void printStandardDun(Dungeon* dun);

/**
 * @brief Prints the dungeon in a cool way using
 *        unicode characters for a better looking
 *        dungeon map
 * 
 * @param dun a pointer to the dungeon to print
 */
void printCoolDun(Dungeon* dun);
