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
    uint8_t hardness;

    Tile(TileType type);
    void setType(TileType type);
    void setType(int hardness);
};

class Room {
  public:
    uint8_t x;
    uint8_t y;
    uint8_t height;
    uint8_t width;
    Room();
    Room(int y, int x, int height, int width);
};

class Dungeon {
  public:
    vector <vector<Tile>> map;

    Dungeon(); 
    Dungeon(string loc);
    void draw();

  private:
    vector<Room> rooms;
    void genBaseDun();
    void addRooms(int numRooms); 
    void makePathToRoom(Room from, Room to);
    void addHallways();
    bool noOverlapFound(Room newRoom);
    void putRoomsInDungeon();
    void readTiles(FILE* file);
    void readRooms(FILE* file, int fileSize);
    uint32_t endianSwap_uInt(int input);
};
