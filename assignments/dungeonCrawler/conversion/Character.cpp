#define CHARACTER_H
#include "Character.h"


using namespace std;

Character::Character() {
  coord = {0,0};
  symbol = '&';
  speed = -1;
  nextEventTime = -1;
  dun = new Dungeon;
  id = -1;
}

Character::~Character() {}

int Character::takeTurn() {
  cout << "generic Character took turn" << endl;
  return 0;
}

int Character::moveRand() {
  int newX = 0;
  int newY = 0;
  int modX = 0;
  int modY = 0;
  int foundId;

  while(dun->map[newY][newX].hardness != 0) {
    if(rand() % 2 == 0) {
      modX = (rand() % 2 == 0) ? 1 : -1;
    } else {
      modX = 0;
    }
    if(rand() % 2 == 0) {
      modY = (rand() % 2 == 0) ? 1 : -1;
    } else {
      modY = 0;
    }
    newX = coord.x + modX;
    newY = coord.y + modY;
  }
  
  dun->updateSpace(coord, NULL);
  foundId = getCharacterId(coord);
  coord = {newY, newX};
  dun->updateSpace(coord, this);
  return foundId;
}

int Character::moveToward(Coordinate coord) {
  Coordinate to = this->coord;
  if(to.y != coord.y) {
    to.y += (to.y > coord.y) ? -1 : 1;
  }
  if(to.x != coord.x) {
    to.x += (to.x > coord.x) ? -1 : 1;
  }
  return moveTo(to);
}

int Character::moveTo(Coordinate coord) {
  int foundId = getCharacterId(coord);
  if(dun->isOpenSpace(coord)) {
    dun->updateSpace(this->coord, NULL);
    this->coord = coord;
    dun->updateSpace(this->coord, this);
  }
  return foundId;
}

int Character::getCharacterId(Coordinate loc) {
  if(dun->charMap[loc.y][loc.x] != NULL) {
    return dun->charMap[loc.y][loc.x]->id;
  }
  return 0;
}

PC::PC(int id, Coordinate coord, int speed, Dungeon* dun, int nextEventTime) {
  symbol = '@';
  this->coord = coord;
  this->speed = speed;
  this->dun = dun;
  this->nextEventTime = nextEventTime;
  this->id = id;
  dun->updateSpace(coord, this);
}

PC::~PC(){}

int PC::takeTurn() {
  nextEventTime += 1000/speed;
  int userPressed;
  movementResDTO res;
  dun->draw();
  do {
    userPressed = getch();
    drawCharacter({0,0}, userPressed);
    // move up left
    if(userPressed == '7' || userPressed == 'y') {
      res = tryToMove({coord.y - 1, coord.x -1});
    }
    // move up
    if(userPressed == '8' || userPressed == 'k') {
      res = tryToMove({coord.y - 1, coord.x});
    }
    // move up right
    if(userPressed == '9' || userPressed == 'u') {
      res = tryToMove({coord.y - 1, coord.x + 1});
    }
    // move right
    if(userPressed == '6' || userPressed == 'l') {
      res = tryToMove({coord.y, coord.x + 1});
    }
    // move down right
    if(userPressed == '3' || userPressed == 'n') {
      res = tryToMove({coord.y + 1, coord.x + 1});
    }
    // move down
    if(userPressed == '2' || userPressed == 'j') {
      res = tryToMove({coord.y + 1, coord.x});
    }
    // move down left
    if(userPressed == '1' || userPressed == 'b') {
      res = tryToMove({coord.y + 1, coord.x - 1});
    }
    // move left
    if(userPressed == '4' || userPressed == 'h') {
      res = tryToMove({coord.y, coord.x - 1});
    }
    // rest for a turn
    if(userPressed == '5' || userPressed == ' ') {
      res.success = true; 
    }
    // Quit
    if(userPressed == 'Q') {
      res.success = true; 
      res.killed = this->id;
    }
    // go down stairs
    if(userPressed == '>') {
      if(dun->map[coord.y][coord.x].type == DOWNSTAIR) {
        return MOVE_BETWEEN_FLOORS;
      }
    }
    // go up stairs
    if(userPressed == '<') {
      if(dun->map[coord.y][coord.x].type == UPSTAIR) {
        return MOVE_BETWEEN_FLOORS;
      }
    }
    // go up stairs
    if(userPressed == 'm') {
      showMonsterList();
    }
  } while(!res.success);
  dun->updateDistMaps();
  dun->draw();
  return res.killed;
}

void PC::showMonsterList() {
  int topLeft_x = 13;
  int topLeft_y = 5;
  int vertBorderWidth = 2;
  int horBorderWidth = 5;
  int stringAreaWidth = 50;
  //int currentTopMonst = 0;
  int numMonstersShown = 5;

  // init monster list
  vector<string> monsterList;
  
  // fill the list
  monsterList = fillMonsterList();
  
  drawMonsterBox(topLeft_y, topLeft_x, horBorderWidth, vertBorderWidth, numMonstersShown, stringAreaWidth);
  
}

void PC::drawMonsterBox(int topLeft_y, int topLeft_x, int horBorderWidth, int vertBorderWidth, int numMonstersShown, int stringAreaWidth) {
  for(int row = topLeft_y; row < topLeft_y + horBorderWidth + numMonstersShown - 1; row++) {
    for(int col = topLeft_x; col < topLeft_x + vertBorderWidth + stringAreaWidth - 1; col++) {
      if(row == topLeft_y || row == topLeft_y + horBorderWidth + numMonstersShown - 2) {
        drawCharacter({row + 1, col}, '_');
      } else if(col == topLeft_x || col == topLeft_x + vertBorderWidth + stringAreaWidth - 2) {
        drawCharacter({row + 1, col}, '|');
      } else {
        drawCharacter({row + 1, col}, ' ');
      }
    }
  }
}

vector<string> PC::fillMonsterList() {
  vector<string> monsterList;
  for(auto row : dun->charMap) {
    for(Character* character : row) {
      if(character != NULL && character->symbol != '@') {
        monsterList.push_back(genMonsterString(this->coord.y - character->coord.y, 
              this->coord.x - character->coord.x, character->symbol));
      }
    }
  }
  return monsterList;
}

string PC::genMonsterString(int ydiff, int xdiff, char symbol) {
  ostringstream ret;
  if(ydiff == 0) {
    ret << symbol << " is " << abs(xdiff) << " to the " << ((xdiff > 0) ? "West" : "East");
  } else if(xdiff == 0) {
    ret << symbol << " is " << abs(ydiff) << " to the " << ((ydiff > 0) ? "North" : "South");
  } else {
    ret << symbol << " is " << abs(ydiff) << " to the " << ((ydiff > 0) ? "North" : "South") << " and " << abs(xdiff) << " to the " << ((xdiff > 0) ? "West" : "East");
  }
  return ret.str();
}

movementResDTO PC::tryToMove(Coordinate to) {
  movementResDTO res;
  if(dun->isOpenSpace(to)) {
    dun->updateSpace(coord, NULL);
    res.killed = getCharacterId(to);
    coord = to;
    dun->updateSpace(coord, this);
    res.success = true;
  }
  return res;
}

NPC::NPC(int id, Coordinate coord, int speed, Dungeon* dun, int nextEventTime, char type, PC* pc) {
  this->coord = coord;
  this->speed = speed;
  this->dun = dun;
  this->type = type;
  this->nextEventTime = nextEventTime;
  this->symbol = getSymbol(type);
  this->pc = pc;
  this->id = id;
  lastSeenPCLoc = {0,0};
  dun->updateSpace(coord, this);
  setTurnLogic();
}

NPC::~NPC(){}

void NPC::setTurnLogic() {
  switch(type) {
    case 0b0001:
      turnLogic = &NPC::movement0001;
      break;
    case 0b0010:
      turnLogic = &NPC::movement0010;
      break;
    case 0b0011:
      turnLogic = &NPC::movement0011;
      break;
    case 0b0100:
      turnLogic = &NPC::movement0100;
      break;
    case 0b0101:
      turnLogic = &NPC::movement0101;
      break;
    case 0b0110:
      turnLogic = &NPC::movement0110;
      break;
    case 0b0111:
      turnLogic = &NPC::movement0111;
      break;
    case 0b1001:
      turnLogic = &NPC::movement1001;
      break;
    case 0b1010:
      turnLogic = &NPC::movement1010;
      break;
    case 0b1011:
      turnLogic = &NPC::movement1011;
      break;
    case 0b1100:
      turnLogic = &NPC::movement1100;
      break;
    case 0b1101:
      turnLogic = &NPC::movement1101;
      break;
    case 0b1110:
      turnLogic = &NPC::movement1110;
      break;
    case 0b1111:
      turnLogic = &NPC::movement1111;
      break;
    default:
      turnLogic = &Character::moveRand;
  }
}

int NPC::takeTurn() {
  nextEventTime += 1000/speed;
  int foundId = (this->*turnLogic)();
  return foundId;
}

void NPC::updatePCLoc() {
  if(dun->canSeeFrom(coord, pc->coord)) {
    lastSeenPCLoc = pc->coord;
  } else if(coord.x == lastSeenPCLoc.x && coord.y == lastSeenPCLoc.y) {
    lastSeenPCLoc = {0,0};
  }
}

int NPC::movement0001() {
  updatePCLoc();
  if(lastSeenPCLoc.x != 0 && lastSeenPCLoc.y != 0) {
    return moveToward(lastSeenPCLoc);
  } else {
    return moveRand();
  }
}
int NPC::movement0010(){
  return moveToward(pc->coord);
}
int NPC::movement0011(){
  Coordinate next = coord;
  int min = INT_MAX;
  for(int y = coord.y - 1; y < coord.y + 2; y++) {
    for(int x = coord.x - 1; x < coord.x + 2; x++) {
      if(dun->openMap[y][x] < min) {
        min = dun->openMap[y][x];
        next = {y,x};
      }
    }
  }
  return moveTo(next);
}
int NPC::movement0100(){
  return moveRandTunneling();
}
int NPC::movement0101(){
  updatePCLoc();
  if(lastSeenPCLoc.x != 0 && lastSeenPCLoc.y != 0) {
    return moveToward(lastSeenPCLoc);
  } else {
    return moveRandTunneling();
  }
}

int NPC::movement0110(){
  return moveTowardTunneling(pc->coord);
}
int NPC::movement0111(){
  Coordinate next = coord;
  int min = INT_MAX;
  int foundID = 0;
  for(int y = coord.y - 1; y < coord.y + 2; y++) {
    for(int x = coord.x - 1; x < coord.x + 2; x++) {
      if(dun->tunnelMap[y][x] < min) {
        min = dun->tunnelMap[y][x];
        next = {y,x};
      }
    }
  }

  if(dun->map[next.y][next.x].hardness > 0) {
    if(dun->map[next.y][next.x].hardness < 85) {
      dun->map[next.y][next.x].hardness = 0;
    } else {
      dun->map[next.y][next.x].hardness -= 85;
    }
    dun->updateDistMaps();
  }

  if(dun->map[next.y][next.x].hardness <= 0) {
    if(dun->map[next.y][next.x].type != HALL && dun->map[next.y][next.x].type != ROOM) {
      dun->map[next.y][next.x].setType(0);
    }
    dun->updateSpace(this->coord, NULL);
    foundID = getCharacterId(next);
    this->coord = {next.y, next.x};
    dun->updateSpace(this->coord, this);
  }
  return foundID;
}

int NPC::movement1001() {
  return (rand() % 2) ? moveRand() : movement0001();
}
int NPC::movement1010(){
  return (rand() % 2) ? moveRand() : movement0010();
}

// Erratic Tunneling Telepathy Intelligence
int NPC::movement1011(){
  return (rand() % 2) ? moveRand() : movement0011();
}

int NPC::movement1100(){
  return movement0100();
}

int NPC::movement1101(){
  return (rand() % 2) ? moveRand() : movement0101();
}

int NPC::movement1110(){
  return (rand() % 2) ? moveRand() : movement0110();
}

int NPC::movement1111(){
  return (rand() % 2) ? moveRand() : movement0111();
}

int NPC::moveTowardTunneling(Coordinate coord) {
  Coordinate to = this->coord;
  int foundId;
  if(to.y != coord.y) {
    to.y += (to.y > coord.y) ? -1 : 1;
  }
  if(to.x != coord.x) {
    to.x += (to.x > coord.x) ? -1 : 1;
  }

  if(dun->map[to.y][to.x].hardness > 0) {
    if(dun->map[to.y][to.x].hardness < 85) {
      dun->map[to.y][to.x].hardness = 0;
    } else {
      dun->map[to.y][to.x].hardness -= 85;
    }
  }

  if(dun->map[to.y][to.x].hardness <= 0) {
    if(dun->map[to.y][to.x].type != HALL && dun->map[to.y][to.x].type != ROOM) {
      dun->map[to.y][to.x].setType(0);
    }
    dun->updateSpace(this->coord, NULL);
    foundId = getCharacterId(to);
    this->coord = {to.y, to.x};
    dun->updateSpace(this->coord, this);
  }
  return foundId;
}

int NPC::moveRandTunneling() {
  int newX = 0;
  int newY = 0;
  int modX = 0;
  int modY = 0;
  int foundId = 0;

  do {
    if(rand() % 2 == 0) {
      modX = (rand() % 2 == 0) ? 1 : -1;
    }
    if(rand() % 2 == 0) {
      modY = (rand() % 2 == 0) ? 1 : -1;
    }
    newX = coord.x + modX;
    newY = coord.y + modY;
  } while(dun->map[newY][newX].type == BORDER);
  
  if(dun->map[newY][newX].hardness > 0) {
    if(dun->map[newY][newX].hardness < 85) {
      dun->map[newY][newX].hardness = 0;
    } else {
      dun->map[newY][newX].hardness -= 85;
    }
  }

  if(dun->map[newY][newX].hardness <= 0) {
    if(dun->map[newY][newX].type != HALL && dun->map[newY][newX].type != ROOM) {
      dun->map[newY][newX].setType(0);
    }
    dun->updateSpace(coord, NULL);
    foundId = getCharacterId(coord);
    coord = {newY, newX};
    dun->updateSpace(coord, this);
  }
  return foundId;
}

char getSymbol(char type) {
  switch(type) {
    case 1:
      return '1';
    case 2:
      return '2';
    case 3:
      return '3';
    case 4:
      return '4';
    case 5:
      return '5';
    case 6:
      return '6';
    case 7:
      return '7';
    case 8:
      return '8';
    case 9:
      return '9';
    case 10:
      return 'A';
    case 11:
      return 'B';
    case 12:
      return 'C';
    case 13:
      return 'D';
    case 14:
      return 'E';
    case 15:
      return 'F';
    default:
      return '0';
  }
}
