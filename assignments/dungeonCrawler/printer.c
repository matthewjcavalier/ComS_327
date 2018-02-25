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

int drawDungeon(Dungeon* dun) {
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
  return 1;
}

int drawEntities(Character* placementMap[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH]);



/*
int main(int argc, char* argv[]) {
  startup();
  drawString(1, 2, "hello curses");
  sleep(1);

  tearDown();
  return 0;
}
*/
