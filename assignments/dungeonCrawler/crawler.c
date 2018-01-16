#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_DUNGEON_HEIGHT 21
#define MAX_DUNGEON_WIDTH 80
#define MAX_ROCK_HARDNESS 5
#define IMMUTABLE -1
#define OPEN_SPACE 0
#define BORDER_HOR_CHAR '-'
#define BORDER_VERT_CHAR '|'
#define ROOM_CHAR '.'
#define HALL_CHAR '#'

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

void addBorders(Dungeon* dun);

void setHardnesses(Dungeon* dun);

void placeRooms(Dungeon* dun);

void placeHallways(Dungeon* dun);

void printDungeon(Dungeon* dun);

int main(int argc, char* argv[]) {
  // set up the dungeon
  Dungeon* dungeon;
  dungeon = genDungeon();
  
  // print the dungeon
  printDungeon(dungeon);
}

Dungeon* genDungeon() {
  Dungeon* newDungeon;
  newDungeon = malloc(sizeof(Dungeon));
  
  addBorders(newDungeon);

  setHardnesses(newDungeon);

  placeRooms(newDungeon);

  placeHallways(newDungeon);

  return newDungeon;
}

void addBorders(Dungeon* dun) {
  int row, col;

  // add borders to the first row and last row
  for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
    dun->map[0][col].isBorder = true;
    dun->map[MAX_DUNGEON_HEIGHT - 1][col].isBorder = true;
  }
  
  // add borders to the first and last column
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    dun->map[row][0].isBorder = true;
    dun->map[row][MAX_DUNGEON_WIDTH - 1].isBorder = true;
  }
}

void setHardnesses(Dungeon* dun) {
  int row, col;

  srand(time(0));
  for(row = 1; row < MAX_DUNGEON_HEIGHT - 1; row++) {
      for(col = 1; col < MAX_DUNGEON_WIDTH - 1; col ++) {
        dun->map[row][col].hardness = rand() % MAX_ROCK_HARDNESS + 1;
      }
  }
}

void placeRooms(Dungeon* dun) {

}

void placeHallways(Dungeon* dun) {

}

void printDungeon(Dungeon* dun) {
  int row, col;
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      // if the tile is a border
      if(dun->map[row][col].isBorder) {
        // if the cell is in the first or last row
        if(row == 0 || row == MAX_DUNGEON_HEIGHT - 1) {
          printf("%c", BORDER_HOR_CHAR);
        }
        // else the cell is one either in the first or last col
        else {
          printf("%c", BORDER_VERT_CHAR);
        }
      }
      // else the tile is part of the normal dungeon
      else {
        printf("%d", dun->map[row][col].hardness);
      }
    }
    printf("\n");
  }
}
