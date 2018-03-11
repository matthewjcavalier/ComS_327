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
      hardness = 0;
      break;
    case ROOM:
      hardness = 0;
      break;
    case ROCK:
      hardness = rand() % MAX_HARDNESS;
      break;
  }
}

void Tile::setType(int hardness) {
  this->hardness = hardness;
  switch(hardness) {
    case(MAX_HARDNESS):
      type = BORDER;
      break;
    case(0):
      type = HALL;
      break;
    default:
      type = ROCK;
  }
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

Dungeon::Dungeon(string loc) {
  char* header = (char*)malloc(sizeof(char) * 12);
  FILE* file = fopen(loc.c_str(), "r");
  uint32_t versionMarker;
  uint32_t fileSize;

  genBaseDun();

  // read the header
  fread(header, 12, 1, file);

  // get version marker
  fread(&versionMarker, sizeof(uint32_t), 1, file);

  // get the size of the file
  fread(&fileSize, sizeof(uint32_t), 1, file);
  fileSize = endianSwap_uInt(fileSize);

  // set cell types and hardnesses
  readTiles(file);
  
  readRooms(file, fileSize);

  fclose(file);

  free(header);
}

uint32_t Dungeon:: endianSwap_uInt(int input) {
  uint32_t leftMost = (input << 8 * 3) & 0xff000000;
  uint32_t left = (input << 8) & 0x00ff0000;
  uint32_t right = (input >> 8) & 0x0000ff00;
  uint32_t rightMost = (input >> 8 * 3) & 0x000000ff;
  uint32_t ret = leftMost + left + right + rightMost;
  
  return ret;
}

void Dungeon::readTiles(FILE* file) {
  for(int row = 0; row < MAX_HEIGHT; row++) {
    for(int col = 0; col < MAX_WIDTH; col++) {
      uint8_t hardness;
      fread(&hardness, sizeof(uint8_t), 1, file);
      map[row][col].setType(hardness);
    }
  }
}

void Dungeon::readRooms(FILE* file, int fileSize) {
  for(int roomNum = 0; roomNum < (int) (fileSize - 1700) / 4; roomNum++) {

    Room newRoom;
    fread(&newRoom.y, sizeof(uint8_t), 1, file);
    fread(&newRoom.x, sizeof(uint8_t), 1, file);
    fread(&newRoom.height, sizeof(uint8_t), 1, file);
    fread(&newRoom.width, sizeof(uint8_t), 1, file);

    rooms.push_back(newRoom);
  }
  putRoomsInDungeon();
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

void Dungeon::save(string loc) {
  FILE* file = fopen(loc.c_str(), "w");
  char str[] = {'R','L','G','3','2','7','-','S','2','0','1','8'};
  uint32_t version = 0;
  uint32_t roomStorageSize = 4; //bytes
  uint32_t fileSize = 1700 + (roomStorageSize * rooms.size());

  fileSize = endianSwap_uInt(fileSize);

  fwrite(str, sizeof(str), 1, file);
  fwrite(&version, sizeof(uint32_t), 1, file);
  fwrite(&fileSize, sizeof(uint32_t), 1, file);

  for(int row = 0; row < MAX_HEIGHT; row++) {
    for(int col = 0; col < MAX_WIDTH; col++) {
      fwrite(&map[row][col].hardness, sizeof(uint8_t), 1, file);
    }
  }

  for(int i = 0; i < (int) rooms.size(); i++) {
    fwrite(&rooms[i].y, sizeof(uint8_t), 1, file);
    fwrite(&rooms[i].x, sizeof(uint8_t), 1, file);
    fwrite(&rooms[i].height, sizeof(uint8_t), 1, file);
    fwrite(&rooms[i].width, sizeof(uint8_t), 1, file);
  }
  fclose(file);
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

void Dungeon::updateDistMaps() {
  int y = 6;
  int x = 53;
  openMap = genDistMap(y, x, false);
  tunnelMap = genDistMap(y, x, true);

  for(int row = 0; row < MAX_HEIGHT; row++) {
    for(int col = 0; col < MAX_WIDTH; col++) {
      if(row == y && col == x) {
        cout << "@";
      } else {
        if(openMap[row][col] == INT_MAX){
          cout << " ";
        } else {
          cout << openMap[row][col] % 10;
        }
      }
    }
    cout << endl;
  }
  for(int row = 0; row < MAX_HEIGHT; row++) {
    for(int col = 0; col < MAX_WIDTH; col++) {
      if(row == y && col == x) {
        cout << "@";
      } else {
        if(tunnelMap[row][col] == INT_MAX){
          cout << " ";
        } else {
          cout << tunnelMap[row][col] % 10;
        }
      }
    }
    cout << endl;
  }
}

vector<vector<int>> Dungeon::genDistMap(int y, int x, bool canTunnel) {
  vector<vector<int>> distMap = getEmptyMap();
  distMap[y][x] = 0;
  vector<Coordinate> queue;
  Coordinate initial(y,x);
  queue.push_back(initial);

  fillDistMap(distMap, queue, canTunnel);

  return distMap;
}

void Dungeon::fillDistMap(vector<vector<int>>& distMap, vector<Coordinate>& queue, bool canTunnel) {
  while(queue.size() > 0) {
    Coordinate coord = queue[0];
    queue.erase(queue.begin());

    for(int y = coord.y - 1; y < coord.y + 2; y++) {
      for(int x = coord.x - 1; x < coord.x + 2; x++) {
        Coordinate current(y, x);
        CoordPair pair(coord, current);
        updateDistMap(distMap, queue, pair, canTunnel);
      }
    }
  }
}

void Dungeon::updateDistMap(vector<vector<int>>& distMap, vector<Coordinate>& queue, CoordPair pair, bool canTunnel) {
  Coordinate neighbor = pair.comparingTo;
  Coordinate current = pair.initial;
  int currentDistMapVal = distMap[current.y][current.x];
  int neighborDistMapVal = distMap[neighbor.y][neighbor.x];
  int distance = 1;

  if(map[pair.comparingTo.y][pair.comparingTo.x].type != BORDER) {
    if(map[pair.comparingTo.y][pair.comparingTo.x].hardness == 0) {
      if(distMap[neighbor.y][neighbor.x] > currentDistMapVal + distance) {
        distMap[neighbor.y][neighbor.x] = currentDistMapVal + distance;
        queue.push_back(neighbor);
      }
    } else if(canTunnel) {
      if(neighborDistMapVal > currentDistMapVal + map[current.y][current.x].hardness / 85 + distance) {
        distMap[neighbor.y][neighbor.x] = currentDistMapVal + map[current.y][current.x].hardness / 85 + distance;
        queue.push_back(neighbor);
      }
    }
  }
}

vector<vector<int>> getEmptyMap() {
  vector<vector<int>> emptyMap;
  emptyMap.resize(MAX_HEIGHT);

  // make the basic dungeon with the first and last rows border
  for(int i = 0; i < MAX_HEIGHT; i++) {
      emptyMap[i].resize(MAX_WIDTH);
  }

  for(int row = 0; row < MAX_HEIGHT; row++) {
    for(int col = 0; col < MAX_WIDTH; col++) {
      emptyMap[row][col] = INT_MAX;
    }
  }
  return emptyMap;
}
