#ifndef DUNGEON_H
  #define DUNGEON_H
  #include "dungeon.h"
#endif
#ifndef STDLIB_H
  #define STDLIB_H
  #include <stdlib.h>
#endif
#ifndef STRING_H
  #define STDLIB_H
  #include <string.h>
#endif
#ifndef TIME_H
  #define TIME_H
  #include <time.h>
#endif
#ifndef IOSTREAM
  #define IOSTREAM
  #include <iostream>
#endif
#ifndef STRING
  #define STRING
  #include <string>
#endif

class Settings{
  public:
    int seed;
    bool load;
    bool save;
    string loadSaveLoc;
};

Dungeon dungeonInit();

void setSettings(int argc, char* argv[]);
