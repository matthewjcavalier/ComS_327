#include "screen.h"


int scrStartup() {
  initscr();
  noecho();
  curs_set(FALSE);
  keypad(stdscr, TRUE);
  return 0;
}

int drawString(Coordinate coord, char* str) {
  mvprintw(coord.y, coord.x, str);
  refresh();
  return 0;
}

int drawCharacter(Coordinate coord, char symbol) {
  move(coord.y, coord.x);
  addch(symbol);
  refresh();
  return 0;
}


int scrTearDown() {
  endwin();
  return 0;
}

