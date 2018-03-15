#ifndef IOSTREAM
  #define IOSTREAM
  #include <iostream>
#endif
#ifndef DUNGEON_H
  #define DUNGEON_H
  #include "dungeon.h"
#endif

class Character {
  public:
    Coordinate coord;
    char symbol;
    int speed;
    int nextEventTime;
    Dungeon dun;

    Character();
    virtual void takeTurn();
};

class PC : public Character {
  public:
    PC(Coordinate coord, int speed, Dungeon& dun, int nextEventTime);
    void takeTurn();
};

class NPC : public Character {
  public:
    char type;
    NPC(Coordinate coord, int speed, Dungeon& dun, int nextEventTime, char type);
    void takeTurn();
};
