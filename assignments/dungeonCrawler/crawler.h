#include <time.h>
#include <string.h>
#include "generator.h"


#define BORDER_HOR_CHAR '-'
#define BORDER_VERT_CHAR '|'
#define ROOM_CHAR '.'
#define HALL_CHAR '#'

#define COOL_ROOM_CHAR "\u26c6" //  ⛆ 
#define COOL_HALL_CHAR "\u26da" // ⛚

typedef struct {
  int seed;
  boolean useCoolChars;
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

