#ifndef NCURSES_H
  #define NCURSES_H
  #include <ncurses.h> 
#endif
#ifndef UNISTD_H
  #define UNISTD_H
  #include <unistd.h>
#endif
#ifndef DEFS_H
  #define DEFS_H
  #include "defs.h"
#endif
#ifndef STRING
  #define STRING
  #include <string>
#endif
#ifndef SSTREAM
  #define NCURSES_H
  #include <sstream>
#endif

using namespace std;

int scrStartup();

int drawString(Coordinate coord, char* str);

int drawCharacter(Coordinate coord, char symbol);

int drawCharacterBold(Coordinate coord, char symbol);

int scrTearDown();

int printMessage(string message);

int clearMessageArea();
