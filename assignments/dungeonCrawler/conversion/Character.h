#ifndef IOSTREAM
  #define IOSTREAM
  #include <iostream>
#endif
#ifndef DUNGEON_H
  #define DUNGEON_H
  #include "dungeon.h"
#endif
#ifndef FUNCTIONAL
  #define FUNCTIONAL
  #include <functional>
#endif
#ifndef UNISTD_H
  #define UNISTD_H
  #include <unistd.h>
#endif

class Dungeon;

class Character {
  public:
    Coordinate coord;
    char symbol;
    int speed;
    int nextEventTime;
    Dungeon* dun;
    int id;

    Character();
    virtual int takeTurn();
    int moveRand();
    int moveTo(Coordinate coord);
    int moveToward(Coordinate coord);
    int getCharacterId(Coordinate loc);
};

class PC : public Character {
  public:
    PC(int id, Coordinate coord, int speed, Dungeon* dun, int nextEventTime);
    int takeTurn();
};

class NPC : public Character {
  public:
    char type;
    Coordinate lastSeenPCLoc;
    NPC(int id, Coordinate coord, int speed, Dungeon* dun, int nextEventTime, char type, PC* pc);
    int takeTurn();
  private:
    PC* pc;
    int (NPC::*turnLogic)();
    void setTurnLogic();
    void updatePCLoc();
    int movement0001();
    int movement0010();
    int movement0011();
    int movement0100();
    int movement0101();
    int movement0110();
    int movement0111();
    int movement1000();
    int movement1001();
    int movement1010();
    int movement1011();
    int movement1100();
    int movement1101();
    int movement1110();
    int movement1111();

    int moveRandTunneling();
    int moveTowardTunneling(Coordinate coord);
};

char getSymbol(char type);
