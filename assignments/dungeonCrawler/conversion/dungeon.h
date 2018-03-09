#ifndef STDLIB_H
  #define STDLIB_H
  #include <stdlib.h>
#endif
#ifndef VECTOR
  #define VECTOR
  #include <vector>
#endif
#ifndef DEFS_H
  #define DEFS_H
  #include "defs.h"
#endif
#ifndef IOSTREAM
  #define IOSTREAM
  #include <iostream>
#endif


using namespace std;

enum TileType {
  ROCK,
  HALL,
  ROOM,
  BORDER
};

class Tile {
  public:
    TileType type;
    int hardness;

    Tile(TileType type);
    void makeBorder();
    void setType(TileType);
};

class Room {
  public:
    int x;
    int y;
    int height;
    int width;
    Room();
    Room(int y, int x, int height, int width);
};

class Dungeon {
  public:
    vector <vector<Tile>> map;

    Dungeon(); 
    void draw();

  private:
    vector<Room> rooms;
    void genBaseDun();
    void addRooms(int numRooms); 
    void makePathToRoom(Room from, Room to);
    void addHallways();
    bool noOverlapFound(Room newRoom);
    void putRoomsInDungeon();
};
