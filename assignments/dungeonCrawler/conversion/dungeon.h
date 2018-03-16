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
#ifndef CLIMITS
  #define CLIMITS
  #include <climits>
#endif
#ifndef CHARACTER_H
  #define CHARACTER_H
  #include "Character.h"
#endif
#ifndef SCREEN_H
  #define SCREEN_H
  #include "screen.h"
#endif


using namespace std;

class Character;
class PC;

enum TileType {
  ROCK,
  HALL,
  ROOM,
  BORDER
};

class CoordPair {
  public:
    Coordinate initial;
    Coordinate comparingTo;
    CoordPair(Coordinate initial, Coordinate comparingTo) {
      this->initial = initial;
      this->comparingTo = comparingTo;
    }
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
    vector <vector<int>> tunnelMap;
    vector <vector<int>> openMap;
    vector <vector<Character*>> charMap;

    Dungeon(); 
    Dungeon(string loc);
    void save(string loc);
    void draw();
    void updateDistMaps();
    vector<vector<int>> genDistMap(int y, int x, bool canTunnel);
    Coordinate getEmptySpace();
    void updateSpace(Coordinate coord, Character* ptr);
    bool isOpenSpace(Coordinate coord);
    bool canSeeFrom(Coordinate from, Coordinate to);
    void setPC(PC* pc);

  private:
    vector<Room> rooms;
    PC* pc;
    void genBaseDun();
    void addRooms(int numRooms); 
    void makePathToRoom(Room from, Room to);
    void addHallways();
    bool noOverlapFound(Room newRoom);
    void putRoomsInDungeon();
    void readTiles(FILE* file);
    void readRooms(FILE* file, int fileSize);
    uint32_t endianSwap_uInt(int input);
    void fillDistMap(vector<vector<int>>& distMap, vector<Coordinate>& queue, bool canTunnel);
    void updateDistMap(vector<vector<int>>& distMap, vector<Coordinate>& queue, CoordPair pair, bool canTunnel);
};


vector<vector<int>> getEmptyMap();
