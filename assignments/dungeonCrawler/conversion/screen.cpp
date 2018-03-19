#include "screen.h"

/**
 * @brief start up an ncurses screen
 * 
 * @return int 
 */
int scrStartup() {
  initscr();
  noecho();
  timeout(0);
  curs_set(FALSE);
  keypad(stdscr, TRUE);
  return 0;
}

/**
 * @brief draw a character string at the input location
 * 
 * @param coord 
 * @param str 
 * @return int 
 */
int drawString(Coordinate coord, char* str) {
  mvprintw(coord.y, coord.x, str);
  refresh();
  return 0;
}

/**
 * @brief draw the input character at the input location
 * 
 * @param coord 
 * @param symbol 
 * @return int 
 */
int drawCharacter(Coordinate coord, char symbol) {
  move(coord.y, coord.x);
  addch(symbol);
  refresh();
  return 0;
}

/**
 * @brief Tear down the ncurses screen
 * 
 * @return int 
 */
int scrTearDown() {
  endwin();
  return 0;
}

/**
 * @brief print the input string message in the message area at the top of the screen
 * 
 * @param message 
 * @return int 
 */
int printMessage(string message) {
  drawString({0,0}, (char*)message.c_str());
  return 0;
}

/**
 * @brief clear out the message area at the top of the screen
 * 
 * @return int 
 */
int clearMessageArea() {
  ostringstream spaces;
  for(int i = 0; i < MAX_WIDTH; i++) {
    spaces << " ";
  }
  return drawString({0,0}, (char*)spaces.str().c_str());
}
