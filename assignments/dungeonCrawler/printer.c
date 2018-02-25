#include "printer.h"

int startup() {
  initscr();
  noecho();
  curs_set(FALSE);
  return 0;
}

int drawString(int row, int col, char* str) {
  mvprintw(row, col, str);
  refresh();
  return 0;
}

int drawCharacter(int row, int col, char character) {
  move(row, col);
  addch(character);
  refresh();
  return 0;
}

int tearDown() {
  endwin();
  return 0;
}

int drawStandardDun(Dungeon* dun) {
  int row, col;
  char toPrint = ' ';
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      // if the tile is a border
      if(dun->map[row][col].isBorder) {
        // if the cell is in the first or last row
        if(row == 0 || row == MAX_DUNGEON_HEIGHT - 1) {
          toPrint = BORDER_HOR_CHAR;
        }
        // else the cell is one either in the first or last col
        else {
          toPrint = BORDER_VERT_CHAR;
        }
      }
      // else the tile is part of the normal dungeon
      else {
        if(dun->map[row][col].isRoom) {
          toPrint = ROOM_CHAR;
        } else if(dun->map[row][col].isHallway) {
          toPrint = HALL_CHAR;
        }
        else {
          toPrint = ' ';
        }
      }
      drawCharacter(row + 1, col, toPrint);
    }
  }
  return 0;
}

int drawCoolDun(Dungeon* dun) {
  int row, col;
  char* charToPrint;

  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
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
    }
    drawString(row + 1, col, charToPrint);
  } 
  return 0;
}

int drawDungeon(Dungeon* dun, Setup setup) {
  if(setup.useCoolChars == TRUE) {
    return drawCoolDun(dun);
  } else {
    return drawStandardDun(dun);
  }
}

int drawEntities(Character* placementMap[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]) {
  int row, col;
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      if(placementMap[row][col] != NULL) {
        drawCharacter(row + 1, col, placementMap[row][col]->symbol);
      }
    }
  }
  return 0;
}



/*
int main(int argc, char* argv[]) {
  startup();
  drawString(1, 2, "hello curses");
  sleep(1);

  tearDown();
  return 0;
}
*/
