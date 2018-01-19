#include "crawler.h"

int main(int argc, char* argv[]) {
  srand(time(0));

  Setup setup = parseArgs(argc, argv);
  
  printf("Using seed: %d\n", setup.seed);
  // seed random num gen
  srand(setup.seed);

  // set up the dungeon
  Dungeon* dungeon;
  dungeon = genDungeon();
  
  // print the dungeon
  printDungeon(dungeon, setup);
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
  int i;

  ret.seed = rand();
  ret.useCoolChars = false;
  
  // look for flags
  for(i = 0; i < argc; i++) {
    if(strcmp(argv[i], "-s") == 0) {
      if(i + 1 < argc){
        ret.seed = atoi(argv[i+1]);
      }
    }
    if(strcmp(argv[i], "-cool") == 0) {
      ret.useCoolChars = true;
    }
  }
  return ret;
}

/**
 * @brief   prints the dungeon to screen
 * 
 * @param dun     the dungeon to print
 * @param setup   a struct containg print info
 */
void printDungeon(Dungeon* dun, Setup setup) {
  int row, col;
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
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
        if(dun->map[row][col].isRoom) {
          if(setup.useCoolChars) {
            printf("%s", COOL_ROOM_CHAR);
          } else {
            printf("%c", ROOM_CHAR);
          }
        } else if(dun->map[row][col].isHallway) {
          if(setup.useCoolChars) {
            printf("%s", COOL_HALL_CHAR);
          } else {
            printf("%c", HALL_CHAR);
          }
        }

        else {
          // debugging hardness  
          //printf("%d", dun->map[row][col].hardness);

          printf(" ");
        }
      }
    }
    printf("\n");
  }
}
