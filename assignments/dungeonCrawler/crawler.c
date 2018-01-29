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
  
  // print the dungeon
  printDungeon(dungeon, setup);

  if(setup.save) {
    saveDungeon(dungeon, setup.saveLoadLocation);
    printf("done\n");
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
  int i;

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
    ret.saveLoadLocation = malloc((strlen(getenv("HOME") + strlen("/.rlg327/dungeon") + 1)) * sizeof(char));
    strcpy(ret.saveLoadLocation, getenv("HOME"));
    ret.saveLoadLocation = strcat(ret.saveLoadLocation, "/.rlg327/dungeon");
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
  if(setup.useCoolChars) {
    printCoolDun(dun);
  } else {
    printStandardDun(dun);
  }
}

void printStandardDun(Dungeon* dun) {
  int row, col;
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      switch(dun->map[row][col].type) {
        case border:
          // if the cell is in the first or last row
          if(row == 0 || row == MAX_DUNGEON_HEIGHT - 1) {
            printf("%c", BORDER_HOR_CHAR);
          }
          // else the cell is one either in the first or last col
          else {
            printf("%c", BORDER_VERT_CHAR);
          }  
          break;
        case room:
          printf("%c", ROOM_CHAR);
          break;
        case hall:
          printf("%c", HALL_CHAR);
          break;
        case rock:
          printf(" ");
          break;
      }
    }
    printf("\n");
  }
}

void printCoolDun(Dungeon* dun) {
  int row, col;
  char* charToPrint;

  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {

      switch(dun->map[row][col].type) {
        case border:
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
          break;
        case room:
          charToPrint = COOL_ROOM_CHAR;
          break;
        case hall:
          charToPrint = COOL_HALL_CHAR;
          break;
        case rock:
          charToPrint = COOL_ROCK;
          break;
      }
      printf("%s", charToPrint);
    }
    printf("\n");
  }
}
