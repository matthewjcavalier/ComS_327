#ifndef NCURSES_H
  #define NCURSES_H
  #include <ncurses.h> 
#endif
#ifndef UNISTD_H
  #define UNISTD_H
  #include <unistd.h>
#endif
#ifndef DUNGEON_H
  #define DUNGEON_H
  #include "dungeon.h"
#endif

int startup();

int drawString(int row, int col, char* str);

int drawCharacter(int row, int col, char character);

int tearDown();

int drawDungeon(Dungeon* dun);

int drawEntities(Character* placementMap[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]);

