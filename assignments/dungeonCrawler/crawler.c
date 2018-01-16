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
#define MIN_ROOM_HEIGHT 2
#define MIN_ROOM_WIDTH 3
#define EXPECTED_ROOM_COUNT 10
#define MIN_ROOM_COUNT 5


typedef enum {false, true} boolean;

typedef struct {
  int xPos;
  int yPos;
  int height;
  int width;
} Room;

typedef struct {
  int hardness;
  boolean isBorder;
  boolean isHallway;
  boolean isRoom;
} Tile;

typedef struct {
  Tile map[MAX_DUNGEON_HEIGHT][MAX_DUNGEON_WIDTH];
  Room rooms[EXPECTED_ROOM_COUNT];
} Dungeon;

Dungeon* genDungeon();

void zeroOutDungeon(Dungeon* dun);

void addBorders(Dungeon* dun);

void setHardnesses(Dungeon* dun);

void placeRooms(Dungeon* dun);

void clearRooms(Dungeon* dun);

boolean isValidRoomPlacement(int xLoc, int yLoc, Dungeon* dun, Room newRoom);

void addRoomToDungeon(Dungeon* dun, Room newRoom); 

void placeHallways(Dungeon* dun);

void printDungeon(Dungeon* dun);

int main(int argc, char* argv[]) {
  // seed random num gen
  srand(time(0));

  // set up the dungeon
  Dungeon* dungeon;
  dungeon = genDungeon();
  
  // print the dungeon
  printDungeon(dungeon);
}

Dungeon* genDungeon() {
  Dungeon* newDungeon;
  newDungeon = malloc(sizeof(Dungeon));

  zeroOutDungeon(newDungeon);
  
  addBorders(newDungeon);

  placeRooms(newDungeon);

  placeHallways(newDungeon);

  setHardnesses(newDungeon);

  return newDungeon;
}

void zeroOutDungeon(Dungeon* dun) {
  int row, col;
  for(row = 0; row < MAX_DUNGEON_HEIGHT; row++) {
    for(col = 0; col < MAX_DUNGEON_WIDTH; col++) {
      dun->map[row][col].hardness = -1;
      dun->map[row][col].isBorder = false;
      dun->map[row][col].isHallway = false;
      dun->map[row][col].isRoom = false;
    }
  }
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

  for(row = 1; row < MAX_DUNGEON_HEIGHT - 1; row++) {
      for(col = 1; col < MAX_DUNGEON_WIDTH - 1; col ++) {
        if(dun->map[row][col].isHallway == false && dun->map[row][col].isRoom == false) {
          dun->map[row][col].hardness = rand() % MAX_ROCK_HARDNESS + 1;
        }
      }
  }
}

void placeRooms(Dungeon* dun) {
  boolean roomsPlacedSuccess = false;
  int roomNum, xLoc, yLoc; 
  int totalRoomsPlaced = 0;
  Room currentRoom;

  // set room sizes
  for(roomNum = 0; roomNum < EXPECTED_ROOM_COUNT; roomNum++) {
    dun->rooms[roomNum].height = MIN_ROOM_HEIGHT + (rand() % 10);
    dun->rooms[roomNum].width = MIN_ROOM_WIDTH + (rand() % 10);
  }

  while(totalRoomsPlaced <= MIN_ROOM_COUNT) {
    totalRoomsPlaced = 0;
    clearRooms(dun);
    for(roomNum = 0; roomNum < EXPECTED_ROOM_COUNT; roomNum++) {
      // minus 2 for the two borders, plus 1 to start at col 1
      xLoc = rand() % (MAX_DUNGEON_WIDTH - 2) + 1;
      // minus 2 for the two borders, plus 1 to start at row 1
      yLoc = rand() % (MAX_DUNGEON_HEIGHT - 2) + 1;
      
      if(isValidRoomPlacement(xLoc, yLoc, dun, dun->rooms[roomNum])) {
        dun->rooms[roomNum].xPos = xLoc;
        dun->rooms[roomNum].yPos = yLoc;
        
        addRoomToDungeon(dun, dun->rooms[roomNum]);
        totalRoomsPlaced++;
      } else {
        break;
      }
    }
  }
}

void clearRooms(Dungeon* dun) {
  int roomNum, row, col;
  // loop through and clear room coords
  for(roomNum = 0; roomNum < EXPECTED_ROOM_COUNT; roomNum++) {
    dun->rooms[roomNum].xPos = -1;
    dun->rooms[roomNum].yPos = -1;
  }

  // loop through and clear the dungeon map
  for(row = 1; row < MAX_DUNGEON_HEIGHT - 1; row++) {
    for(col = 1; col < MAX_DUNGEON_WIDTH - 1; col++) {
      dun->map[row][col].isRoom = false;
    }
  }
}
boolean isValidRoomPlacement(int xLoc, int yLoc, Dungeon* dun, Room newRoom) {
  int col, row;
  boolean ret = true;
  for(row = yLoc - 1; row < newRoom.height + 2; row++) {
    for(col = xLoc - 1; col < newRoom.width + 2; col++) {
      if(dun->map[row][col].isRoom) {
        ret = false;
        break;
      }
    }
  }
  return ret;
}

void addRoomToDungeon(Dungeon* dun, Room newRoom) {
  int row, col;
  for(row = newRoom.yPos; row < newRoom.height + newRoom.yPos; row++) {
    for(col = newRoom.xPos; col < newRoom.width + newRoom.xPos; col++) {
      dun->map[row][col].isRoom = true;
    }
  }
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
        if(dun->map[row][col].isRoom) {
          printf("%c", ROOM_CHAR);
        } else if(dun->map[row][col].isHallway) {
          printf("%c", HALL_CHAR);
        }

        // debugging print of hardness
        else {
          printf("%d", dun->map[row][col].hardness);
        }
      }
    }
    printf("\n");
  }
}
