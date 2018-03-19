#include "screen.h"


int scrStartup() {
  initscr();
  noecho();
  timeout(0);
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

int drawCharacterBold(Coordinate coord, char symbol) {
  move(coord.y, coord.x);
  addch(symbol);
  refresh();
  return 0;
}

int scrTearDown() {
  endwin();
  return 0;
}

int printMessage(string message) {
  drawString({0,0}, (char*)message.c_str());
  return 0;
}

int clearMessageArea() {
  ostringstream spaces;
  for(int i = 0; i < MAX_WIDTH; i++) {
    spaces << " ";
  }
  return drawString({0,0}, (char*)spaces.str().c_str());
}
