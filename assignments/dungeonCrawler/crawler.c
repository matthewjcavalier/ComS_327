#include <stdio.h>
#include <stdlib.h>

#define MAX_DUNGEON_HEIGHT 21
#define MAX_DUNGEON_WIDTH 80
#define MAX_ROCK_HARDNESS 5
#define IMMUTABLE -1
#define OPEN_SPACE 0

typedef enum {false, true} boolean;

typedef struct {
  int hardness;
  boolean isBorder;
  boolean isHallway;
  boolean isRoom;
} Tile;

typedef struct {
  Tile map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH];
} Dungeon;

Dungeon* genDungeon();

void setHardnesses(Dungeon* dun);

void placeRooms(Dungeon* dun);

void placeHallways(Dungeon* dun);

int main(int argc, char* argv[]) {
  // set up the dungeon
  Dungeon* dungeon;
  dungeon = genDungeon();
  
  // print the dungeon
  
}

Dungeon* genDungeon() {
  Dungeon* newDungeon;
  newDungeon = malloc(sizeof(Dungeon));
  
  setHardnesses(newDungeon);

  placeRooms(newDungeon);

  placeHallways(newDungeon);

  return newDungeon;
}

void setHardnesses(Dungeon* dun) {
  int row, col;
  for(row = 1; row < MAX_DUNGEON_HEIGHT - 1; row++) {
      for(col = 1; col < MAX_DUNGEON_WIDTH - 1; col ++) {
      }
  }
}

void placeRooms(Dungeon* dun) {

}

void placeHallways(Dungeon* dun) {

}

