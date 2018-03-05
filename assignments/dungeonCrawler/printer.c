#include "printer.h"

/**
 * @brief  stars up ncurses and the main window
 * 
 * @return int 
 */
int startup() {
  initscr();
  noecho();
  curs_set(FALSE);
  keypad(stdscr, TRUE);
  return 0;
}

/**
 * @brief   draws a string at the given row col location
 * 
 * @param row   row to draw in
 * @param col   column to start drawing at
 * @param str   character string that is to be printed
 * @return int  status indicator
 */
int drawString(int row, int col, char* str) {
  mvprintw(row, col, str);
  refresh();
  return 0;
}

/**
 * @brief  draws a character at the given row col location
 * 
 * @param row   row to draw at
 * @param col   col to draw at
 * @param character   character to draw
 * @return int  status indicator
 */
int drawCharacter(int row, int col, char character) {
  move(row, col);
  addch(character);
  refresh();
  return 0;
}

/**
 * @brief  properly tears down the window
 * 
 * @return int status indicator
 */
int tearDown() {
  endwin();
  return 0;
}

/**
 * @brief   draws the standard dungeon using characters
 * 
 * @param dun   dungeon to draw
 * @return int  status indicator
 */
int drawStandardDun(Dungeon* dun) {
  int row, col;
  char toPrint = ' ';
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      toPrint = getDunChar(dun->map[row][col], row, col);
      drawCharacter(row + 1, col, toPrint);
    }
  }
  return 0;
}

/**
 * @brief  TODO fix this abomination so that we can
 *         make dungeon drawing great again :P
 * 
 * @param dun   dungeon to draw
 * @return int  status indicator
 */
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

/**
 * @brief   main dungeon drawing handler, will choose to draw
 *          in the standard or cool manner
 * 
 * @param dun     dungeon to draw
 * @param setup   setup struct that tells which way to draw
 * @return int    status indicator
 */
int drawDungeon(Dungeon* dun, Setup setup) {
  if(setup.useCoolChars == TRUE) {
    return drawCoolDun(dun);
  } else {
    return drawStandardDun(dun);
  }
}

/**
 * @brief   draws the entities in the input character map to screen
 * 
 * @param placementMap the map that has the characters* to draw
 * @return int  status indicator
 */
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

/**
 * @brief Figures out which character should be returned
 *        based upon the input tile
 * 
 * @param tile  the tile being looked at
 * @param row   the row that the tile exists at
 * @param col   the col that the tile exists at
 * @return char the character that corresponds with the tile
 */
char getDunChar(Tile tile, int row, int col) {
  char toPrint;
  // if the tile is a border
  if(tile.isBorder) {
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
    if(tile.isUpstairs) {
      toPrint = '<';
    } else if(tile.isDownstairs) {
      toPrint = '>';
    } else if(tile.isRoom) {
      toPrint = ROOM_CHAR;
    } else if(tile.isHallway) {
      toPrint = HALL_CHAR;
    }
    else {
      toPrint = ' ';
    }
  }
  return toPrint;
}
