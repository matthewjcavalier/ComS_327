#define DUNGEON_H
#include "dungeon.h"

/**
 * @brief Construct a new Tile:: Tile object
 * 
 * @param type 
 */
Tile::Tile(TileType type) {
  this->type = type;
  setType(type);
}

/**
 * @brief Set the tile to a new type based on the input
 * 
 * @param type 
 */
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
    case UPSTAIR:
    case DOWNSTAIR:
      hardness = 0;
      break;
    case ROCK:
      hardness = rand() % MAX_HARDNESS;
      break;
  }
}

/**
 * @brief Set the tile to a new type based on the hardness input
 * 
 * @param hardness 
 */
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

/**
 * @brief Construct a new Room:: Room object1
 * 
 */
Room::Room() {
  height = MIN_ROOM_HEIGHT + rand() % 15;
  width = MIN_ROOM_WIDTH + rand() % 15;

  y = rand() % (MAX_HEIGHT - 2 - height) + 1;
  x = rand() % (MAX_WIDTH - 2 - width) + 1;
}

/**
 * @brief Construct a new Room:: Room object
 * 
 * @param y 
 * @param x 
 * @param height 
 * @param width 
 */
Room::Room(int y, int x, int height, int width) {
  this->y = y;
  this->x = x;
  this->height = height;
  this->width = width;
}

/**
 * @brief Construct a new Dungeon:: Dungeon object
 * 
 */
Dungeon::Dungeon() {
  build();
}

/**
 * @brief go through the steps to build the dungeon
 * 
 */
void Dungeon::build() {
  genBaseDun();
  addRooms(MIN_ROOM_COUNT);
  addHallways();
  addStairs();

}
/**
 * @brief Rebuild the existing dungeon
 * 
 */
void Dungeon::rebuild() {
  for(int y = 0; y < MAX_HEIGHT; y++){
    for(int x = 0; x < MAX_WIDTH; x++) {
      charMap[y][x] = NULL;
      if(objectMap[y][x]) {
        //objectMap[y][x]->~object();
      }
      objectMap[y][x] = NULL;
      if(y > 0 && y < MAX_HEIGHT - 1 && x > 0 && x < MAX_WIDTH) {
        map[y][x].setType(ROCK);
      }
    }
  }
  build();
}

/**
 * @brief Construct a new Dungeon:: Dungeon object from a saved dungeon file
 * 
 * @param loc 
 */
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

/**
 * @brief  function to swap the endianness of an input value
 * 
 * @param input 
 * @return uint32_t endianSwap_uInt 
 */
uint32_t Dungeon:: endianSwap_uInt(int input) {
  uint32_t leftMost = (input << 8 * 3) & 0xff000000;
  uint32_t left = (input << 8) & 0x00ff0000;
  uint32_t right = (input >> 8) & 0x0000ff00;
  uint32_t rightMost = (input >> 8 * 3) & 0x000000ff;
  uint32_t ret = leftMost + left + right + rightMost;
  
  return ret;
}

/**
 * @brief read each tile of the dungeon from a file and build out the dungeon's tiles
 * 
 * @param file 
 */
void Dungeon::readTiles(FILE* file) {
  for(int row = 0; row < MAX_HEIGHT; row++) {
    for(int col = 0; col < MAX_WIDTH; col++) {
      uint8_t hardness;
      fread(&hardness, sizeof(uint8_t), 1, file);
      map[row][col].setType(hardness);
    }
  }
}

/**
 * @brief  read each room's info from the file
 * 
 * @param file 
 * @param fileSize 
 */
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

/**
 * @brief gnerate a dungeon that only has rock and borders
 * 
 */
void Dungeon::genBaseDun() {
  Tile normalTile(ROCK);
  Tile borderTile(BORDER);
  map.resize(MAX_HEIGHT);
  charMap.resize(MAX_HEIGHT);
  objectMap.resize(MAX_HEIGHT);

  // make the basic dungeon with the first and last rows border
  for(int i = 0; i < MAX_HEIGHT; i++) {
    if(i == 0 || i == MAX_HEIGHT -1) {
      map[i].resize(MAX_WIDTH, borderTile);
    } else {
      map[i].resize(MAX_WIDTH, normalTile);
    }
    charMap[i].resize(MAX_WIDTH, NULL);
    objectMap[i].resize(MAX_WIDTH, NULL);
  }

  for(int i = 0; i < MAX_HEIGHT; i++) {
    map[i][0].setType(BORDER);
    map[i][MAX_WIDTH - 1].setType(BORDER);
  }
}

/**
 * @brief Add rooms to the dungeon with a lower bound on the number of rooms needed
 * 
 * @param numRooms 
 */
void Dungeon::addRooms(int numRooms) {
  bool complete = false;
  int counter = 0;
  int randRoomCount = rand() % 5 + numRooms;

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

/**
 * @brief  place the rooms from the room list into the dungeon
 * 
 */
void Dungeon::putRoomsInDungeon() {
  for(Room room : rooms) {
    for(int y = room.y; y < room.y + room.height; y++) {
      for(int x = room.x; x < room.x + room.width; x++) {
        map[y][x].setType(ROOM);
      }
    }
  }
}

/**
 * @brief check if the input room will have any overlap with any existing rooms
 * 
 * @param newRoom 
 * @return true 
 * @return false 
 */
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

/**
 * @brief add hallways to the dungeon
 * 
 */
void Dungeon::addHallways() {
  for(int i = 0; i < (int) rooms.size() - 1; i++) {
    makePathToRoom(rooms[i], rooms[i+1]);
  }
}

/**
 * @brief add an up and a downstairs to the dungeon
 * 
 */
void Dungeon::addStairs() {
  int x;
  int y;
  do {
    x = rand() % MAX_WIDTH;
    y = rand() % MAX_HEIGHT;
  } while(!isOpenSpace({y,x}));
  map[y][x].type = UPSTAIR;
  do {
    x = rand() % MAX_WIDTH;
    y = rand() % MAX_HEIGHT;
  } while(!isOpenSpace({y,x}));
  map[y][x].type = DOWNSTAIR;
}

/**
 * @brief  make a path from one room to another to make the hallway
 * 
 * @param from 
 * @param to 
 */
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

/**
 * @brief save the dungeon to disk
 * 
 * @param loc 
 */
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

/**
 * @brief Draw the entire dungeon as well as all Characters
 * 
 */
void Dungeon::draw() {
  char toDraw = '~';
  for(int row = 0; row < MAX_HEIGHT; row++) {
    for(int col = 0; col < MAX_WIDTH; col++) {
      if(charMap[row][col] != NULL) {
        toDraw = charMap[row][col]->symbol;
      } else if(objectMap[row][col] != NULL) {
        toDraw = objectMap[row][col]->symbol;
      } else {
        toDraw = getTileSym(map[row][col].type);
      }
      drawCharacter({row + 1, col}, toDraw);
    }
  }
}

/**
 * @brief Update the dungeon's distance maps from the PC
 * 
 */
void Dungeon::updateDistMaps() {
  int x = pc->coord.x;
  int y = pc->coord.y;
  openMap = genDistMap(y, x, false);
  tunnelMap = genDistMap(y, x, true);
  /*
  // print the open distance map
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
  
  // print the tunneling map
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
  */
}

/**
 * @brief Return if the spot in the dungeon at the input coord is open space (hardness of zero)
 * 
 * @param coord 
 * @return true 
 * @return false 
 */
bool Dungeon::isOpenSpace(Coordinate coord) {
  return map[coord.y][coord.x].hardness == 0;
}

/**
 * @brief return if their is a direct path from the from coordinate to the to coordinate with no obstructions
 * 
 * @param from 
 * @param to 
 * @return true 
 * @return false 
 */
bool Dungeon::canSeeFrom(Coordinate from, Coordinate to) {
  if(from.y == to.y && from.x == to.x) {
    return true;
  }
  Coordinate curr = from;
  do{
    if(curr.y != to.y) {
      curr.y += (curr.y > to.y) ? -1 : 1;
    }
    if(curr.x != to.x) {
      curr.x += (curr.x > to.x) ? -1 : 1;
    }
    if(!isOpenSpace(curr)) {
      return false;
    }
  } while(curr.y != to.y || curr.x != to.x);
  return true;
}

/**
 * @brief generate a distance map, taking into account where the PC is (the y, x location), and if the requested map is
 *        for tunneling
 * 
 * @param y 
 * @param x 
 * @param canTunnel 
 * @return vector<vector<int>> 
 */
vector<vector<int>> Dungeon::genDistMap(int y, int x, bool canTunnel) {
  vector<vector<int>> distMap = getEmptyMap();
  distMap[y][x] = 0;
  vector<Coordinate> queue;
  Coordinate initial(y,x);
  queue.push_back(initial);

  fillDistMap(distMap, queue, canTunnel);

  return distMap;
}

/**
 * @brief A version of Dijkstra's algo to fill the map
 * 
 * @param distMap 
 * @param queue 
 * @param canTunnel 
 */
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

/**
 * @brief second part of the Dijkstra algo
 * 
 * @param distMap 
 * @param queue 
 * @param pair 
 * @param canTunnel 
 */
void Dungeon::updateDistMap(vector<vector<int>>& distMap, vector<Coordinate>& queue, CoordPair pair, bool canTunnel) {
  Coordinate neighbor = pair.comparingTo;
  Coordinate current = pair.initial;
  if(current.y <= 0 || current.y >= MAX_HEIGHT ||
      current.x <= 0 || current.x >= MAX_WIDTH ||
      neighbor.y <= 0 || neighbor.y >= MAX_HEIGHT ||
      neighbor.x <= 0 || neighbor.x >= MAX_WIDTH) {
    return;
  }

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

/**
 * @brief get a random coordinate that is an open space in the dungeon
 * 
 * @return Coordinate 
 */
Coordinate Dungeon::getEmptySpace() {
  int row = 0;
  int col = 0;
  while(map[row][col].hardness != 0 && charMap[row][col] == NULL) {
    row = rand() % MAX_HEIGHT;
    col = rand() % MAX_WIDTH;
  }
  Coordinate coord(row,col);
  return coord;
}

/**
 * @brief  update the input coordinate of the character map to point to the input location
 * 
 * @param coord 
 * @param ptr 
 */
void Dungeon::updateSpace(Coordinate coord, Character* ptr) {
  charMap[coord.y][coord.x] = ptr;
}

/**
 * @brief  set the Dungeon's pointer to the PC
 * 
 * @param pc 
 */
void Dungeon::setPC(PC* pc) {
  this->pc = pc;
  updateDistMaps();
}

/**
 * @brief Get an empty 2d vector of ints
 * 
 * @return vector<vector<int>> 
 */
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

/**
 * @brief Get the symbol that cooresponds to the input enum
 * 
 * @param type 
 * @return char 
 */
char getTileSym(TileType type) {
  switch(type) {
    case ROOM:
      return ROOM_CHAR;
    case HALL:
      return HALL_CHAR;
    case UPSTAIR:
      return UPSTAIR_CHAR;
    case DOWNSTAIR:
      return DOWNSTAIR_CHAR;
    default:
      return ROCK_CHAR;
  }
}
