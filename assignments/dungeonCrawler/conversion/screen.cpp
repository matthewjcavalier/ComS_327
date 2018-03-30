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
  start_color();
  setupColors();
  return 0;
}

int setupColors() {
  init_pair(COLOR_RED, COLOR_RED, COLOR_BLACK);
  init_pair(COLOR_GREEN, COLOR_GREEN, COLOR_BLACK);
  init_pair(COLOR_BLUE, COLOR_BLUE, COLOR_BLACK);
  init_pair(COLOR_CYAN, COLOR_CYAN, COLOR_BLACK);
  init_pair(COLOR_YELLOW, COLOR_YELLOW, COLOR_BLACK);
  init_pair(COLOR_MAGENTA, COLOR_MAGENTA, COLOR_BLACK);
  init_pair(COLOR_WHITE, COLOR_WHITE, COLOR_BLACK);
  init_pair(COLOR_BLACK, COLOR_WHITE, COLOR_BLACK);

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

int turnOnColorPair(string str) {
  if(str.compare("RED") == 0) {
    attron(COLOR_PAIR(COLOR_RED));
  } else if(str.compare("GREEN") == 0) {
    attron(COLOR_PAIR(COLOR_GREEN));
  } else if(str.compare("BLUE") == 0) {
    attron(COLOR_PAIR(COLOR_BLUE));
  } else if(str.compare("CYAN") == 0) {
    attron(COLOR_PAIR(COLOR_CYAN));
  } else if(str.compare("YELLOW") == 0) {
    attron(COLOR_PAIR(COLOR_YELLOW));
  } else if(str.compare("MAGENTA") == 0) {
    attron(COLOR_PAIR(COLOR_MAGENTA));
  } else if(str.compare("WHITE") == 0) {
    attron(COLOR_PAIR(COLOR_WHITE));
  } else if(str.compare("BLACK") == 0) {
    attron(COLOR_PAIR(COLOR_BLACK));
  }
  return 0;
}

int turnOffColorPair(string str) {
  if(str.compare("RED") == 0) {
    attroff(COLOR_PAIR(COLOR_RED));
  } else if(str.compare("GREEN") == 0) {
    attroff(COLOR_PAIR(COLOR_GREEN));
  } else if(str.compare("BLUE") == 0) {
    attroff(COLOR_PAIR(COLOR_BLUE));
  } else if(str.compare("CYAN") == 0) {
    attroff(COLOR_PAIR(COLOR_CYAN));
  } else if(str.compare("YELLOW") == 0) {
    attroff(COLOR_PAIR(COLOR_YELLOW));
  } else if(str.compare("MAGENTA") == 0) {
    attroff(COLOR_PAIR(COLOR_MAGENTA));
  } else if(str.compare("WHITE") == 0) {
    attroff(COLOR_PAIR(COLOR_WHITE));
  } else if(str.compare("BLACK") == 0) {
    attroff(COLOR_PAIR(COLOR_BLACK));
  }
  return 0;
}