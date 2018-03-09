#ifndef DUNGEON_H
  #define DUNGEON_H
  #include "dungeon.h"
#endif

Tile::Tile(TileType type) {
  this->type = type;
  setType(type);
}

void Tile::setType(TileType type) {
  this->type = type;
  switch(type) {
    case BORDER:
      hardness = MAX_HARDNESS;
      break;
    case HALL:
    case ROOM:
      hardness = 0;
    case ROCK:
      hardness = rand() % MAX_HARDNESS;
      break;
  }
}

void Tile::makeBorder() {
  type = BORDER;
  hardness = MAX_HARDNESS;
}

Room::Room() {
  height = MIN_ROOM_HEIGHT + rand() % 15;
  width = MIN_ROOM_WIDTH + rand() % 15;

  y = rand() % (MAX_HEIGHT - 2 - height) + 1;
  x = rand() % (MAX_WIDTH - 2 - width) + 1;
}

Room::Room(int y, int x, int height, int width) {
  this->y = y;
  this->x = x;
  this->height = height;
  this->width = width;
}

Dungeon::Dungeon() {
  genBaseDun();
  addRooms(MIN_ROOM_COUNT);
  addHallways();
}

void Dungeon::genBaseDun() {
  Tile normalTile(ROCK);
  Tile borderTile(BORDER);
  map.resize(MAX_HEIGHT);

  // make the basic dungeon with the first and last rows border
  for(int i = 0; i < MAX_HEIGHT; i++) {
    if(i == 0 || i == MAX_HEIGHT -1) {
      map[i].resize(MAX_WIDTH, borderTile);
    } else {
      map[i].resize(MAX_WIDTH, normalTile);
    }
  }

  for(int i = 0; i < MAX_HEIGHT; i++) {
    map[i][0].setType(BORDER);
    map[i][MAX_WIDTH - 1].setType(BORDER);
  }
}

void Dungeon::addRooms(int numRooms) {
  bool complete = false;
  int counter = 0;
  int randRoomCount = rand() % 5 + MIN_ROOM_COUNT;

  while(!complete) {
    if(counter > 500) {
      rooms.clear();
      counter = 0;
    }
    // try to keep adding rooms
    Room newRoom;
    
    if(noOverlapFound(newRoom)) {
      rooms.push_back(newRoom);
    } else if((int) rooms.size() >= randRoomCount){
      complete = true;
    } else {
      counter++;
    }
  }
  putRoomsInDungeon();
}

void Dungeon::putRoomsInDungeon() {
  for(Room room : rooms) {
    for(int y = room.y; y < room.y + room.height; y++) {
      for(int x = room.x; x < room.x + room.width; x++) {
        map[y][x].setType(ROOM);
      }
    }
  }
}

bool Dungeon::noOverlapFound(Room newRoom) {
  for(Room room : rooms){
    for(int y = room.y - 1; y < room.y + room.height + 1; y++) {
      for(int x = room.x - 1; x < room.x + room.width + 1; x++) {
        if(y >= newRoom.y && y <= newRoom.y + newRoom.height && x >= newRoom.x && x <= newRoom.x + newRoom.width){
          return false;
        }
      }
    }
  }
  return true;
}

void Dungeon::addHallways() {
  for(int i = 0; i < (int) rooms.size() - 1; i++) {
    makePathToRoom(rooms[i], rooms[i+1]);
  }
}

void  Dungeon::makePathToRoom(Room from, Room to) {
  int vertMod = (from.y > to.y) ? -1 : 1;
  int horzMod = (from.x > to.x) ? -1 : 1;

  int currY = from.y;
  int currX = from.x;

  while(currX != to.x || currY != to.y) {
    if((currY != to.y && rand() % 2) || currX == to.x) {
      currY += vertMod;
    } else {
      currX += horzMod;
    }
    if(map[currY][currX].type != ROOM) {
      map[currY][currX].setType(HALL);
    }
  }
}

void Dungeon::draw() {
  for(auto row: map) {
    for(Tile tile: row) {
      switch (tile.type) {
        case BORDER:
          cout<< "|";
          break;
        case ROOM:
          cout<< ".";
          break;
        case HALL:
          cout<< "#";
          break;
        default:
          cout<< " ";
      }
    }
    cout<<endl;
  }
}
